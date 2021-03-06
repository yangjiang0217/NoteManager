#include "pch.h"
#include "VerifyManager.h"
#include "../Database/DatabaseApi.h"
#include "../Crypto/CryptoApi.h"
#ifdef _WIN32
#ifdef _DEBUG
#pragma comment(lib,"../lib/Debug/Database.lib")
#pragma comment(lib,"../lib/Debug/Crypto.lib")
#else
#pragma comment(lib,"../lib/Release/Database.lib")
#pragma comment(lib,"../lib/Release/Crypto.lib")
#endif
#endif

// ���캯��
CVerifyManager::CVerifyManager()
    : m_nVerify(-1)
    , m_szKey{ 0 }
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
// ��֤��Կ
int CVerifyManager::VerifyKey(const char *pKey)
{
    CRYPTO_EncryptString(pKey, pKey, m_szKey, sizeof(m_szKey));
    int nRet = DBM_Connect();
    if (nRet != 0)
    {
        return -1;
    }
    DBM_QueryKey(QueryKeyResult, this);
    DBM_Disconnect();
    return m_nVerify;
}
// ������Կ
int CVerifyManager::UpdateKey(const char *pNewKey, const char *pOldKey)
{
    // ��֤��Կ
    CRYPTO_EncryptString(pOldKey, pOldKey, m_szKey, sizeof(m_szKey));
    DBM_QueryKey(QueryKeyResult, this);
    // ��֤ʧ��
    if (m_nVerify != 0)
    {
        return -1;
    }
    // �޸�����
    char szNewKey[128] = { 0 };
    CRYPTO_EncryptString(pNewKey, pNewKey, szNewKey, sizeof(szNewKey));
    return DBM_UpdateKey(szNewKey, m_szKey);
}
// ��ѯ��Կ����ص�����
int CVerifyManager::QueryKeyResult(int nKeyID, const char *pKey, void *pUser)
{
    if (NULL == pUser)
    {
        return 0;
    }
    CVerifyManager *pThis = static_cast<CVerifyManager*>(pUser);
    return pThis->VerifyKeyResult(pKey);
}
// ��֤��ѯ������Կ
int CVerifyManager::VerifyKeyResult(const char *pKey)
{
    if (NULL == pKey)
    {
        return 0;
    }
    if (strcmp(m_szKey, pKey) == 0)
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
