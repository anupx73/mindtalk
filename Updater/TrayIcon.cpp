#include "stdafx.h"
#include "TrayIcon.h"
#include "resource.h"

CTrayIcon cTray;

CTrayIcon::CTrayIcon()
{
	ZeroMemory(&nid, sizeof(NOTIFYICONDATA));
	ZeroMemory(&vi, sizeof(OSVERSIONINFO));
}

CTrayIcon::~CTrayIcon()
{
	DelTrayIcon();
	//delete bData;
}

void CTrayIcon::SetTrayIcon (HWND hWnd, HINSTANCE hIns, LPTSTR szTip)
{
	vi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&vi);
	
	if (vi.dwMajorVersion >= 6)
	{
		nid.cbSize = sizeof(NOTIFYICONDATA);
		nid.uVersion = NOTIFYICON_VERSION_4;
	}

	else if (vi.dwMajorVersion >= 5)
	{
		nid.cbSize = NOTIFYICONDATA_V2_SIZE;	
		nid.uVersion = NOTIFYICON_VERSION;
	}

	Shell_NotifyIcon (NIM_SETVERSION, &nid);

	nid.hWnd = hWnd;
	nid.uID = 0;
	//nid.uCallbackMessage = NIF_MSG;
	nid.hIcon = LoadIcon(hIns, MAKEINTRESOURCE(IDI_ICON1));
	nid.uFlags = NIF_ICON | NIF_TIP;

	_tcscpy_s(nid.szTip, _countof(nid.szTip), szTip);

	Shell_NotifyIcon (NIM_ADD, &nid);
}


void CTrayIcon::ShowBalloon (LPTSTR szInfoTitle, LPTSTR szInfo)
{
	nid.uFlags = NIF_INFO;
	nid.dwInfoFlags = NIIF_INFO;

	_tcscpy_s(nid.szInfoTitle, _countof(nid.szInfoTitle), szInfoTitle);
	_tcscpy_s(nid.szInfo, _countof(nid.szInfo), szInfo);

	Shell_NotifyIcon(NIM_MODIFY, &nid);
}


void CTrayIcon::DelTrayIcon ()
{
	Shell_NotifyIcon(NIM_DELETE, &nid);
}

//void CTrayIcon::StartBlinkTrayIcon(DWORD iDelayTime)
//{
//	bData->iDelay = iDelayTime;
//	bData->hBlink = ::CreateEvent(0, 1, 0, 0);
//	AfxBeginThread(BlinkIconThread, (LPVOID)bData);
//}
//
//void CTrayIcon::StopBlinkTrayIcon()
//{
//	::SetEvent(bData->hBlink);
//}
//
//UINT BlinkIconThread(LPVOID lParam)
//{
//	blinkData *bDataThread = (blinkData*)lParam;
//
//	while (WaitForSingleObject(bDataThread->hBlink, 0) != WAIT_OBJECT_0)
//	{
//		cTray.nid.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME_2);
//		Shell_NotifyIcon(NIM_MODIFY, &cTray.nid);
//		Sleep(bDataThread->iDelay);
//
//		cTray.nid.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
//		Shell_NotifyIcon(NIM_MODIFY, &cTray.nid);
//		Sleep(bDataThread->iDelay);
//
//	}
//	return 0;
//}