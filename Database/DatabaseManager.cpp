#include "pch.h"
#include <string>
#include "DatabaseManager.h"

// 创建用户便签表
constexpr auto SQL_CREATE_TABLE_USER_NOTE = "CREATE TABLE IF NOT EXISTS user_note(id INTEGER NOT NULL PRIMARY KEY,platform VARCHAR(64) NOT NULL,user_account VARCHAR(128) NOT NULL,user_password VARCHAR(128) NOT NULL)";
// 创建密钥表
constexpr auto SQL_CREATE_TABLE_SYS_KEY = "CREATE TABLE IF NOT EXISTS sys_key(id INTEGER NOT NULL PRIMARY KEY,user_key VARCHAR(128) NOT NULL)";
// 创建默认密钥
constexpr auto SQL_CREATE_DEFAULT_SYS_KEY = "INSERT INTO sys_key values(NULL,'DEC2B1AA5B7F1D33D7CE4E3A30D15BF7FFB3D65540ED719ABF52F76493E09BB4')";
/**
*   获取程序库路径
*   @param[in]  pModuleName     模块名称
*   @param[out] pModulePath     模块路径(不包括字符'\')
*   @param[in]  nModulePathLen  模块路径最大长度
*   @return =0  success
*   @return =-1 fail
*/
int Utils_GetModulePath(const char *pModuleName, char *pModulePath, int nModulePathLen)
{
    // 检查参数
    if ((NULL == pModuleName) || (NULL == pModulePath) || (nModulePathLen <= 0))
    {
        printf("Utils_GetModulePath parameter error.\n");
        return -1;
    }

    char szDllPath[512];    // 库路径
    memset(szDllPath, 0, sizeof(szDllPath));

#ifdef _WIN32 // windows执行如下处理
    int nRet = 0;
    // 考虑到IE调用ocx使用底层库，所以获取程序库目录作为使用目录
    // 取得dll所在的目录
    HANDLE hModuleHandle = NULL;
    hModuleHandle = ::GetModuleHandleA(pModuleName);
    if (NULL == hModuleHandle)
    {
        printf("Utils_GetModulePath GetModuleHandleA error.\n");
        return -1;
    }
    nRet = ::GetModuleFileNameA((HMODULE)hModuleHandle, szDllPath, sizeof(szDllPath));
    if (nRet == 0)
    {
        printf("Utils_GetModulePath GetModuleFileNameA error.\n");
        return -1;
    }
    // 从后往前查第一个出现'\'字符的地方
    char *pTemp = strrchr(szDllPath, '\\');
    if (NULL == pTemp)
    {
        printf("Utils_GetModulePath search charactor error.\n");
        return -1;
    }
    // 去掉文件名，并且路径中最后位置不包括字符'\'
    *pTemp = '\0';
#else // linux执行如下处理
    // 以[.]做为程序库所在目录
    memset(szDllPath, 0, sizeof(szDllPath));
    strncpy(szDllPath, ".", sizeof(szDllPath) - 1);
#endif
    strncpy(pModulePath, szDllPath, nModulePathLen - 1);
    return 0;
}

