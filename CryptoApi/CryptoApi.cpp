// CryptoApi.cpp : 定义 DLL 的导出函数。
#include "pch.h"
#include "framework.h"
#include "CryptoApi.h"
#include "Crypto.h"

// 加密
CRYPTOAPI_API int CRYPTO_EncryptString(const char *pText, const char *pKey, char *pOut, unsigned int nOutLen)
{
    try
    {
        if (NULL == pText || NULL == pKey || NULL == pOut || nOutLen <= 0)
        {
            return -1;
        }
        CryptoV2 nc(pKey, strlen(pKey));
        std::string strText = pText;
        std::string strEnc = nc.EncryptString(strText);
        snprintf(pOut, nOutLen - 1, strEnc.c_str());
    }
    catch (...)
    {
        return -1;
    }
    return 0;
}
// 解密
CRYPTOAPI_API int CRYPTO_DecryptString(const char *pText, const char *pKey, char *pOut, unsigned int nOutLen)
{
    try
    {
        CryptoV2 nc(pKey, strlen(pKey));
        std::string strText = pText;
        std::string strDec = nc.DecryptString(strText);
        snprintf(pOut, nOutLen - 1, strDec.c_str());
    }
    catch (...)
    {
        return -1;
    }
    return 0;
}
