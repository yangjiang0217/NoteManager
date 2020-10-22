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

    // ��֤�û�
    int VerifyUser(const char *pUserName, const char *pPassword);
    // �޸�����
    int ModifyPassword(const char *pUserName, const char *pOldPassword, const char *pPassword);

private:
    // ��ѯ�û�����ص�����
    static int QueryUserResult(int nUserID, const char *pUserName, const char *pPassword, void *pUser);
    // ��֤��ѯ�����û�
    int VerifyUserResult(const char *pUserName, const char *pPassword);

    int m_nVerify;
    const char *m_pDefaultKey = "0123456789abcdefg";
    char m_szUser[64];
    char m_szPassword[128];
};

