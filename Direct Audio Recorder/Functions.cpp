#include "Functions.h"

/*

	char Buffer[10];
	DWORD err = GetLastError();            //Error-Checking Code
	itoa(err, Buffer, 10);
	SetWindowTextA(hDisplay, Buffer);
	return;

*/

HWND hDisplay;


ICaptureGraphBuilder2* pCaptureBuilder = NULL;
IGraphBuilder* pGraphBuilder = NULL;
IMediaControl* pMediaControl = NULL;
ICreateDevEnum* pDevEnum = NULL;
IEnumMoniker* pEnumMoniker = NULL;
IMoniker* pMoniker = NULL;
IBaseFilter* pAudioCaptureFilter = NULL, * pSinkFilter = NULL;

void Initialize(HWND hWnd)
{
//-------------------------------------------------Initialize UI---------------------------------------------------------------//
	CreateWindowExA(0, "Button", "Start", WS_CHILD | WS_VISIBLE, 300, 200, 120, 50, hWnd, (HMENU)BTN_START, NULL, NULL);
	CreateWindowExA(0, "Button", "Stop", WS_CHILD | WS_VISIBLE, 600, 200, 120, 50, hWnd, (HMENU)BTN_STOP, NULL, NULL);

	hDisplay = CreateWindowExA(0, "Edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER, 150, 20, 690, 25, hWnd, NULL, NULL, NULL);


//------------------------------------------Initialize COM and Other Interfaces------------------------------------------------//
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (!SUCCEEDED(hr))
	{
		MessageBoxW(NULL, L"Failed to Initialize COM!", L"Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, 
		(LPVOID*)&pCaptureBuilder);
	if (!SUCCEEDED(hr))
	{
		MessageBoxW(NULL, L"Failed to Instantiate Capture Graph Builder!", L"Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (LPVOID*)&pGraphBuilder);
	if (!SUCCEEDED(hr))
	{
		MessageBoxW(NULL, L"Failed to Instantiate Filter Graph Builder!", L"Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	hr = pGraphBuilder->QueryInterface(IID_IMediaControl, (LPVOID*)&pMediaControl);
	if (!SUCCEEDED(hr))
	{
		MessageBoxW(NULL, L"Failed to Instantiate Media Controls!", L"Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	pCaptureBuilder -> SetFiltergraph(pGraphBuilder);

//--------------------------------------Enumerate and Select Audio Capture Device----------------------------------------------//
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (LPVOID*)&pDevEnum);
	if (!SUCCEEDED(hr))
	{
		MessageBoxW(NULL, L"Failed to Instantiate Device Enumerator!", L"Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	hr = pDevEnum->CreateClassEnumerator(CLSID_AudioInputDeviceCategory, &pEnumMoniker, NULL);
	if (hr != S_OK)
	{
		MessageBoxW(NULL, L"Failed to Enumerate Audio Devices!", L"Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	hr = pEnumMoniker->Next(1, &pMoniker, NULL);
	if (hr != S_OK)
	{
		MessageBoxW(NULL, L"Failed to Retrive Audio Device Moniker!", L"Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (LPVOID*)&pAudioCaptureFilter);
	if (hr != S_OK)
	{
		MessageBoxW(NULL, L"Failed to Create Audio Capture Filter!", L"Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	hr = pGraphBuilder->AddFilter(pAudioCaptureFilter, L"Audio Capture Filter");
	if (hr != S_OK)
	{
		MessageBoxW(NULL, L"Failed to Add Filter!", L"Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	hr = pCaptureBuilder->SetOutputFileName(&MEDIASUBTYPE_Avi, L"C:\\Users\\opiyo\\Videos\\testadio.avi", &pSinkFilter, NULL);
	if (hr != S_OK)
	{
		MessageBoxW(NULL, L"Failed to Create File Writer!", L"Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	pGraphBuilder->AddFilter(pSinkFilter, L"Audio File Writer Filter");
}


void StartCapture()
{																												
	HRESULT	hr = pCaptureBuilder->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Audio, pAudioCaptureFilter, NULL, pSinkFilter);
	if (hr == E_INVALIDARG)
	{
		MessageBoxW(NULL, L"Failed to Start Audio Capture!", L"Error", MB_OK | MB_ICONERROR);
		return;
	}

	MessageBoxW(NULL, L"Capture Started!", L"Confirmation", MB_OK | MB_ICONINFORMATION);

	pMediaControl->Run();
	
}



void StopCapture()
{
	pMediaControl->Stop();
}



