#include "pch.h"
#include <string>
#include "DatabaseManager.h"

// �����û���ǩ��
constexpr auto SQL_CREATE_TABLE_USER_NOTE = "CREATE TABLE IF NOT EXISTS user_note(id INTEGER NOT NULL PRIMARY KEY,platform VARCHAR(64) NOT NULL,user_account VARCHAR(128) NOT NULL,user_password VARCHAR(128) NOT NULL)";
// ������Կ��
constexpr auto SQL_CREATE_TABLE_SYS_KEY = "CREATE TABLE IF NOT EXISTS sys_key(id INTEGER NOT NULL PRIMARY KEY,user_key VARCHAR(128) NOT NULL)";
// ����Ĭ����Կ
constexpr auto SQL_CREATE_DEFAULT_SYS_KEY = "INSERT INTO sys_key values(NULL,'DEC2B1AA5B7F1D33D7CE4E3A30D15BF7FFB3D65540ED719ABF52F76493E09BB4')";
/**
*   ��ȡ�����·��
*   @param[in]  pModuleName     ģ������
*   @param[out] pModulePath     ģ��·��(�������ַ�'\')
*   @param[in]  nModulePathLen  ģ��·����󳤶�
*   @return =0  success
*   @return =-1 fail
*/
int Utils_GetModulePath(const char *pModuleName, char *pModulePath, int nModulePathLen)
{
    // ������
    if ((NULL == pModuleName) || (NULL == pModulePath) || (nModulePathLen <= 0))
    {
        printf("Utils_GetModulePath parameter error.\n");
        return -1;
    }

    char szDllPath[512];    // ��·��
    memset(szDllPath, 0, sizeof(szDllPath));

#ifdef _WIN32 // windowsִ�����´���
    int nRet = 0;
    // ���ǵ�IE����ocxʹ�õײ�⣬���Ի�ȡ�����Ŀ¼��Ϊʹ��Ŀ¼
    // ȡ��dll���ڵ�Ŀ¼
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
    // �Ӻ���ǰ���һ������'\'�ַ��ĵط�
    char *pTemp = strrchr(szDllPath, '\\');
    if (NULL == pTemp)
    {
        printf("Utils_GetModulePath search charactor error.\n");
        return -1;
    }
    // ȥ���ļ���������·�������λ�ò������ַ�'\'
    *pTemp = '\0';
#else // linuxִ�����´���
    // ��[.]��Ϊ���������Ŀ¼
    memset(szDllPath, 0, sizeof(szDllPath));
    strncpy(szDllPath, ".", sizeof(szDllPath) - 1);
#endif
    strncpy(pModulePath, szDllPath, nModulePathLen - 1);
    return 0;
}

