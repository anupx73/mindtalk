#pragma once

#ifndef NOTIFYICON_VERSION_4
#define NOTIFYICON_VERSION_4 4
#endif

#ifndef NIF_MSG
#define NIF_MSG (WM_APP + 0x100)
#endif

class CTrayIcon	{
public:
	CTrayIcon();
	~CTrayIcon();

	NOTIFYICONDATA nid;
	OSVERSIONINFO vi;

public:
	void SetTrayIcon (HWND, LPWSTR);
	void ShowBalloon (DWORD, LPWSTR, LPWSTR);
	void StartBlinkTrayIcon (DWORD);
	void StopBlinkTrayIcon ();
	void DelTrayIcon ();
};

UINT BlinkIconThread(LPVOID lP);

typedef struct BlinkIconData	{
	HANDLE hBlink;
	DWORD iDelay;
}blinkData;

extern CTrayIcon cTray;