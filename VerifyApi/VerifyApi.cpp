// VerifyApi.cpp : 定义 DLL 的导出函数。
#include "pch.h"
#include "framework.h"
#include "VerifyApi.h"
#include "VerifyManager.h"


// 验证用户
VERIFYAPI_API int Verify_User(const char *pUserName, const char *pPassword)
{
    return CVerifyManager::GetInstance().VerifyUser(pUserName, pPassword);
}

// 修改密码
VERIFYAPI_API int Verify_ModifyPassword(const char *pUserName, const char *pOldPassword, const char *pPassword)
{
    return CVerifyManager::GetInstance().ModifyPassword(pUserName, pOldPassword, pPassword);
}