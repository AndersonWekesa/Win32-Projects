#pragma once

#define BTN_START 1
#define BTN_STOP 2

#include <Windows.h>
#include <dshow.h>

void Initialize(HWND hWnd);
void StartCapture(HWND hWnd);
void StopCapture();
