#include "sha1.h"
#include "md5.h"
#include <vector>
#include "Crypto.h"

CryptoV1::CryptoV1()
{
    const uint8_t DefaultKey[8] = {
        '3', 'D', 'C', '5', 'C', 'A', '3', '9'
    };
    ACCEL_SHA1_DIGEST KeyHash;
    accel_SHA1(DefaultKey, sizeof(DefaultKey), &KeyHash);
    accel_Blowfish_set_key(KeyHash.byte, sizeof(KeyHash), &BlowfishKey);
}

CryptoV1::CryptoV1(const void* srcBytes, size_t srclen)
{
    ACCEL_SHA1_DIGEST KeyHash;
    accel_SHA1(srcBytes, srclen, &KeyHash);
    accel_Blowfish_set_key(KeyHash.byte, sizeof(KeyHash), &BlowfishKey);
}

std::string CryptoV1::EncryptString(const std::string& str)
{
    return EncryptString(str.c_str(), str.length());
}

std::string CryptoV1::DecryptString(const std::string& str)
{
    return DecryptString(str.c_str(), str.length());
}

void CryptoV1::BytesToHex(const void* src, size_t len, char* dst)
{
    for (size_t i = 0; i < len; ++i)
    {
        char h = reinterpret_cast<const uint8_t*>(src)[i] >> 4;
        char l = reinterpret_cast<const uint8_t*>(src)[i] & 0x0F;

        h += h >= 10 ? 'A' - 10 : '0';
        l += l >= 10 ? 'A' - 10 : '0';
        dst[2 * i] = h;
        dst[2 * i + 1] = l;
    }
}

bool CryptoV1::CheckHex(const char* src, size_t len)
{
    if (len % 2 != 0)
    {
        return false;
    }

    for (size_t i = 0; i < len; i += 2)
    {
        char h = src[i];
        char l = src[i + 1];

        if (src[i] < '0' || src[i] > 'F')
        {
            return false;
        }
        if (src[i] < 'A' && src[i] > '9')
        {
            return false;
        }
        if (src[i + 1] < '0' || src[i + 1] > 'F')
        {
            return false;
        }
        if (src[i + 1] < 'A' && src[i + 1] > '9')
        {
            return false;
        }
    }

    return true;
}

void CryptoV1::HexToBytes(const char* src, size_t len, void* dst)
{
    for (size_t i = 0; i < len; i += 2)
    {
        uint8_t h = src[i];
        uint8_t l = src[i + 1];

        h -= h > '9' ? 'A' - 10 : '0';
        l -= l > '9' ? 'A' - 10 : '0';

        reinterpret_cast<uint8_t*>(dst)[i / 2] = (h << 4) ^ l;
    }
}

std::string CryptoV1::EncryptString(const void* srcBytes, size_t srclen)
{
    std::string ret = "";

    if (srclen == 0)
    {
        return ret;
    }

    ret.resize(srclen * 2);

    uint8_t CV[BLOWFISH_BLOCK_SIZE] = {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
    };
    accel_Blowfish_encrypt(CV, &BlowfishKey, BLOWFISH_BIG_ENDIAN);

    const uint64_t* blocks = reinterpret_cast<const uint64_t*>(srcBytes);
    size_t blocks_len = srclen / BLOWFISH_BLOCK_SIZE;
    for (size_t i = 0; i < blocks_len; ++i)
    {
        union {
            uint8_t byte[8];
            uint64_t qword;
        } temp;

        temp.qword = blocks[i];
        temp.qword ^= *reinterpret_cast<uint64_t*>(CV);
        accel_Blowfish_encrypt(temp.byte, &BlowfishKey, BLOWFISH_BIG_ENDIAN);
        *reinterpret_cast<uint64_t*>(CV) ^= temp.qword;

        BytesToHex(temp.byte, 8, const_cast<char*>(ret.data()) + 16 * i);
    }

    if (srclen % BLOWFISH_BLOCK_SIZE)
    {
        accel_Blowfish_encrypt(CV, &BlowfishKey, BLOWFISH_BIG_ENDIAN);
        for (size_t i = 0; i < srclen % BLOWFISH_BLOCK_SIZE; ++i)
            CV[i] ^= reinterpret_cast<const uint8_t*>(blocks + blocks_len)[i];
        BytesToHex(CV, srclen % BLOWFISH_BLOCK_SIZE, const_cast<char*>(ret.data()) + 16 * blocks_len);
    }

    return ret;
}

