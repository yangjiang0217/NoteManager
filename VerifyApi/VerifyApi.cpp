// VerifyApi.cpp : 定义 DLL 的导出函数。
#include "pch.h"
#include "framework.h"
#include "VerifyApi.h"
#include "VerifyManager.h"


// 验证密钥
VERIFY_API int VERIFY_CALL Verify_Key(const char *pKey)
{
    return CVerifyManager::GetInstance().VerifyKey(pKey);
}

// 更新密钥
VERIFY_API int VERIFY_CALL Verify_UpdateKey(const char *pNeyKey, const char *pOldKey)
{
    return CVerifyManager::GetInstance().UpdateKey(pNeyKey, pOldKey);
}
