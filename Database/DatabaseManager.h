/**
*   @file       DatabaseManager.h
*   @version    V1.0
*   @date       2020-10-17
*   @author     yangjiang
*   @brief      数据库管理类
*/
#pragma once
#include <mutex>
#include "DatabaseApi.h"
#include "Sqlite3/include/sqlite3.h"
#pragma comment(lib,"Sqlite3/lib/sqlite3.lib")
//---------------------------------------------------------
/**
*   @class  数据库访问相关处理管理类
*   @brief  数据库是sqllite
*/
class CDatabaseManager
{
private:
    // 禁用构造函数
    CDatabaseManager();

public:
    // 禁用构造传参赋值
    CDatabaseManager(const CDatabaseManager &) = delete;
    //< 禁用=赋值
    CDatabaseManager & operator = (const CDatabaseManager &) = delete;
    // 析构函数
    ~CDatabaseManager();
    // 本类用单件模式实现，通过此静态函数来获取类的唯一实例
    static CDatabaseManager & GetInstance();

    // 连接数据库
    int ConnectDB();
    // 断开数据库连接
    int DisconnectDB();
    // 添加便签
    int AddNote(const char *pPlatform, const char *pAccount, const char *pPassword);
    // 删除便签
    int DeleteNote(int nNoteID);
    // 更新便签
    int UpdateNote(int nNoteID, const char *pPlatform, const char *pAccount, const char *pPassword);
    // 更新平台
    int UpdatePlatform(int nNoteID, const char *pPlatform);
    // 更新账户
    int UpdateAccount(int nNoteID, const char *pAccount);
    // 更新密码
    int UpdatePassword(int nNoteID, const char *pPassword);
    // 查询便签
    int QueryNote(QueryNoteResultCallback fun, void *pUser);

    // 更新用户
    int UpdateKey(const char *pNewKey, const char *pOldKey);
    // 查询用户
    int QueryKey(QueryKeyResultCallback fun, void *pUser);

private:
    std::mutex m_mutexDBMgr;        // 互斥锁
    sqlite3 *m_pDB;
};