// 构造函数
CDatabaseManager::CDatabaseManager()
    :m_pDB(NULL)
{
}
// 析构函数
CDatabaseManager::~CDatabaseManager()
{
    DisconnectDB();
}
// 本类用单件模式实现，通过此静态函数来获取类的唯一实例
CDatabaseManager & CDatabaseManager::GetInstance()
{
    static CDatabaseManager sInstance;  // 局部静态变量，程序退出时由操作系统释放
    return sInstance;
}
// 连接数据库
int CDatabaseManager::ConnectDB()
{
    try
    {
        // 取得模块路径
        char szModulePath[256] = { 0 }; // 模块路径
        int nRet = Utils_GetModulePath("Database.dll", szModulePath, sizeof(szModulePath));
        if (nRet != 0)
        {
            printf("%s Utils_GetModulePath failed", __FUNCTION__);
            return -1;
        }
        // 组合数据库文件路径
        std::string strDBFile = std::string(szModulePath) + std::string("\\user_note.db");
        std::lock_guard<std::mutex> guardDBMgr(m_mutexDBMgr);
        // 创建和打开数据库(数据库存在打开，若不存在则创建再打开)
        nRet = sqlite3_open(strDBFile.c_str(), &m_pDB);
        if (nRet != SQLITE_OK)
        {
            printf("%s sqlite3_open error.", __FUNCTION__);
            return -1;
        }
        // 创建不存的数据表，若存在则不创建
        // 创建便签表
        char *pErrMsg = NULL;
        nRet = sqlite3_exec(m_pDB, SQL_CREATE_TABLE_USER_NOTE, 0, 0, &pErrMsg);
        if (nRet != SQLITE_OK)
        {
            printf("%s create table user_note error. ret:%d err:%s", __FUNCTION__, nRet, pErrMsg);
            sqlite3_free(pErrMsg);
            sqlite3_close(m_pDB);
            return -1;
        }
        // 创建密钥表
        nRet = sqlite3_exec(m_pDB, SQL_CREATE_TABLE_SYS_KEY, 0, 0, &pErrMsg);
        if (nRet != SQLITE_OK)
        {
            printf("%s create table sys_key error. ret:%d err:%s", __FUNCTION__, nRet, pErrMsg);
            sqlite3_free(pErrMsg);
            sqlite3_close(m_pDB);
            return -1;
        }
        // 查询密钥表是否有密钥
        std::string strSql = "SELECT count(*) FROM sys_key";
        sqlite3_stmt *pStmt = NULL;
        nRet = sqlite3_prepare(m_pDB, strSql.c_str(), -1, &pStmt, 0);
        if (nRet != SQLITE_OK)
        {
            printf("%s select error. ret:%d sql:%s.", __FUNCTION__, nRet, strSql.c_str());
            return -1;
        }
        int nRc = sqlite3_step(pStmt);
        int nKeyCount = 0;
        while (nRc == SQLITE_ROW)
        {
            nKeyCount = sqlite3_column_int(pStmt, 0);
            break;
        }
        sqlite3_finalize(pStmt);
        if (nKeyCount > 0)
        {
            return 0;
        }
        // 密钥表没有密钥，插入默认密钥
        nRet = sqlite3_exec(m_pDB, SQL_CREATE_DEFAULT_SYS_KEY, 0, 0, &pErrMsg);
        if (nRet != SQLITE_OK)
        {
            printf("%s error. ret:%d err:%s.", __FUNCTION__, nRet, pErrMsg);
            sqlite3_free(pErrMsg);
        }
    }
    catch (...)
    {
        printf("%s exception.", __FUNCTION__);
        return -1;
    }
    return 0;
}
// 断开数据库连接
int CDatabaseManager::DisconnectDB()
{
    try
    {
        std::lock_guard<std::mutex> guardDBMgr(m_mutexDBMgr);
        if (NULL != m_pDB)
        {
            int nRet = sqlite3_close(m_pDB);
            if (nRet != SQLITE_OK)
            {
                printf("%s fail ret:%d.", __FUNCTION__, nRet);
                return -1;
            }
            m_pDB = NULL;
        }
    }
    catch (...)
    {
        printf("%s exception.", __FUNCTION__);
        return -1;
    }
    return 0;
}
// 添加便签
int CDatabaseManager::AddNote(const char *pPlatform, const char *pAccount, const char *pPassword)
{
    try
    {
        if (NULL == pPlatform || NULL == pAccount || NULL == pPassword)
        {
            printf("%s param error.", __FUNCTION__);
            return -1;
        }
        // 组合sql语句
        std::string strSql = "INSERT INTO user_note values(NULL,'";
        strSql += pPlatform;
        strSql += "','";
        strSql += pAccount;
        strSql += "','";
        strSql += pPassword;
        strSql += "');";
        // 执行sql语句
        std::lock_guard<std::mutex> guardDBMgr(m_mutexDBMgr);
        char *pErrMsg = NULL;
        int nRet = sqlite3_exec(m_pDB, strSql.c_str(), 0, 0, &pErrMsg);
        if (nRet != SQLITE_OK)
        {
            printf("%s error. ret:%d err:%s.", __FUNCTION__, nRet, pErrMsg);
            sqlite3_free(pErrMsg);
            return -1;
        }
    }
    catch (...)
    {
        printf("%s exception.", __FUNCTION__);
        return -1;
    }
    return 0;
}
// 删除便签
int CDatabaseManager::DeleteNote(int nNoteID)
{
    try
    {
        if (nNoteID <= 0)
        {
            return -1;
        }
        // 组合sql语句
        std::string strSql = "DELETE FROM user_note WHERE id=";
        strSql += std::to_string(nNoteID);
        char *pErrMsg = NULL;
        // 执行sql语句
        std::lock_guard<std::mutex> guardDBMgr(m_mutexDBMgr);
        int nRet = sqlite3_exec(m_pDB, strSql.c_str(), 0, 0, &pErrMsg);
        if (nRet != SQLITE_OK)
        {
            printf("%s error. ret:%d err:%s.", __FUNCTION__, nRet, pErrMsg);
            sqlite3_free(pErrMsg);
            return -1;
        }
    }
    catch (...)
    {
        printf("%s exception.", __FUNCTION__);
        return -1;
    }
    return 0;
}
// 更新便签
int CDatabaseManager::UpdateNote(int nNoteID, const char *pPlatform, const char *pAccount, const char *pPassword)
{
    try
    {
        if (NULL == pPlatform || NULL == pAccount || NULL == pPassword || nNoteID <= 0)
        {
            printf("%s param error.", __FUNCTION__);
            return -1;
        }
        // 组合sql语句
        std::string strSql = "UPDATE user_note SET platform ='";
        strSql += pPlatform;
        strSql += "', user_account ='";
        strSql += pAccount;
        strSql += "', user_password ='";
        strSql += pPassword;
        strSql += "' WHERE id=";
        strSql += std::to_string(nNoteID);
        char *pErrMsg = NULL;
        // 执行sql语句
        std::lock_guard<std::mutex> guardDBMgr(m_mutexDBMgr);
        int nRet = sqlite3_exec(m_pDB, strSql.c_str(), 0, 0, &pErrMsg);
        if (nRet != SQLITE_OK)
        {
            printf("%s error. ret:%d err:%s", __FUNCTION__, nRet, pErrMsg);
            sqlite3_free(pErrMsg);
            return -1;
        }
    }
    catch (...)
    {
        printf("%s exception.", __FUNCTION__);
        return -1;
    }
    return 0;
}
// 更新平台
int CDatabaseManager::UpdatePlatform(int nNoteID, const char *pPlatform)
{
    try
    {
        if (NULL == pPlatform || nNoteID <= 0)
        {
            printf("%s param error.", __FUNCTION__);
            return -1;
        }
        // 组合sql语句
        std::string strSql = "UPDATE user_note SET platform ='";
        strSql += pPlatform;
        strSql += "' WHERE id=";
        strSql += std::to_string(nNoteID);
        char *pErrMsg = NULL;
        // 执行sql语句
        std::lock_guard<std::mutex> guardDBMgr(m_mutexDBMgr);
        int nRet = sqlite3_exec(m_pDB, strSql.c_str(), 0, 0, &pErrMsg);
        if (nRet != SQLITE_OK)
        {
            printf("%s error. ret:%d err:%s", __FUNCTION__, nRet, pErrMsg);
            sqlite3_free(pErrMsg);
            return -1;
        }
    }
    catch (...)
    {
        printf("%s exception.", __FUNCTION__);
        return -1;
    }
    return 0;
}
// 更新账户
int CDatabaseManager::UpdateAccount(int nNoteID, const char *pAccount)
{
    try
    {
        if (NULL == pAccount || nNoteID <= 0)
        {
            printf("%s param error.", __FUNCTION__);
            return -1;
        }
        // 组合sql语句
        std::string strSql = "UPDATE user_note SET user_account ='";
        strSql += pAccount;
        strSql += "' WHERE id=";
        strSql += std::to_string(nNoteID);
        char *pErrMsg = NULL;
        // 执行sql语句
        std::lock_guard<std::mutex> guardDBMgr(m_mutexDBMgr);
        int nRet = sqlite3_exec(m_pDB, strSql.c_str(), 0, 0, &pErrMsg);
        if (nRet != SQLITE_OK)
        {
            printf("%s error. ret:%d err:%s", __FUNCTION__, nRet, pErrMsg);
            sqlite3_free(pErrMsg);
            return -1;
        }
    }
    catch (...)
    {
        printf("%s exception.", __FUNCTION__);
        return -1;
    }
    return 0;
}
// 更新密码
int CDatabaseManager::UpdatePassword(int nNoteID, const char *pPassword)
{
    try
    {
        if (NULL == pPassword || nNoteID <= 0)
        {
            printf("%s param error.", __FUNCTION__);
            return -1;
        }
        // 组合sql语句
        std::string strSql = "UPDATE user_note SET user_password ='";
        strSql += pPassword;
        strSql += "' WHERE id=";
        strSql += std::to_string(nNoteID);
        char *pErrMsg = NULL;
        // 执行sql语句
        std::lock_guard<std::mutex> guardDBMgr(m_mutexDBMgr);
        int nRet = sqlite3_exec(m_pDB, strSql.c_str(), 0, 0, &pErrMsg);
        if (nRet != SQLITE_OK)
        {
            printf("%s error. ret:%d err:%s", __FUNCTION__, nRet, pErrMsg);
            sqlite3_free(pErrMsg);
            return -1;
        }
    }
    catch (...)
    {
        printf("%s exception.", __FUNCTION__);
        return -1;
    }
    return 0;
}
// 查询便签
int CDatabaseManager::QueryNote(QueryNoteResultCallback fun, void *pUser)
{
    try
    {
        if (NULL == fun)
        {
            return -1;
        }
        // 组合sql语句
        std::string strSql = "SELECT id,platform,user_account,user_password FROM user_note";
        sqlite3_stmt *pStmtNote = NULL;
        // 执行sql语句
        std::lock_guard<std::mutex> guardDBMgr(m_mutexDBMgr);
        int nRet = sqlite3_prepare(m_pDB, strSql.c_str(), -1, &pStmtNote, 0);
        if (nRet != SQLITE_OK)
        {
            printf("%s select error. ret:%d sql:%s.", __FUNCTION__, nRet, strSql.c_str());
            return -1;
        }
        // 获取查询结果
        int nRc = sqlite3_step(pStmtNote);
        int nCol = 0;
        int nId = 0;
        std::string strPlatform = "";
        std::string strUserAccount = "";
        std::string strUserPassword = "";
        while (nRc == SQLITE_ROW)
        {
            nCol = 0;
            nId = sqlite3_column_int(pStmtNote, nCol);
            nCol++;
            strPlatform = (char*)sqlite3_column_text(pStmtNote, nCol);
            nCol++;
            strUserAccount = (char*)sqlite3_column_text(pStmtNote, nCol);
            nCol++;
            strUserPassword = (char*)sqlite3_column_text(pStmtNote, nCol);
            nRet = fun(nId, strPlatform.c_str(), strUserAccount.c_str(), strUserPassword.c_str(), pUser);
            if (nRet != 0)
            {
                break;
            }
            nRc = sqlite3_step(pStmtNote);
        }
        sqlite3_finalize(pStmtNote);
    }
    catch (...)
    {
        printf("%s exception.", __FUNCTION__);
        return -1;
    }
    return 0;
}

