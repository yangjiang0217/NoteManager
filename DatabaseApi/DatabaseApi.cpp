#include "pch.h"
#include "framework.h"
#include "DatabaseApi.h"
#include "DatabaseManager.h"

// 连接数据库
DATABASE_API int DATABASE_CALL DBM_Connect()
{
    return CDatabaseManager::GetInstance().ConnectDB();
}
// 断开数据库连接
DATABASE_API int DATABASE_CALL DBM_Disconnect()
{
    return CDatabaseManager::GetInstance().DisconnectDB();
}
// 添加便签
DATABASE_API int DATABASE_CALL DBM_AddNote(const char *pPlatform, const char *pAccount, const char *pPassword)
{
    return CDatabaseManager::GetInstance().AddNote(pPlatform, pAccount, pPassword);
}
// 删除便签
DATABASE_API int DATABASE_CALL DBM_DeleteNote(int nNoteID)
{
    return CDatabaseManager::GetInstance().DeleteNote(nNoteID);
}
// 更新便签
DATABASE_API int DATABASE_CALL DBM_UpdateNote(int nNoteID, const char *pPlatform, const char *pAccount, const char *pPassword)
{
    return CDatabaseManager::GetInstance().UpdateNote(nNoteID, pPlatform, pAccount, pPassword);
}
// 更新平台
DATABASE_API int DATABASE_CALL DBM_UpdatePlatform(int nNoteID, const char *pPlatform)
{
    return CDatabaseManager::GetInstance().UpdatePlatform(nNoteID, pPlatform);
}
// 更新账户
DATABASE_API int DATABASE_CALL DBM_UpdateAccount(int nNoteID, const char *pAccount)
{
    return CDatabaseManager::GetInstance().UpdateAccount(nNoteID, pAccount);
}
// 更新密码
DATABASE_API int DATABASE_CALL DBM_UpdatePassword(int nNoteID, const char *pPassword)
{
    return CDatabaseManager::GetInstance().UpdatePassword(nNoteID, pPassword);
}
// 查询便签
DATABASE_API int DATABASE_CALL DBM_QueryNote(QueryNoteResultCallback fun, void *pUser)
{
    return CDatabaseManager::GetInstance().QueryNote(fun, pUser);
}
// 更新密钥
DATABASE_API int DATABASE_CALL DBM_UpdateKey(const char *pNewKey, const char *pOldKey)
{
    return CDatabaseManager::GetInstance().UpdateKey(pNewKey, pOldKey);
}
// 查询密钥
DATABASE_API int DATABASE_CALL DBM_QueryKey(QueryKeyResultCallback fun, void *pUser)
{
    return CDatabaseManager::GetInstance().QueryKey(fun, pUser);
}
