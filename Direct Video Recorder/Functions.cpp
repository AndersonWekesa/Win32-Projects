#include "Functions.h"

ICaptureGraphBuilder2* pCaptureGraphBuilder = NULL;
IGraphBuilder* pGraphBuilder = NULL;
IMediaControl* pMediaControl = NULL;
IVideoWindow* pVideoWindow = NULL;
ICreateDevEnum* pDevEnum = NULL;
IEnumMoniker* pEnumMoniker = NULL;
IMoniker* pMoniker = NULL;
IBaseFilter* pVideoCaptureFilter = NULL;

void Initialize(HWND hWnd)
{
	CreateWindowExA(0, "Button", "Start", WS_CHILD | WS_VISIBLE, 830, 380, 120,
		50, hWnd, (HMENU)BTN_START, NULL, NULL);
	CreateWindowExA(0, "Button", "Stop", WS_CHILD | WS_VISIBLE, 830, 440, 120,
		50, hWnd, (HMENU)BTN_STOP, NULL, NULL);

//-------------------------------------Initialize COM and Other Video Capture Elements------------------------------------------//
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to Initialize COM!", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER,
		IID_ICaptureGraphBuilder2, (LPVOID*)&pCaptureGraphBuilder);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to Instantiate Capture Graph Builder!", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder,
		(LPVOID*)&pGraphBuilder);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to Instantiate Capture Graph Builder!", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	hr = pGraphBuilder->QueryInterface(IID_IMediaControl, (LPVOID*)&pMediaControl);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to Instantiate Media Controls!", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	hr = pGraphBuilder->QueryInterface(IID_IVideoWindow, (LPVOID*)&pVideoWindow);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to Instantiate Video Window!", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	pCaptureGraphBuilder->SetFiltergraph(pGraphBuilder);



	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (LPVOID*)&pDevEnum);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to Instantiate Device Enumerator!", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumMoniker, NULL);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to Enumerate Video Inpute Devices!", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	hr = pEnumMoniker->Next(1, &pMoniker, NULL);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to Retrieve Video Device Moniker!", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (LPVOID*)&pVideoCaptureFilter);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to Create Video Capture Filter!", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	pGraphBuilder->AddFilter(pVideoCaptureFilter, L"Video Input Filter");
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to Add Video Capture Filter!", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	//pCaptureGraphBuilder -> SetOutputFileName()

	pCaptureGraphBuilder->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, pVideoCaptureFilter, NULL, NULL);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to Connect Filter Pins!", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

}

void StartCapture(HWND hWnd)
{
	pVideoWindow->put_Owner((OAHWND)hWnd);
	pVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
	pVideoWindow->SetWindowPosition(10, 10, 600, 480);
	pVideoWindow->put_Visible(true);

	HRESULT hr = pMediaControl->Run();
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to Start Capture!", "Error", MB_OK | MB_ICONERROR);
		return;
	}

}

void StopCapture()
{
	pMediaControl->Stop();

}