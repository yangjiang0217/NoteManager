#include "pch.h"
#include "VerifyManager.h"
#include "../DatabaseApi/DatabaseApi.h"
#include "../CryptoApi/CryptoApi.h"
#ifdef _WIN32
#ifdef _DEBUG
#pragma comment(lib,"../lib/Debug/DatabaseApi.lib")
#pragma comment(lib,"../lib/Debug/CryptoApi.lib")
#else
#pragma comment(lib,"../lib/Release/DatabaseApi.lib")
#pragma comment(lib,"../lib/Release/CryptoApi.lib")
#endif
#endif

// 构造函数
CVerifyManager::CVerifyManager()
    : m_nVerify(-1)
    , m_szUser{ 0 }
    , m_szPassword{ 0 }
{
}
// 析构函数
CVerifyManager::~CVerifyManager()
{
}
// 本类用单件模式实现，通过此静态函数来获取类的唯一实例
CVerifyManager & CVerifyManager::GetInstance()
{
    static CVerifyManager sInstance;
    return sInstance;
}
// 验证用户
int CVerifyManager::VerifyUser(const char *pUserName, const char *pPassword)
{
    CRYPTO_EncryptString(pUserName, m_pDefaultKey, m_szUser, sizeof(m_szUser));
    CRYPTO_EncryptString(pPassword, m_pDefaultKey, m_szPassword, sizeof(m_szPassword));
    int nRet = DBM_Connect();
    if (nRet != 0)
    {
        return -1;
    }
    DBM_QueryUser(QueryUserResult, this);
    DBM_Disconnect();
    return m_nVerify;
}
// 修改密码
int CVerifyManager::ModifyPassword(const char *pUserName, const char *pOldPassword, const char *pPassword)
{
    int nRet = VerifyUser(pUserName, pOldPassword);
    if (nRet != 0)
    {
        return -1;
    }
    CRYPTO_EncryptString(pPassword, m_pDefaultKey, m_szPassword, sizeof(m_szPassword));
    return DBM_ModifyUserPassword(m_szUser, m_szPassword);
}
// 查询用户结果回调函数
int CVerifyManager::QueryUserResult(int nUserID, const char *pUserName, const char *pPassword, void *pUser)
{
    if (NULL == pUser)
    {
        return 0;
    }
    CVerifyManager *pThis = static_cast<CVerifyManager*>(pUser);
    return pThis->VerifyUserResult(pUserName, pPassword);
}
// 验证查询返回用户
int CVerifyManager::VerifyUserResult(const char *pUserName, const char *pPassword)
{
    if (NULL == pUserName || NULL == pPassword)
    {
        return 0;
    }
    if (strcmp(m_szUser, pUserName) == 0 && strcmp(m_szPassword, pPassword) == 0)
    {
        m_nVerify = 0;
        return -1;
    }
    else
    {
        m_nVerify = -1;
        return 0;
    }
}
