#ifndef UNICODE
#define UNICODE
#endif // UNICODE

#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#include <shellapi.h>
#include "menucmd.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void InitializeMenu(HWND hWnd);
void InitializeControls(HWND hWnd);

HINTERNET hInternetOpen, hOpenUrl;
HWND hEditBox, hDisplay;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int CmdShow)
{

    const wchar_t CLASS_NAME[] = L"Practice Class";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClass(&wc))
    {
        MessageBox(NULL, L"Failed to register Class", L"Class Registration Error", MB_OK + MB_ICONERROR);
    }

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Hello Program",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
        );

        if (hwnd == NULL)
        {
            MessageBoxA(NULL, "Failed to create Window", "Window Creation Failed", MB_OK + MB_ICONERROR);
        }

        ShowWindow(hwnd, 1);

        MSG msg = {};
        while(GetMessage(&msg, NULL, 0, 0) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return 0;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    switch(uMsg)
    {
        case WM_COMMAND:
            switch(wParam)
            {
                case FILE_MENU_OPEN:
                    MessageBeep(MB_ICONERROR);
                break;
                case FILE_MENU_SAVE:
                    MessageBeep(MB_OK);
                break;
                case FILE_MENU_EXIT:
                    DestroyWindow(hwnd);
                break;
                case GO_BUTTON:
                    if (InternetGetConnectedState(NULL, 0)== true)
                    {
                        hInternetOpen = InternetOpenA("HttpPractice", LOCAL_INTERNET_ACCESS,
                                                           NULL, NULL, 0);
                        if (hInternetOpen == NULL)
                        {
                            MessageBox(NULL, L"Failed to create Internet Handle", L"Error", MB_OK | MB_ICONERROR);
                        }
                        else
                        {
                            hOpenUrl = InternetOpenUrl(hInternetOpen, L"https://bing.com", NULL, 0,
                                                       INTERNET_SERVICE_HTTP , NULL);

                            if (hOpenUrl == NULL)
                            {
                                MessageBox(NULL, L"Failed to connect to site!", L"Error", MB_OK | MB_ICONERROR);
                            }
                            else
                            {
                                char pResourceSize[] = "Content-Length";  //1256 Bytes
                                DWORD BufferSize = sizeof(pResourceSize);
                                if (HttpQueryInfoA(hOpenUrl, HTTP_QUERY_CUSTOM, &pResourceSize, &BufferSize, NULL)== true)
                                {

                                    /*char Buffer[10];
                                    DWORD err = GetLastError();            //For printing any error codes
                                    itoa(err, Buffer, 10);
                                    SetWindowTextA(hDisplay, Buffer);*/
                                    char *pMainBuffer = new char[*pResourceSize - '0']; // char to int
                                    *pMainBuffer = 0;
                                    char *pTempBuffer = new char[*pResourceSize - '0']; //char to int
                                    DWORD dwBytesRead;

                                    while(InternetReadFile(hOpenUrl, pTempBuffer, *pResourceSize, &dwBytesRead) && (dwBytesRead) != 0)
                                    {
                                        strncat(pMainBuffer, pTempBuffer, dwBytesRead);
                                    }
                                    SetWindowTextA(hDisplay, (LPCSTR)pMainBuffer);
                                }

                            }

                        }
                    }
                    else
                    {
                        MessageBox(NULL, L"No Internet!", L"Error", MB_OK | MB_ICONERROR);
                    }
                break;
            }
        break;

        case WM_CREATE:
            //InitializeMenu(hwnd);
            InitializeControls(hwnd);
        break;

        case WM_DESTROY:
            InternetCloseHandle(hOpenUrl);
            InternetCloseHandle(hInternetOpen);
            PostQuitMessage(0);
        break;

        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+0));

            EndPaint(hwnd, &ps);
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void InitializeMenu(HWND hWnd)
{
    HMENU hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_OPEN, L"Open");
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_SAVE, L"Save");
    //AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, L"Exit");

    SetMenu(hWnd, hMenu);

}

void InitializeControls(HWND hWnd)
{
    hDisplay = CreateWindow(L"Edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE |WS_VSCROLL,
                            10, 80, 983, 410, hWnd, NULL, NULL, NULL);
    CreateWindow(L"Static", L"Enter URL:", WS_VISIBLE | WS_CHILD, 10, 20, 80, 20, hWnd, NULL, NULL, NULL);
    hEditBox = CreateWindow(L"Edit", NULL, WS_VISIBLE | WS_CHILD, 100, 20, 703, 20, hWnd, NULL, NULL, NULL);
    CreateWindow(L"Button", L"Go", WS_VISIBLE | WS_CHILD, 850, 15, 98, 30, hWnd, (HMENU)GO_BUTTON, NULL, NULL);
}






