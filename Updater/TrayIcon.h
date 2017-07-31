#pragma once

#ifndef NOTIFYICON_VERSION_4
#define NOTIFYICON_VERSION_4 4
#endif

class CTrayIcon	{
public:
	CTrayIcon();
	~CTrayIcon();

	NOTIFYICONDATA nid;
	OSVERSIONINFO vi;

public:
	void SetTrayIcon (HWND, HINSTANCE, LPTSTR);
	void ShowBalloon (LPTSTR, LPTSTR);
	//void StartBlinkTrayIcon (DWORD);
	//void StopBlinkTrayIcon ();
	void DelTrayIcon ();
};

//UINT BlinkIconThread(LPVOID lP);
//
//typedef struct BlinkIconData	{
//	HANDLE hBlink;
//	DWORD iDelay;
//}blinkData;

extern CTrayIcon cTray;