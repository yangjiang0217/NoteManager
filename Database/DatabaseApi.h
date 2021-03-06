/**
*   @file       DatabaseApi.h
*   @version    V1.0
*   @date       2020-10-17
*   @author     yangjiang
*   @brief      数据库接口
*/
#ifndef _DATABASE_API_H_
#define _DATABASE_API_H_

#ifdef DATABASE_EXPORTS
#define DATABASE_API __declspec(dllexport)
#else
#define DATABASE_API __declspec(dllimport)
#endif
#define DATABASE_CALL __stdcall

#ifdef __cplusplus
extern "C" {
#endif
    // 查询便签结果回调函数，返回!0表示不继续接收
    typedef int QueryNoteResultCallback(int nNoteID, const char *pPlatform, const char *pAccount, const char *pPassword, void *pUser);
    // 查询用户结果回调函数，返回!0表示不继续接收
    typedef int QueryKeyResultCallback(int nKeyID, const char *pKey, void *pUser);

    // 连接数据库
    DATABASE_API int DATABASE_CALL DBM_Connect();
    // 断开数据库连接
    DATABASE_API int DATABASE_CALL DBM_Disconnect();
    // 添加便签
    DATABASE_API int DATABASE_CALL DBM_AddNote(const char *pPlatform, const char *pAccount, const char *pPassword);
    // 删除便签
    DATABASE_API int DATABASE_CALL DBM_DeleteNote(int nNoteID);
    // 更新便签
    DATABASE_API int DATABASE_CALL DBM_UpdateNote(int nNoteID, const char *pPlatform, const char *pAccount, const char *pPassword);
    // 更新平台
    DATABASE_API int DATABASE_CALL DBM_UpdatePlatform(int nNoteID, const char *pPlatform);
    // 更新账户
    DATABASE_API int DATABASE_CALL DBM_UpdateAccount(int nNoteID, const char *pAccount);
    // 更新密码
    DATABASE_API int DATABASE_CALL DBM_UpdatePassword(int nNoteID, const char *pPassword);
    // 查询便签
    DATABASE_API int DATABASE_CALL DBM_QueryNote(QueryNoteResultCallback fun, void *pUser);
    // 更新密钥
    DATABASE_API int DATABASE_CALL DBM_UpdateKey(const char *pNewKey, const char *pOldKey);
    // 查询密钥
    DATABASE_API int DATABASE_CALL DBM_QueryKey(QueryKeyResultCallback fun, void *pUser);
#ifdef __cplusplus
}
#endif
#endif // !_DATABASE_API_H_
