/**
*   @file       DatabaseManager.h
*   @version    V1.0
*   @date       2020-10-17
*   @author     yangjiang
*   @brief      ���ݿ������
*/
#pragma once
#include <mutex>
#include "DatabaseApi.h"
#include "Sqlite3/include/sqlite3.h"
#pragma comment(lib,"Sqlite3/lib/sqlite3.lib")
//---------------------------------------------------------
/**
*   @class  ���ݿ������ش��������
*   @brief  ���ݿ���sqllite
*/
class CDatabaseManager
{
private:
    // ���ù��캯��
    CDatabaseManager();

public:
    // ���ù��촫�θ�ֵ
    CDatabaseManager(const CDatabaseManager &) = delete;
    //< ����=��ֵ
    CDatabaseManager & operator = (const CDatabaseManager &) = delete;
    // ��������
    ~CDatabaseManager();
    // �����õ���ģʽʵ�֣�ͨ���˾�̬��������ȡ���Ψһʵ��
    static CDatabaseManager & GetInstance();

    // �������ݿ�
    int ConnectDB();
    // �Ͽ����ݿ�����
    int DisconnectDB();
    // ��ӱ�ǩ
    int AddNote(const char *pPlatform, const char *pAccount, const char *pPassword);
    // ɾ����ǩ
    int DeleteNote(int nNoteID);
    // ���±�ǩ
    int UpdateNote(int nNoteID, const char *pPlatform, const char *pAccount, const char *pPassword);
    // ����ƽ̨
    int UpdatePlatform(int nNoteID, const char *pPlatform);
    // �����˻�
    int UpdateAccount(int nNoteID, const char *pAccount);
    // ��������
    int UpdatePassword(int nNoteID, const char *pPassword);
    // ��ѯ��ǩ
    int QueryNote(QueryNoteResultCallback fun, void *pUser);

    // �����û�
    int UpdateKey(const char *pNewKey, const char *pOldKey);
    // ��ѯ�û�
    int QueryKey(QueryKeyResultCallback fun, void *pUser);

private:
    std::mutex m_mutexDBMgr;        // ������
    sqlite3 *m_pDB;
};

