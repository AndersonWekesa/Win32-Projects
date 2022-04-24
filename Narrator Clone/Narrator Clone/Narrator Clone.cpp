#include "header.h"
#include "Narrator Clone.h"
#include <ShObjIdl_core.h>
#include <sapi.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void loadMainUI(HWND hwnd);
void IntroVoice(HWND hwnd);
void directVoiceX(HWND hwnd);
void directVoiceY(HWND hwnd);

HWND hMainWindow;

HRESULT hr;
ISpVoice *pVoice;
POINT cursorPoint;


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdShow, int nCmdShow) {

	HRESULT hrMain = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	if (FAILED(hrMain)) {
		MessageBox(NULL, L"Error initializing COM", L"Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	const wchar_t CLASS_NAME[] = L"NarratorClone";

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	hMainWindow = CreateWindowEx(0, CLASS_NAME, L"Narrator Clone", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL);

	if (hMainWindow == NULL){
		return FALSE;
	}

	ShowWindow(hMainWindow, nCmdShow);

	MSG msg = {};

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (SUCCEEDED(hrMain)) {
		CoUninitialize();
	}

	return 0;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg) {

		case WM_CREATE:
		{
			loadMainUI(hwnd);
			SetTimer(hwnd, 1, 3000, NULL);
		}
		break;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

			EndPaint(hwnd, &ps);
		}
		break;

		case WM_TIMER:
		{
			KillTimer(hwnd, 1);
			IntroVoice(hwnd);
		}
		break;

		case WM_COMMAND:
		{
			switch (wParam) {
				case BTN_CLOSE:
				{
					DestroyWindow(hwnd);
				}
				break;
			}
		}
		break;

		case WM_CLOSE:
		{
			int result = MessageBox(hwnd, L"Are you sure you want to quit?", L"Quit?", MB_OKCANCEL | MB_ICONQUESTION);
			
			if (result == IDOK) {
				pVoice->Release();
				DestroyWindow(hwnd);
			}
			else {
				return 0;
			}
		}
		break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


void loadMainUI(HWND hwnd) {
	HWND hButton = CreateWindowEx(0, L"Button", L"Find Me!", WS_CHILD | WS_VISIBLE, 280, 200, 150, 30, hwnd, (HMENU) BTN_CLOSE,
		NULL, NULL);

}

void IntroVoice(HWND hwnd) {
	pVoice = NULL;
	hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, reinterpret_cast<void**>(&pVoice));

	if (SUCCEEDED(hr)) {
		pVoice->Speak(L"Welcome, User! Close your eyes and move the cursor as instructed.", SPF_DEFAULT, NULL);
	}

	directVoiceX(hwnd);
}


void directVoiceX(HWND hwnd) {
	//X-limit = 340
	
	if (cursorPoint.x < 330) {
		while (cursorPoint.x < 330) {
			GetCursorPos(&cursorPoint);
			ScreenToClient(hwnd, &cursorPoint);
			pVoice->Speak(L"Move right", SPF_DEFAULT, NULL);
		}
	}
	
	if (cursorPoint.x > 350) {
		while (cursorPoint.x > 350) {
			GetCursorPos(&cursorPoint);
			ScreenToClient(hwnd, &cursorPoint);
			pVoice->Speak(L"Move left", SPF_DEFAULT, NULL);
		}
	}


	pVoice->Speak(L"Stop there", SPF_DEFAULT, NULL);
	
	SetTimer(hwnd, 2, 2000, (TIMERPROC)directVoiceY);
}


void directVoiceY(HWND hwnd) {
	//Y-limit = 215

	KillTimer(hwnd, 2);

	if (cursorPoint.y < 210) {
		while (cursorPoint.y < 215) {
			GetCursorPos(&cursorPoint);
			ScreenToClient(hwnd, &cursorPoint);
			pVoice->Speak(L"Move down", SPF_DEFAULT, NULL);
		}
	}

	if (cursorPoint.y > 220) {
		while (cursorPoint.y > 220) {
			GetCursorPos(&cursorPoint);
			ScreenToClient(hwnd, &cursorPoint);
			pVoice->Speak(L"Move up", SPF_DEFAULT, NULL);
		}
	}

	GetCursorPos(&cursorPoint);
	ScreenToClient(hwnd, &cursorPoint);

	if (cursorPoint.x > 330) {
		if (cursorPoint.x < 350) {
			//Do nothing....

		}
		else {
			directVoiceX(hwnd);
		}
	}
	

	pVoice->Speak(L"Stop there. You've got the button.", SPF_DEFAULT, NULL);

	pVoice -> Speak(L"This button is called Find Me. Press it to exit the program.", SPF_DEFAULT, NULL);

}





