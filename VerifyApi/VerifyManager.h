#pragma once
class CVerifyManager
{
private:
    // 禁用构造函数
    CVerifyManager();

public:
    // 禁用构造传参赋值
    CVerifyManager(const CVerifyManager &) = delete;
    //< 禁用=赋值
    CVerifyManager & operator = (const CVerifyManager &) = delete;
    // 析构函数
    ~CVerifyManager();
    // 本类用单件模式实现，通过此静态函数来获取类的唯一实例
    static CVerifyManager & GetInstance();

    // 验证用户
    int VerifyUser(const char *pUserName, const char *pPassword);
    // 修改密码
    int ModifyPassword(const char *pUserName, const char *pOldPassword, const char *pPassword);

private:
    // 查询用户结果回调函数
    static int QueryUserResult(int nUserID, const char *pUserName, const char *pPassword, void *pUser);
    // 验证查询返回用户
    int VerifyUserResult(const char *pUserName, const char *pPassword);

    int m_nVerify;
    const char *m_pDefaultKey = "0123456789abcdefg";
    char m_szUser[64];
    char m_szPassword[128];
};

