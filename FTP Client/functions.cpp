#include <windows.h>
#include <wininet.h>
#include "functions.h"


/*
 //Error-checking code

char Buffer[10];
DWORD err = GetLastError();
itoa(err, Buffer, 10);
SetWindowTextA(hDisplay, Buffer);

*/


HWND hFilePath, hDisplay;
HINTERNET hOpen, hConnect;
BOOL hGetFile, hPutFile;

void CloseHandles()
{
    InternetCloseHandle(hOpen);
    InternetCloseHandle(hConnect);
}

void InitializeUI(HWND hMainWindow)
{
    CreateWindowEx(0, "Static", "Enter File Path:", WS_CHILD | WS_VISIBLE, 10, 22, 100, 20, hMainWindow,
                   NULL, NULL, NULL);
    hFilePath = CreateWindowEx(0, "Edit", NULL, WS_CHILD | WS_VISIBLE, 120, 20, 703, 20, hMainWindow, NULL, NULL, NULL);
    CreateWindowEx(0, "Button", "Upload File", WS_CHILD | WS_VISIBLE, 850, 15, 98, 30, hMainWindow,
                   (HMENU) BTN_UPLOAD, NULL, NULL);
    CreateWindowEx(0, "Button", "Download File", WS_CHILD | WS_VISIBLE, 850, 55, 98, 30, hMainWindow, (HMENU) BTN_DOWNLOAD,
                   NULL, NULL);
    hDisplay = CreateWindowEx(0, "Edit", NULL, WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_VSCROLL | WS_BORDER, 10, 100, 983, 400, hMainWindow,
                   NULL, NULL, NULL);
}

void UploadFile()
{
    int nLength = GetWindowTextLengthA(hFilePath);
    char *pBuffer = new char[nLength + 1];
    GetWindowTextA(hFilePath, pBuffer, nLength);

    if (nLength == 0)
    {
        MessageBox(NULL, "Enter a valid file path!", "Error", MB_OK | MB_ICONEXCLAMATION);
        return;
    }

    if (!InternetGetConnectedState(NULL, 0))
    {
        MessageBox(NULL, "No Internet", "Error", MB_OK | MB_ICONERROR);
        return;
    }


    hOpen = InternetOpen("FtpPractice", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

    if (!hOpen)
    {
        MessageBox(NULL, "Failed to create Internet Handle!", "Error", MB_OK | MB_ICONERROR);
        InternetCloseHandle(hOpen);
        return;
    }

    hConnect = InternetConnectA(hOpen, "192.168.0.10", 2121, NULL, NULL, INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);

    if (!hConnect)
    {
        MessageBox(NULL, "Failed to connect to Server!", "Error", MB_OK | MB_ICONERROR);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hOpen);
        return;
    }

    hPutFile = FtpPutFile(hConnect, "Upload.txt", "Upload.txt", FTP_TRANSFER_TYPE_BINARY, 0);

    if (hPutFile)
    {
        MessageBox(NULL, "File Uploaded!", "Confirmation", MB_OK | MB_ICONINFORMATION);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hOpen);
        return;
    }
    else
    {
        MessageBoxA(NULL, "Failed to access File!", "Error", MB_OK | MB_ICONERROR);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hOpen);
        return;
    }
}

void DownloadFile()
{
    int nLength = GetWindowTextLengthA(hFilePath);
    char *pBuffer = new char[nLength + 1];
    GetWindowTextA(hFilePath, pBuffer, nLength);

    if (nLength == 0)
    {
        MessageBox(NULL, "Enter a valid file path!", "Error", MB_OK | MB_ICONEXCLAMATION);
        return;
    }

    if (!InternetGetConnectedState(NULL, 0))
    {
        MessageBox(NULL, "No Internet", "Error", MB_OK | MB_ICONERROR);
        return;
    }


    hOpen = InternetOpen("FtpPractice", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

    if (!hOpen)
    {
        MessageBox(NULL, "Failed to create Internet Handle!", "Error", MB_OK | MB_ICONERROR);
        InternetCloseHandle(hOpen);
        return;
    }

    hConnect = InternetConnectA(hOpen, "192.168.0.10", 2121, NULL, NULL, INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);

    if (!hConnect)
    {
        MessageBox(NULL, "Failed to connect to Server!", "Error", MB_OK | MB_ICONERROR);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hOpen);
        return;
    }

    hGetFile = FtpGetFileA(hConnect, "Proxies.txt", "Proxies.txt", FALSE, FILE_ATTRIBUTE_NORMAL, FTP_TRANSFER_TYPE_BINARY, 0);

    if (hGetFile)
    {
        MessageBox(NULL, "File Downloaded!", "Confirmation", MB_OK | MB_ICONINFORMATION);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hOpen);
        return;
    }
    else
    {
        MessageBoxA(NULL, "Failed to access File!", "Error", MB_OK | MB_ICONERROR);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hOpen);
        return;
    }
}
