// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 CRYPTOAPI_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// CRYPTOAPI_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef CRYPTOAPI_EXPORTS
#define CRYPTOAPI_API __declspec(dllexport)
#else
#define CRYPTOAPI_API __declspec(dllimport)
#endif

// 加密
CRYPTOAPI_API int CRYPTO_EncryptString(const char *pText, const char *pKey, char *pOut, unsigned int nOutLen);
// 解密
CRYPTOAPI_API int CRYPTO_DecryptString(const char *pText, const char *pKey, char *pOut, unsigned int nOutLen);
