#include <windows.h>
#include <wininet.h>
#include "Functions.h"

/*
    char Buffer[10];
    DWORD err = GetLastError();            //Error-Checking Code
    itoa(err, Buffer, 10);
    SetWindowTextA(hDisplay, Buffer);

*/


HWND hDisplay, hEditBox;
HINTERNET hOpen, hConnect, hRequest;
BOOL hSend;

void InitializeUI(HWND hWnd)
{
    hDisplay = CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE |WS_VSCROLL,
                            10, 80, 983, 410, hWnd, NULL, NULL, NULL);
    CreateWindow("Static", "Enter URL:", WS_VISIBLE | WS_CHILD, 10, 20, 80, 20, hWnd, NULL, NULL, NULL);
    hEditBox = CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD, 100, 20, 703, 20, hWnd, NULL, NULL, NULL);
    CreateWindow("Button", "Go", WS_VISIBLE | WS_CHILD, 850, 15, 98, 30, hWnd, (HMENU)GO_BUTTON, NULL, NULL);
}

void GoButton()
{
    if (!InternetGetConnectedState(NULL, 0))
    {
        MessageBoxW(NULL, L"No Internet!", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    hOpen = InternetOpenW(L"Practice1", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

    if (!hOpen)
    {
        MessageBoxW(NULL, L"Failed to Create Internet Session!", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    /*
                                        For HTTP

    hConnect = InternetConnect(hOpen, "www.example.com", INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP,
                               NULL, 0);
    */

    //                                  For HTTPS
    hConnect = InternetConnect(hOpen, "www.google.com", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP,
                               NULL, 0);


    if (!hConnect)
    {
        MessageBoxW(NULL, L"Failed to Connect to Server!", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    /*                              For HTTP

    hRequest = HttpOpenRequestA(hConnect, NULL,"www.example.com", NULL, NULL, NULL, INTERNET_FLAG_SECURE, 0);

    */

    //                              For HTTPS
    hRequest = HttpOpenRequestA(hConnect, NULL, "www.google.com", NULL, NULL, NULL, INTERNET_FLAG_SECURE, 0);

    if (!hRequest)
    {
        MessageBoxW(NULL, L"Failed to Create HTTP Request!", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    hSend = HttpSendRequest(hRequest, NULL, 0, NULL, 0);

    if (!hSend)
    {
        MessageBoxW(NULL, L"Failed Send HTTP Request!", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    char pResourceSize[] = "Content-Length";
    DWORD nBufferSize = sizeof(pResourceSize);
    if (!HttpQueryInfoA(hRequest, HTTP_QUERY_CUSTOM, &pResourceSize, &nBufferSize, NULL))
    {
        char Buffer[10];
        DWORD err = GetLastError();
        if (err != 12150)
        {
            MessageBox(NULL, "HTTP Header not Found!", "Fail", MB_OK | MB_ICONEXCLAMATION);
            return;
        }
        MessageBox(NULL, "HTTP Query Failed!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    char *pMainBuffer = new char[*pResourceSize - '0']; // char to int
    *pMainBuffer = 0;
    char *pTempBuffer = new char[*pResourceSize - '0']; //char to int
    DWORD dwBytesRead;

    /*
                    //If no header is available
    char *pMainBuffer = new char[1000000];
    *pMainBuffer = 0;
    char *pTempBuffer = new char[1000000]; //char to int
    DWORD dwBytesRead;

    */


    while(InternetReadFile(hRequest, pTempBuffer, *pResourceSize, &dwBytesRead) && (dwBytesRead) != 0)
    {
        strncat(pMainBuffer, pTempBuffer, dwBytesRead);
    }
    SetWindowTextA(hDisplay, (LPCSTR)pMainBuffer);

}