// 更新密钥
int CDatabaseManager::UpdateKey(const char *pNewKey, const char *pOldKey)
{
    try
    {
        if (NULL == pNewKey || NULL == pOldKey)
        {
            printf("%s param error.", __FUNCTION__);
            return -1;
        }
        // 组合sql语句
        std::string strSql = "UPDATE sys_key SET user_key ='";
        strSql += pNewKey;
        strSql += "' WHERE user_key='";
        strSql += pOldKey;
        strSql += "';";
        char *pErrMsg = NULL;
        // 执行sql语句
        std::lock_guard<std::mutex> guardDBMgr(m_mutexDBMgr);
        int nRet = sqlite3_exec(m_pDB, strSql.c_str(), 0, 0, &pErrMsg);
        if (nRet != SQLITE_OK)
        {
            printf("%s error. ret:%d err:%s", __FUNCTION__, nRet, pErrMsg);
            sqlite3_free(pErrMsg);
            return -1;
        }
    }
    catch (...)
    {
        printf("%s exception.", __FUNCTION__);
        return -1;
    }
    return 0;
}
// 查询密钥
int CDatabaseManager::QueryKey(QueryKeyResultCallback fun, void *pUser)
{
    try
    {
        if (NULL == fun)
        {
            return -1;
        }
        // 组合sql语句
        std::string strSql = "SELECT id,user_key FROM sys_key";
        sqlite3_stmt *pStmtNote = NULL;
        // 执行sql语句
        std::lock_guard<std::mutex> guardDBMgr(m_mutexDBMgr);
        int nRet = sqlite3_prepare(m_pDB, strSql.c_str(), -1, &pStmtNote, 0);
        if (nRet != SQLITE_OK)
        {
            printf("%s select error. ret:%d sql:%s.", __FUNCTION__, nRet, strSql.c_str());
            return -1;
        }
        // 获取查询结果
        int nRc = sqlite3_step(pStmtNote);
        int nCol = 0;
        int nId = 0;
        std::string strUserKey = "";
        while (nRc == SQLITE_ROW)
        {
            nCol = 0;
            nId = sqlite3_column_int(pStmtNote, nCol);
            nCol++;
            strUserKey = (char*)sqlite3_column_text(pStmtNote, nCol);
            nRet = fun(nId, strUserKey.c_str(), pUser);
            if (nRet != 0)
            {
                break;
            }
            nRc = sqlite3_step(pStmtNote);
        }
        sqlite3_finalize(pStmtNote);
    }
    catch (...)
    {
        printf("%s exception.", __FUNCTION__);
        return -1;
    }
    return 0;
}