// ���캯��
CDatabaseManager::CDatabaseManager()
    :m_pDB(NULL)
{
}
// ��������
CDatabaseManager::~CDatabaseManager()
{
    DisconnectDB();
}
// �����õ���ģʽʵ�֣�ͨ���˾�̬��������ȡ���Ψһʵ��
CDatabaseManager & CDatabaseManager::GetInstance()
{
    static CDatabaseManager sInstance;  // �ֲ���̬�����������˳�ʱ�ɲ���ϵͳ�ͷ�
    return sInstance;
}
// �������ݿ�
int CDatabaseManager::ConnectDB()
{
    try
    {
        // ȡ��ģ��·��
        char szModulePath[256] = { 0 }; // ģ��·��
        int nRet = Utils_GetModulePath("DatabaseApi.dll", szModulePath, sizeof(szModulePath));
        if (nRet != 0)
        {
            printf("%s Utils_GetModulePath failed", __FUNCTION__);
            return -1;
        }
        // ������ݿ��ļ�·��
        std::string strDBFile = std::string(szModulePath) + std::string("\\user_note.db");
        std::lock_guard<std::mutex> guardDBMgr(m_mutexDBMgr);
        // �����ʹ����ݿ�(���ݿ���ڴ򿪣����������򴴽��ٴ�)
        nRet = sqlite3_open(strDBFile.c_str(), &m_pDB);
        if (nRet != SQLITE_OK)
        {
            printf("%s sqlite3_open error.", __FUNCTION__);
            return -1;
        }
        // ������������ݱ��������򲻴���
        // ������ǩ��
        char *pErrMsg = NULL;
        nRet = sqlite3_exec(m_pDB, SQL_CREATE_TABLE_USER_NOTE, 0, 0, &pErrMsg);
        if (nRet != SQLITE_OK)
        {
            printf("%s create table user_note error. ret:%d err:%s", __FUNCTION__, nRet, pErrMsg);
            sqlite3_free(pErrMsg);
            sqlite3_close(m_pDB);
            return -1;
        }
        // ������Կ��
        nRet = sqlite3_exec(m_pDB, SQL_CREATE_TABLE_SYS_KEY, 0, 0, &pErrMsg);
        if (nRet != SQLITE_OK)
        {
            printf("%s create table sys_key error. ret:%d err:%s", __FUNCTION__, nRet, pErrMsg);
            sqlite3_free(pErrMsg);
            sqlite3_close(m_pDB);
            return -1;
        }
        // ��ѯ��Կ���Ƿ�����Կ
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
        // ��Կ��û����Կ������Ĭ����Կ
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
// �Ͽ����ݿ�����
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
// ��ӱ�ǩ
int CDatabaseManager::AddNote(const char *pPlatform, const char *pAccount, const char *pPassword)
{
    try
    {
        if (NULL == pPlatform || NULL == pAccount || NULL == pPassword)
        {
            printf("%s param error.", __FUNCTION__);
            return -1;
        }
        // ���sql���
        std::string strSql = "INSERT INTO user_note values(NULL,'";
        strSql += pPlatform;
        strSql += "','";
        strSql += pAccount;
        strSql += "','";
        strSql += pPassword;
        strSql += "');";
        // ִ��sql���
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
// ɾ����ǩ
int CDatabaseManager::DeleteNote(int nNoteID)
{
    try
    {
        if (nNoteID <= 0)
        {
            return -1;
        }
        // ���sql���
        std::string strSql = "DELETE FROM user_note WHERE id=";
        strSql += std::to_string(nNoteID);
        char *pErrMsg = NULL;
        // ִ��sql���
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
// ���±�ǩ
int CDatabaseManager::UpdateNote(int nNoteID, const char *pPlatform, const char *pAccount, const char *pPassword)
{
    try
    {
        if (NULL == pPlatform || NULL == pAccount || NULL == pPassword || nNoteID <= 0)
        {
            printf("%s param error.", __FUNCTION__);
            return -1;
        }
        // ���sql���
        std::string strSql = "UPDATE user_note SET platform ='";
        strSql += pPlatform;
        strSql += "', user_account ='";
        strSql += pAccount;
        strSql += "', user_password ='";
        strSql += pPassword;
        strSql += "' WHERE id=";
        strSql += std::to_string(nNoteID);
        char *pErrMsg = NULL;
        // ִ��sql���
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
// ����ƽ̨
int CDatabaseManager::UpdatePlatform(int nNoteID, const char *pPlatform)
{
    try
    {
        if (NULL == pPlatform || nNoteID <= 0)
        {
            printf("%s param error.", __FUNCTION__);
            return -1;
        }
        // ���sql���
        std::string strSql = "UPDATE user_note SET platform ='";
        strSql += pPlatform;
        strSql += "' WHERE id=";
        strSql += std::to_string(nNoteID);
        char *pErrMsg = NULL;
        // ִ��sql���
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
// �����˻�
int CDatabaseManager::UpdateAccount(int nNoteID, const char *pAccount)
{
    try
    {
        if (NULL == pAccount || nNoteID <= 0)
        {
            printf("%s param error.", __FUNCTION__);
            return -1;
        }
        // ���sql���
        std::string strSql = "UPDATE user_note SET user_account ='";
        strSql += pAccount;
        strSql += "' WHERE id=";
        strSql += std::to_string(nNoteID);
        char *pErrMsg = NULL;
        // ִ��sql���
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
// ��������
int CDatabaseManager::UpdatePassword(int nNoteID, const char *pPassword)
{
    try
    {
        if (NULL == pPassword || nNoteID <= 0)
        {
            printf("%s param error.", __FUNCTION__);
            return -1;
        }
        // ���sql���
        std::string strSql = "UPDATE user_note SET user_password ='";
        strSql += pPassword;
        strSql += "' WHERE id=";
        strSql += std::to_string(nNoteID);
        char *pErrMsg = NULL;
        // ִ��sql���
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
// ��ѯ��ǩ
int CDatabaseManager::QueryNote(QueryNoteResultCallback fun, void *pUser)
{
    try
    {
        if (NULL == fun)
        {
            return -1;
        }
        // ���sql���
        std::string strSql = "SELECT id,platform,user_account,user_password FROM user_note";
        sqlite3_stmt *pStmtNote = NULL;
        // ִ��sql���
        std::lock_guard<std::mutex> guardDBMgr(m_mutexDBMgr);
        int nRet = sqlite3_prepare(m_pDB, strSql.c_str(), -1, &pStmtNote, 0);
        if (nRet != SQLITE_OK)
        {
            printf("%s select error. ret:%d sql:%s.", __FUNCTION__, nRet, strSql.c_str());
            return -1;
        }
        // ��ȡ��ѯ���
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

// ������Կ
int CDatabaseManager::UpdateKey(const char *pNewKey, const char *pOldKey)
{
    try
    {
        if (NULL == pNewKey || NULL == pOldKey)
        {
            printf("%s param error.", __FUNCTION__);
            return -1;
        }
        // ���sql���
        std::string strSql = "UPDATE sys_key SET user_key ='";
        strSql += pNewKey;
        strSql += "' WHERE user_key='";
        strSql += pOldKey;
        strSql += "';";
        char *pErrMsg = NULL;
        // ִ��sql���
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
// ��ѯ��Կ
int CDatabaseManager::QueryKey(QueryKeyResultCallback fun, void *pUser)
{
    try
    {
        if (NULL == fun)
        {
            return -1;
        }
        // ���sql���
        std::string strSql = "SELECT id,user_key FROM sys_key";
        sqlite3_stmt *pStmtNote = NULL;
        // ִ��sql���
        std::lock_guard<std::mutex> guardDBMgr(m_mutexDBMgr);
        int nRet = sqlite3_prepare(m_pDB, strSql.c_str(), -1, &pStmtNote, 0);
        if (nRet != SQLITE_OK)
        {
            printf("%s select error. ret:%d sql:%s.", __FUNCTION__, nRet, strSql.c_str());
            return -1;
        }
        // ��ȡ��ѯ���
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
