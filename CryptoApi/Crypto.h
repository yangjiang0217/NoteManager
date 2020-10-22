#pragma once
#include "aes.h"
#include "blowfish.h"
#include <string>

class CryptoV1{
public:
    CryptoV1();

    CryptoV1(const void* srcBytes, size_t srclen);

    std::string EncryptString(const std::string& str);

    std::string DecryptString(const std::string& str);

protected:
    void BytesToHex(const void* src, size_t len, char* dst);

    bool CheckHex(const char* src, size_t len);

    void HexToBytes(const char* src, size_t len, void* dst);

    std::string EncryptString(const void* srcBytes, size_t srclen);

    std::string DecryptString(const char* srchex, size_t srclen);

protected:
    ACCEL_BLOWFISH_KEY BlowfishKey;
};

class CryptoV2
    : public CryptoV1
{
public:

    CryptoV2();

    CryptoV2(const void* srcBytes, size_t srclen);

    std::string EncryptString(const std::string& str);

    std::string DecryptString(const std::string& str);

    std::string EncryptStringForNCX(const std::string& str);

    std::string DecryptStringForNCX(const std::string& str);

protected:
    std::string EncryptStringForNCX(const void* srcBytes, size_t srclen);

    std::string DecryptStringForNCX(const char* srchex, size_t srclen);

protected:
    ACCEL_AES_KEY AES128Key;
};
