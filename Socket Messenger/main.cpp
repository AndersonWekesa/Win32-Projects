#ifndef UNICODE
#define UNICODE
#endif // UNICODE

#include <windows.h>
#include <winsock2.h>
#include "Functions.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND hMainWindow;
extern SOCKET sInSock, sOutSock;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    const wchar_t CLASS_NAME[] = L"Sock Class";

    WNDCLASS wc = {};
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClass(&wc))
    {
        MessageBoxW(NULL, L"Failed to Register Class!", L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }


    hMainWindow = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Beeper",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hMainWindow)
    {
        MessageBoxW(NULL, L"Failed to Create Window!", L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    ShowWindow(hMainWindow, nCmdShow);

    MSG msg = {};

    while (GetMessage(&msg, hMainWindow, NULL, NULL))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }


    return 0;
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    switch (uMsg)
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
        Initialize(hWnd);
    }
    break;

    case WM_COMMAND:
    {
        switch (wParam)
        {
        case BTN_SEND:
        {
            SendTextMessage();
        }
        break;
        }
    }
    break;

    case WM_DESTROY:
    {
        closesocket(sOutSock);
        closesocket(sInSock);
        WSACleanup();
        PostQuitMessage(0);
    }
    break;

    }


    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
