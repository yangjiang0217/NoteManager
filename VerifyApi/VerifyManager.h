#pragma once
class CVerifyManager
{
private:
    // ���ù��캯��
    CVerifyManager();

public:
    // ���ù��촫�θ�ֵ
    CVerifyManager(const CVerifyManager &) = delete;
    //< ����=��ֵ
    CVerifyManager & operator = (const CVerifyManager &) = delete;
    // ��������
    ~CVerifyManager();
    // �����õ���ģʽʵ�֣�ͨ���˾�̬��������ȡ���Ψһʵ��
    static CVerifyManager & GetInstance();

    // ��֤��Կ
    int VerifyKey(const char *pKey);
    // ������Կ
    int UpdateKey(const char *pNewKey, const char *pOldKey);

private:
    // ��ѯ��Կ����ص�����
    static int QueryKeyResult(int nKeyID, const char *pKey, void *pUser);
    // ��֤��ѯ������Կ
    int VerifyKeyResult(const char *pKey);

    int m_nVerify;
    char m_szKey[128];
};

