/**
*   @file       VerifyApi.h
*   @version    V1.0
*   @date       2020-10-17
*   @author     yangjiang
*   @brief      数据库接口
*/
#ifndef _VERIFY_API_H_
#define _VERIFY_API_H_

#ifdef VERIFYAPI_EXPORTS
#define VERIFY_API __declspec(dllexport)
#else
#define VERIFY_API __declspec(dllimport)
#endif
#define VERIFY_CALL

#ifdef __cplusplus
extern "C" {
#endif
    // 验证密钥
    VERIFY_API int VERIFY_CALL Verify_Key(const char *pKey);
    // 更新密钥
    VERIFY_API int VERIFY_CALL Verify_UpdateKey(const char *pNeyKey, const char *pOldKey);
#ifdef __cplusplus
}
#endif
#endif // !_VERIFY_API_H_
