// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 DATABASE_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// DATABASE_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef DATABASE_EXPORTS
#define DATABASE_API __declspec(dllexport)
#else
#define DATABASE_API __declspec(dllimport)
#endif

// 查询便签结果回调函数，返回!0表示不继续接收
typedef int QueryNoteResultCallback(int nNoteID, const char *pPlatform, const char *pAccount, const char *pPassword, void *pUser);
// 查询用户结果回调函数，返回!0表示不继续接收
typedef int QueryUserResultCallback(int nUserID, const char *pUserName, const char *pPassword, void *pUser);

// 连接数据库
DATABASE_API int DBM_Connect();
// 断开数据库连接
DATABASE_API int DBM_Disconnect();
// 添加便签
DATABASE_API int DBM_AddNote(const char *pPlatform, const char *pAccount, const char *pPassword);
// 删除便签
DATABASE_API int DBM_DeleteNote(int nNoteID);
// 更新便签
DATABASE_API int DBM_UpdateNote(int nNoteID, const char *pPlatform, const char *pAccount, const char *pPassword);
// 更新平台
DATABASE_API int DBM_UpdatePlatform(int nNoteID, const char *pPlatform);
// 更新账户
DATABASE_API int DBM_UpdateAccount(int nNoteID, const char *pAccount);
// 更新密码
DATABASE_API int DBM_UpdatePassword(int nNoteID, const char *pPassword);
// 查询便签
DATABASE_API int DBM_QueryNote(QueryNoteResultCallback fun, void *pUser);
// 添加用户
DATABASE_API int DBM_AddUser(const char *pUserName, const char *pPassword);
// 删除用户
DATABASE_API int DBM_DeleteUser(int nUserID);
// 更新用户
DATABASE_API int DBM_UpdateUser(int nUserID, const char *pUserName, const char *pPassword);
// 修改用户密码
DATABASE_API int DBM_ModifyUserPassword(const char *pUserName, const char *pPassword);
// 查询用户
DATABASE_API int DBM_QueryUser(QueryUserResultCallback fun, void *pUser);
