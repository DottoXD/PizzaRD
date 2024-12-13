#include "pch.h"
#include <Windows.h>
#include <WinINet.h>
#include <iostream>
#include <locale>

#include <MinHook.h>
#pragma comment(lib, "libMinHook.x64.lib")

const LPCWSTR REDIRECT_IP = L"Your IP";
const INTERNET_PORT REDIRECT_PORT =  80;
const bool HTTPS = false;
const bool CONSOLE = false;

typedef HINTERNET(WINAPI* PFN_HttpOpenRequest)(HINTERNET, LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR&, DWORD, DWORD_PTR);
typedef HINTERNET(WINAPI* PFN_InternetConnect)(HINTERNET, LPCWSTR, INTERNET_PORT, LPCWSTR, LPCWSTR, DWORD, DWORD, DWORD_PTR);

PFN_HttpOpenRequest OpenRequestHttp = nullptr;
PFN_InternetConnect InternetConnectHttp = nullptr;

HINTERNET WINAPI FixedOpenRequestHttp(HINTERNET hConnect, LPCWSTR lpszVerb, LPCWSTR lpszObjectName, LPCWSTR lpszVersion, LPCWSTR lpszReferrer, LPCWSTR lpszAcceptTypes[], DWORD dwFlags, DWORD_PTR dwContext) {
    return OpenRequestHttp(hConnect, lpszVerb, lpszObjectName, lpszVersion, lpszReferrer, *lpszAcceptTypes, (HTTPS ? INTERNET_FLAG_SECURE | INTERNET_FLAG_DONT_CACHE : INTERNET_FLAG_DONT_CACHE), dwContext);
}

HINTERNET WINAPI FixedInternetConnectHttp(HINTERNET hInternet, LPCWSTR lpszServerName, INTERNET_PORT nServerPort, LPCWSTR lpszUserName, LPCWSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext) {
    return InternetConnectHttp(hInternet, REDIRECT_IP, REDIRECT_PORT, lpszUserName, lpszPassword, dwService, dwFlags, dwContext);
}

MH_STATUS HookInternetOpen() {
    if (CONSOLE) {
        AllocConsole();
        FILE* fp;
        freopen_s(&fp, "CONOUT$", "w+", stdout);
    }

    MH_STATUS status = MH_Initialize();
    if (status != MH_OK) std::cout << "MinHook fail!\n";
    else {
        std::cout << "PizzaRedirect started succesfully!\n";

        MH_CreateHookApi(L"wininet.dll", "HttpOpenRequestW", &FixedOpenRequestHttp, reinterpret_cast<void**>(&OpenRequestHttp));
        MH_CreateHookApi(L"wininet.dll", "InternetConnectW", &FixedInternetConnectHttp, reinterpret_cast<void**>(&InternetConnectHttp));

        return MH_EnableHook(MH_ALL_HOOKS);
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            HookInternetOpen();
            break;
        case DLL_PROCESS_DETACH:
            MH_Uninitialize();
            break;
    }

    return TRUE;
}
