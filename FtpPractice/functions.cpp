#include <windows.h>
#include <wininet.h>
#include "functions.h"

HWND hFilePath, hDisplay;
HINTERNET hOpen, hConnect, hFirstFile;
WIN32_FIND_DATA w32FindData = {};

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


    hConnect = InternetOpenUrlA(hOpen, "ftp://192.168.0.10:2221", NULL, 0, INTERNET_FLAG_PASSIVE, 0);

    if (!hConnect)
    {
        MessageBox(NULL, "Failed to Connect to server!", "Error", MB_OK | MB_ICONERROR);
        //MessageBox(NULL, "Connected to Server!", "Confirmation", MB_OK | MB_ICONINFORMATION);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hOpen);
        return;
    }

    hFirstFile = FtpFindFirstFileA(hConnect, NULL, &w32FindData, 0, 0);
    if (!hFirstFile)
    {
        MessageBoxA(NULL, "Failed to access File!", "Error", MB_OK | MB_ICONERROR);
        InternetCloseHandle(hFirstFile);
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

    hConnect = InternetOpenUrlA(hOpen, "ftp://192.168.0.10:2221", NULL, 0, INTERNET_FLAG_PASSIVE, 0);

    if (!hConnect)
    {
        MessageBox(NULL, "Failed to Connect to server!", "Error", MB_OK | MB_ICONERROR);
        //MessageBox(NULL, "Connected to Server!", "Confirmation", MB_OK | MB_ICONINFORMATION);
        InternetCloseHandle(hOpen);
        InternetCloseHandle(hConnect);
        return;
    }




}















