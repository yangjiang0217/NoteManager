/**
*   @file       CryptoApi.h
*   @version    V1.0
*   @date       2020-10-17
*   @author     yangjiang
*   @brief      加密接口
*/
#ifndef _CRYPTO_API_H_
#define _CRYPTO_API_H_

#ifdef CRYPTOAPI_EXPORTS
#define CRYPTOAPI_API __declspec(dllexport)
#else
#define CRYPTOAPI_API __declspec(dllimport)
#endif
#define CRYPTOAPI_CALL __stdcall

#ifdef __cplusplus
extern "C" {
#endif
    // 加密
    CRYPTOAPI_API int CRYPTOAPI_CALL CRYPTO_EncryptString(const char *pText, const char *pKey, char *pOut, unsigned int nOutLen);
    // 解密
    CRYPTOAPI_API int CRYPTOAPI_CALL CRYPTO_DecryptString(const char *pText, const char *pKey, char *pOut, unsigned int nOutLen);
#ifdef __cplusplus
}
#endif
#endif // !_CRYPTO_API_H_