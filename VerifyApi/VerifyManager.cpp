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

// ���캯��
CVerifyManager::CVerifyManager()
    : m_nVerify(-1)
    , m_szUser{ 0 }
    , m_szPassword{ 0 }
{
}
// ��������
CVerifyManager::~CVerifyManager()
{
}
// �����õ���ģʽʵ�֣�ͨ���˾�̬��������ȡ���Ψһʵ��
CVerifyManager & CVerifyManager::GetInstance()
{
    static CVerifyManager sInstance;
    return sInstance;
}
// ��֤�û�
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
// �޸�����
int CVerifyManager::ModifyPassword(const char *pUserName, const char *pOldPassword, const char *pPassword)
{
    // ��֤�û�
    CRYPTO_EncryptString(pUserName, m_pDefaultKey, m_szUser, sizeof(m_szUser));
    CRYPTO_EncryptString(pOldPassword, m_pDefaultKey, m_szPassword, sizeof(m_szPassword));
    int nRet = DBM_Connect();
    if (nRet != 0)
    {
        return -1;
    }
    DBM_QueryUser(QueryUserResult, this);
    // ��֤ʧ��
    if (m_nVerify != 0)
    {
        DBM_Disconnect();
        return -1;
    }
    // �޸�����
    CRYPTO_EncryptString(pPassword, m_pDefaultKey, m_szPassword, sizeof(m_szPassword));
    nRet = DBM_ModifyUserPassword(m_szUser, m_szPassword);
    DBM_Disconnect();
    return nRet;
}
// ��ѯ�û�����ص�����
int CVerifyManager::QueryUserResult(int nUserID, const char *pUserName, const char *pPassword, void *pUser)
{
    if (NULL == pUser)
    {
        return 0;
    }
    CVerifyManager *pThis = static_cast<CVerifyManager*>(pUser);
    return pThis->VerifyUserResult(pUserName, pPassword);
}
// ��֤��ѯ�����û�
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