std::string CryptoV1::DecryptString(const char* srchex, size_t srclen)
{
    std::string ret = "";

    if (CheckHex(srchex, srclen) == false)
    {
        return ret;
    }

    ret.resize(srclen / 2);

    uint8_t CV[BLOWFISH_BLOCK_SIZE] = {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
    };
    accel_Blowfish_encrypt(CV, &BlowfishKey, BLOWFISH_BIG_ENDIAN);

    const char(*blocks)[16] = reinterpret_cast<const char(*)[16]>(srchex);
    size_t blocks_len = srclen / 16;
    for (size_t i = 0; i < blocks_len; ++i)
    {
        union {
            uint8_t byte[8];
            uint64_t qword;
        } temp, temp2;

        HexToBytes(blocks[i], 16, temp.byte);
        std::memcpy(temp2.byte, temp.byte, 8);
        accel_Blowfish_decrypt(temp.byte, &BlowfishKey, BLOWFISH_BIG_ENDIAN);
        temp.qword ^= *reinterpret_cast<uint64_t*>(CV);
        *reinterpret_cast<uint64_t*>(const_cast<char*>(ret.data()) + 8 * i) = temp.qword;
        *reinterpret_cast<uint64_t*>(CV) ^= temp2.qword;
    }

    if (srclen % 16)
    {
        union {
            uint8_t byte[8];
            uint64_t qword;
        } temp = { };
        HexToBytes(blocks[blocks_len], srclen % 16, temp.byte);

        accel_Blowfish_encrypt(CV, &BlowfishKey, BLOWFISH_BIG_ENDIAN);
        for (size_t i = 0; i < (srclen % 16) / 2; ++i)
            ret[blocks_len * 8 + i] = temp.byte[i] ^ CV[i];
    }

    return ret;
}




CryptoV2::CryptoV2() : CryptoV1()
{
    uint8_t DefaultKey[16] = {
        'l', 'i', 'b', 'c', 'c', 'k', 'e', 'y',
        'l', 'i', 'b', 'c', 'c', 'k', 'e', 'y'
    };
    accel_AES128_set_key(DefaultKey, &AES128Key);
}

CryptoV2::CryptoV2(const void* srcBytes, size_t srclen)
    : CryptoV1(srcBytes, srclen)
{
#if 0
    uint8_t DefaultKey[16] = {
        'l', 'i', 'b', 'c', 'c', 'k', 'e', 'y',
        'l', 'i', 'b', 'c', 'c', 'k', 'e', 'y'
    };
#else
    MD5_CTX Md5Ctx = { 0 };
    MD5Init(&Md5Ctx);
    MD5Update(&Md5Ctx, (unsigned char*)srcBytes, srclen);
    MD5Final(&Md5Ctx);
    uint8_t DefaultKey[16] = { 0 };
    for (int i = 0; i < 16; i++)
    {
        DefaultKey[i] = Md5Ctx.digest[i];
    }
#endif
    accel_AES128_set_key(DefaultKey, &AES128Key);
}

std::string CryptoV2::EncryptString(const std::string& str)
{
    return EncryptStringForNCX(str.c_str(), str.length());
}

std::string CryptoV2::DecryptString(const std::string& str)
{
    return DecryptStringForNCX(str.c_str(), str.length());
}

std::string CryptoV2::EncryptStringForNCX(const std::string& str)
{
    return EncryptStringForNCX(str.c_str(), str.length());
}

std::string CryptoV2::DecryptStringForNCX(const std::string& str)
{
    return DecryptStringForNCX(str.c_str(), str.length());
}

