#ifndef UNICODE
#define UNICODE
#endif // UNICODE

#include <windows.h>
#include "Functions.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{

    const wchar_t CLASS_NAME[]= L"Http Class";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = CLASS_NAME;
    wc.hInstance = hInstance;

    if (!RegisterClass(&wc))
    {
        MessageBox(NULL, L"Class Registration Failed!", L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    HWND hMainWindow = CreateWindowEx(
                0,
                CLASS_NAME,
                L"HTTP Project",
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                NULL,
                NULL,
                hInstance,
                NULL
                );


    if (!hMainWindow)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error", MB_OK | MB_ICONERROR);
    }

    ShowWindow(hMainWindow, nCmdShow);

    MSG msg = {};

    while (GetMessage(&msg, hMainWindow, 0, 0))
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
        case WM_PAINT:
        {
            PAINTSTRUCT ps = {};
            HDC hdc = BeginPaint(hWnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW));

            EndPaint(hWnd, &ps);
        }
        break;

        case WM_CREATE:
        {
            InitializeUI(hWnd);
        }
        break;

        case WM_COMMAND:
        {
            GoButton();
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
