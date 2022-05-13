#define _WIN32_WINNT 0x501

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <wininet.h>
#include "Functions.h"

/*
    char Buffer[10];
    DWORD err = WSAGetLastError();            //Error-Checking Code
    itoa(err, Buffer, 10);
    SetWindowTextA(hDisplay, Buffer);
    return;
*/

HWND hDisplay, hAddress;
SOCKET sInSock, sOutSock, sIncoming;
//char Target[15] = {}; // Maximum length of an IP Address

void Initialize(HWND hWnd)
{
    //-----------------------------------Initialize User Interface-------------------------------------------------//

    hAddress = CreateWindowExA(0, "Static", "Message To: ", WS_VISIBLE | WS_CHILD, 10, 20, 100, 20, hWnd, NULL, NULL, NULL);
    CreateWindowExA(0, "Edit", NULL, WS_VISIBLE | WS_BORDER | WS_CHILD, 130, 20, 230, 20, hWnd, NULL, NULL, NULL);
    hDisplay = CreateWindowExA(0, "Edit", NULL, WS_VISIBLE | WS_BORDER | WS_CHILD | ES_MULTILINE, 130, 50, 800, 280, hWnd,
                               NULL, NULL, NULL);
    CreateWindowExA(0, "Button", "Send Message", WS_VISIBLE | WS_BORDER | WS_CHILD, 800, 350, 135, 33, hWnd, (HMENU)BTN_SEND,
                    NULL, NULL);

    //-------------------------------------Initialize WinSock---------------------------------------------------//

    WSADATA wData;

    if (WSAStartup(MAKEWORD(2, 2), &wData))
    {
        MessageBoxA(NULL, "Failed To Initialize WinSock!", "Error", MB_OK | MB_ICONERROR);
        PostQuitMessage(0);
    }

    //-----------------------------------Create Server Thread---------------------------------------------------//

    if (!CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecieveTextMessage, NULL, 0, 0))
    {
        MessageBoxA(NULL, "Failed To Start Server Thread!", "Error", MB_OK | MB_ICONERROR);
        PostQuitMessage(0);
    }


}


void SendTextMessage()
{
    if (!InternetGetConnectedState(NULL, 0))
    {
        MessageBoxA(NULL, "No Internet!", "Error", MB_OK | MB_ICONERROR);
        return;
    }


    struct addrinfo* pAddress = NULL;

    /*struct addrinfo hints;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;*/

    if (getaddrinfo("192.168.0.15", "6007", NULL, &pAddress)) //&hints
    {
        MessageBoxA(NULL, "Address Conversion Failed!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    sOutSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (!sOutSock)
    {
        MessageBoxA(NULL, "Failed to Create Socket!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    /*

            The bind function can be called here to specify the outgoing port number. This can be useful
            in preventing the firewall from blocking the outgoing connection from the port randomly assigned
            by the operating system.
            To do this, create and fill a sockaddr_in struct, including the desired port number, and use that
            to bind the socket.

            struct sockaddr_in sLocalPort;
            sLocalPort.sin_addr.S_addr = inet_addr("192.168.0.12");
            sLocalPort.sin_family = AF_INET;
            sLocalPort.sin_port = htons(5007);

            if (bind(sOutSock, sLocalPort.sin_addr, sizeof(sLocalPort.sin_addr)))
            {
                MessageBoxA(NULL, "Failed to Establish Outgoing Port!", "Error", MB_OK | MB_ICONERROR);
                return;
            }

    */

    if (connect(sOutSock, pAddress->ai_addr, (int)pAddress->ai_addrlen))
    {
        MessageBoxA(NULL, "Failed to Connect to Target!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    int nBufferSize = GetWindowTextLengthA(hDisplay);
    nBufferSize++;
    char* pbuffer = new char[nBufferSize];
    GetWindowTextA(hDisplay, pbuffer, nBufferSize);

    int nBytesSent = 0;
    int nBufferIndex = 0;
    while (nBytesSent < (nBufferSize - 1))
    {
        nBytesSent = send(sOutSock, &pbuffer[nBufferIndex], nBufferSize, NULL);
        nBufferIndex += nBytesSent;
    }
    closesocket(sOutSock);

}


void RecieveTextMessage()
{
    struct addrinfo* pLocalInfo = NULL;
    struct addrinfo hints;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    if(getaddrinfo(NULL, "6007", &hints, &pLocalInfo))
    {
        MessageBoxA(NULL, "Failed to Resolve Local IP Address!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    sInSock = socket(pLocalInfo -> ai_family, pLocalInfo -> ai_socktype, pLocalInfo -> ai_protocol);
    if (sInSock == INVALID_SOCKET)
    {
        MessageBoxA(NULL, "Failed to Create Incoming Socket!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    if(bind(sInSock, pLocalInfo -> ai_addr, (int)pLocalInfo -> ai_addrlen))
    {
        MessageBoxA(NULL, "Failed to Bind Socket!", "Error", MB_OK | MB_ICONERROR);
        closesocket(sInSock);
        return;
    }


    char pInBuffer[1024];
    while(1)
    {

        if(listen(sInSock, SOMAXCONN))
        {
            MessageBoxA(NULL, "Cannot Listen to Socket!", "Error", MB_OK | MB_ICONERROR);
            closesocket(sInSock);
            break;
        }

        sIncoming = accept(sInSock, NULL, NULL);
        if(!sIncoming)
        {
            MessageBoxA(NULL, "Target cannot connect to Socket!", "Error", MB_OK | MB_ICONERROR);
            closesocket(sInSock);
            break;
        }

        while(1)
        {
            ZeroMemory(pInBuffer, sizeof(pInBuffer));
            if( recv(sIncoming, pInBuffer, sizeof(pInBuffer), 0) == 0)
            {
                break;
            }

            SetWindowTextA(hDisplay, (LPCSTR)pInBuffer);
        }

        closesocket(sIncoming);

    }

    //closesocket(sIncoming);

}













