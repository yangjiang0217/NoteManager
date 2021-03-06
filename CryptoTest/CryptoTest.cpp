#include <iostream>
#include <string>
#include "../Crypto/CryptoApi.h"
#ifdef _WIN32
#ifdef _DEBUG
#pragma comment(lib,"../lib/Debug/Crypto.lib")
#else
#pragma comment(lib,"../lib/Release/Crypto.lib")
#endif
#endif

int main()
{
    std::cout << "Please input text:" << std::endl;
    std::string strText = "";
    std::cin >> strText;

    std::cout << "Please input key:" << std::endl;
    std::string strKey = "";
    std::cin >> strKey;

    char szEncrypt[128] = { 0 };
    char szDecrypt[128] = { 0 };
    CRYPTO_EncryptString(strText.c_str(), strKey.c_str(), szEncrypt, sizeof(szEncrypt));
    std::cout << szEncrypt << "\n";

    CRYPTO_DecryptString(szEncrypt, strKey.c_str(), szDecrypt, sizeof(szDecrypt));
    std::cout << szDecrypt << "\n";
    system("pause");
    return 0;
}
