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

    // 验证密钥
    int VerifyKey(const char *pKey);
    // 更新密钥
    int UpdateKey(const char *pNewKey, const char *pOldKey);

private:
    // 查询密钥结果回调函数
    static int QueryKeyResult(int nKeyID, const char *pKey, void *pUser);
    // 验证查询返回密钥
    int VerifyKeyResult(const char *pKey);

    int m_nVerify;
    char m_szKey[128];
};

