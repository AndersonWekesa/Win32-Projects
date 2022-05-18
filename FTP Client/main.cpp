#ifndef UNICODE
#define UNICODE
#endif // UNICODE

#include <windows.h>
#include "functions.h"

HWND hMainWindow;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{

    const wchar_t CLASS_NAME[] = L"Practice";

    WNDCLASS wc = {};
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = CLASS_NAME;

    if(!RegisterClass(&wc))
    {
        MessageBox(NULL, L"Failed to register Class", L"Error", MB_OK + MB_ICONERROR);
    }

    hMainWindow = CreateWindowEx(
                    0,
                    CLASS_NAME,
                    L"FTP Client",
                    WS_OVERLAPPEDWINDOW,
                    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                    NULL,
                    NULL,
                    hInstance,
                    NULL
                    );

    if (!hMainWindow)
    {
        MessageBox(NULL, L"Failed to create Main Window!", L"Error", MB_OK | MB_ICONERROR);
    }

    ShowWindow(hMainWindow, nCmdShow);

    MSG msg = {};
    while(GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}



LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
         case WM_COMMAND:
        {
            switch(wParam)
            {
                case BTN_UPLOAD:
                {
                    UploadFile();
                }
                break;

                case BTN_DOWNLOAD:
                {
                    DownloadFile();
                }
                break;
            }
        }
        break;

        case WM_CREATE:
        {
            InitializeUI(hWnd);
        }
        break;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + 0));
            EndPaint(hWnd, &ps);
        }
        break;

        case WM_DESTROY:
        {
            PostQuitMessage(0);
        }
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

