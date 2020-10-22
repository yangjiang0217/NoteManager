// DatabaseApi.cpp : 定义 DLL 的导出函数。
#include "pch.h"
#include "framework.h"
#include "DatabaseApi.h"
#include "DatabaseManager.h"

// 连接数据库
DATABASE_API int DBM_Connect()
{
    return CDatabaseManager::GetInstance().ConnectDB();
}
// 断开数据库连接
DATABASE_API int DBM_Disconnect()
{
    return CDatabaseManager::GetInstance().DisconnectDB();
}
// 添加便签
DATABASE_API int DBM_AddNote(const char *pPlatform, const char *pAccount, const char *pPassword)
{
    return CDatabaseManager::GetInstance().AddNote(pPlatform, pAccount, pPassword);
}
// 删除便签
DATABASE_API int DBM_DeleteNote(int nNoteID)
{
    return CDatabaseManager::GetInstance().DeleteNote(nNoteID);
}
// 更新便签
DATABASE_API int DBM_UpdateNote(int nNoteID, const char *pPlatform, const char *pAccount, const char *pPassword)
{
    return CDatabaseManager::GetInstance().UpdateNote(nNoteID, pPlatform, pAccount, pPassword);
}
// 更新平台
DATABASE_API int DBM_UpdatePlatform(int nNoteID, const char *pPlatform)
{
    return CDatabaseManager::GetInstance().UpdatePlatform(nNoteID, pPlatform);
}
// 更新账户
DATABASE_API int DBM_UpdateAccount(int nNoteID, const char *pAccount)
{
    return CDatabaseManager::GetInstance().UpdateAccount(nNoteID, pAccount);
}
// 更新密码
DATABASE_API int DBM_UpdatePassword(int nNoteID, const char *pPassword)
{
    return CDatabaseManager::GetInstance().UpdatePassword(nNoteID, pPassword);
}
// 查询便签
DATABASE_API int DBM_QueryNote(QueryNoteResultCallback fun, void *pUser)
{
    return CDatabaseManager::GetInstance().QueryNote(fun, pUser);
}
// 添加用户
DATABASE_API int DBM_AddUser(const char *pUserName, const char *pPassword)
{
    return CDatabaseManager::GetInstance().AddUser(pUserName, pPassword);
}
// 删除用户
DATABASE_API int DBM_DeleteUser(int nUserID)
{
    return CDatabaseManager::GetInstance().DeleteUser(nUserID);
}
// 更新用户
DATABASE_API int DBM_UpdateUser(int nUserID, const char *pUserName, const char *pPassword)
{
    return CDatabaseManager::GetInstance().UpdateUser(nUserID, pUserName, pPassword);
}
// 修改用户密码
DATABASE_API int DBM_ModifyUserPassword(const char *pUserName, const char *pPassword)
{
    return CDatabaseManager::GetInstance().UpdateUser(pUserName, pPassword);
}
// 查询用户
DATABASE_API int DBM_QueryUser(QueryUserResultCallback fun, void *pUser)
{
    return CDatabaseManager::GetInstance().QueryUser(fun, pUser);
}