std::string CryptoV2::EncryptStringForNCX(const void* srcBytes, size_t srclen)
{
    std::string ret = "";

    if (srclen == 0)
    {
        return ret;
    }

    ret.resize((srclen / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE * 2);

    union {
        uint8_t byte[AES_BLOCK_SIZE];
        uint64_t qword[2];
    } CV = {
        'l', 'i', 'b', 'c', 'c', 'i', 'v', ' ',
        'l', 'i', 'b', 'c', 'c', 'i', 'v', ' '
    };

    const uint8_t(*blocks)[AES_BLOCK_SIZE] = reinterpret_cast<const uint8_t(*)[AES_BLOCK_SIZE]>(srcBytes);
    size_t blocks_len = srclen / AES_BLOCK_SIZE;
    for (size_t i = 0; i < blocks_len; ++i)
    {
        union {
            uint8_t byte[AES_BLOCK_SIZE];
            uint64_t qword[2];
        } temp;

        std::memcpy(temp.byte, blocks[i], AES_BLOCK_SIZE);
        temp.qword[0] ^= CV.qword[0];
        temp.qword[1] ^= CV.qword[1];
        accel_AES128_encrypt(temp.byte, &AES128Key);
        BytesToHex(temp.byte, AES_BLOCK_SIZE, const_cast<char*>(ret.data()) + 2 * AES_BLOCK_SIZE * i);
        CV.qword[0] = temp.qword[0];
        CV.qword[1] = temp.qword[1];
    }

    uint8_t padding = AES_BLOCK_SIZE - srclen % AES_BLOCK_SIZE;
    union {
        uint8_t byte[AES_BLOCK_SIZE];
        uint64_t qword[2];
    } temp;

    std::memcpy(temp.byte, blocks[blocks_len], srclen % AES_BLOCK_SIZE);
    for (size_t i = srclen % AES_BLOCK_SIZE; i < AES_BLOCK_SIZE; ++i)
    {
        temp.byte[i] = padding;
    }

    temp.qword[0] ^= CV.qword[0];
    temp.qword[1] ^= CV.qword[1];
    accel_AES128_encrypt(temp.byte, &AES128Key);
    BytesToHex(temp.byte, AES_BLOCK_SIZE, const_cast<char*>(ret.data()) + 2 * AES_BLOCK_SIZE * blocks_len);

    return ret;
}

std::string CryptoV2::DecryptStringForNCX(const char* srchex, size_t srclen)
{
    std::string ret = "";

    if (srclen % (2 * AES_BLOCK_SIZE) != 0 || CheckHex(srchex, srclen) == false)
    {
        return ret;
    }

    ret.reserve(srclen / 2);
    ret.resize(srclen / 2 - AES_BLOCK_SIZE);
    union {
        uint8_t byte[AES_BLOCK_SIZE];
        uint64_t qword[2];
    } CV = {
        'l', 'i', 'b', 'c', 'c', 'i', 'v', ' ',
        'l', 'i', 'b', 'c', 'c', 'i', 'v', ' '
    };

    const char(*blocks)[2 * AES_BLOCK_SIZE] = reinterpret_cast<const char(*)[2 * AES_BLOCK_SIZE]>(srchex);
    size_t blocks_len = srclen / (2 * AES_BLOCK_SIZE) - 1;
    for (size_t i = 0; i < blocks_len; ++i)
    {
        union {
            uint8_t byte[AES_BLOCK_SIZE];
            uint64_t qword[2];
        } temp, NextVector;

        HexToBytes(blocks[i], 2 * AES_BLOCK_SIZE, temp.byte);
        std::memcpy(NextVector.byte, temp.byte, AES_BLOCK_SIZE);
        accel_AES128_decrypt(temp.byte, &AES128Key);
        temp.qword[0] ^= CV.qword[0];
        temp.qword[1] ^= CV.qword[1];

        std::memcpy(const_cast<char*>(ret.data()) + AES_BLOCK_SIZE * i, temp.byte, AES_BLOCK_SIZE);
        std::memcpy(CV.byte, NextVector.byte, AES_BLOCK_SIZE);
    }

    union {
        uint8_t byte[AES_BLOCK_SIZE];
        uint64_t qword[2];
    } temp;
    HexToBytes(blocks[blocks_len], 2 * AES_BLOCK_SIZE, temp.byte);
    accel_AES128_decrypt(temp.byte, &AES128Key);
    temp.qword[0] ^= CV.qword[0];
    temp.qword[1] ^= CV.qword[1];

    if (temp.byte[AES_BLOCK_SIZE - 1] > AES_BLOCK_SIZE)
    {
        ret.clear();
        return ret;
    }

    uint8_t padding = temp.byte[AES_BLOCK_SIZE - 1];
    for (size_t i = AES_BLOCK_SIZE - padding; i < AES_BLOCK_SIZE; ++i)
    {
        if (temp.byte[i] != padding)
        {
            ret.clear();
            return ret;
        }
    }

    ret.append(reinterpret_cast<char*>(temp.byte), AES_BLOCK_SIZE - padding);
    return ret;
}

