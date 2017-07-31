#include "stdafx.h"
#include "TrayIcon.h"
#include "resource.h"

bool GetFile (TCHAR *szUrl, TCHAR *szFileName) 
{
	HINTERNET hOpen, hConnect;
	DWORD dwSize, lpNumBytes;
	TCHAR szHead[] = _T("Accept: */*\r\n\r\n");
	VOID *szTemp[512];
	HANDLE hFile;

	hOpen = InternetOpen(_T("InetURL/1.0"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
	if ( !(hConnect = InternetOpenUrl ( hOpen, szUrl, szHead, _tcslen (szHead), INTERNET_FLAG_DONT_CACHE, 0)))
	{
		MessageBox(0,"InternetOpen err..",0,0);
		return false;
	}

	hFile = CreateFile(szFileName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == NULL)
	{
		MessageBox(0,"createFile err..",0,0);
		return false;
	}

	while (1)
	{
		if (!InternetReadFile (hConnect, szTemp, 511,  &dwSize) )
		{
			MessageBox(0,"InternetReadFile err..",0,0);
			CloseHandle(hFile);
			return false;
		}
		if (!dwSize)
			break;  
		else
			WriteFile(hFile, szTemp, dwSize, &lpNumBytes, NULL);
	}

	CloseHandle(hFile);

	return true;
}

void KillClientServer()
{
	DWORD dID = 0;
	HWND hWServer = NULL;
	HWND hWClient = NULL;

	hWServer = ::FindWindow(_T("#32770"), _T("mindtalk - server"));
	if (IsWindow(hWServer))
	{
		::GetWindowThreadProcessId(hWServer, &dID);
		HANDLE hProc = ::OpenProcess(PROCESS_TERMINATE, TRUE, dID);
		TerminateProcess(hProc, 0);
		CloseHandle(hProc);
	}

	hWClient = ::FindWindow(_T("#32770"), _T("mindtalk - login"));
	if (hWClient == NULL)
		hWClient = ::FindWindow(_T("#32770"), _T("mindtalk - client"));

	if (IsWindow(hWClient))
	{
		::GetWindowThreadProcessId(hWClient, &dID);
		HANDLE hProc = ::OpenProcess(PROCESS_TERMINATE, TRUE, dID);
		TerminateProcess(hProc, 0);
		CloseHandle(hProc);
	}
	
	HWND hTray = ::FindWindowEx(::FindWindow(_T("Shell_TrayWnd"), 0), 0, _T("TrayNotifyWnd"), 0);
	HWND hSysPager = ::FindWindowEx(hTray, 0, _T("SysPager"), 0);
	HWND hNotiArea = ::FindWindowEx(hSysPager, 0, 0, _T("Notification Area"));

	::SendMessage(hNotiArea, WM_MOUSEMOVE, 0, 0);
}

int __stdcall WinMain(HINSTANCE hIns, HINSTANCE, LPSTR szParam, int)
{
	HWND hWnd = CreateDialog(hIns, MAKEINTRESOURCE(IDD_DIALOG1), 0, 0);
	ShowWindow(hWnd, SW_HIDE);

	/*if (_tcscmp(szParam,_T("lol090")) != 0)
	{
		MessageBox(0, _T("You can't run updater this way.."), _T("Updater error"), MB_OK | MB_ICONERROR);
		return -1;
	}*/


	bool bGet = GetFile(_T("http://mindtalk.googlecode.com/files/version.ini"), _T("version.ini"));

	/*if (bGet == true)
		MessageBox(0, "true", 0, 0);

	else if(bGet == false)
		MessageBox(0, "false", 0, 0);*/

	return 0;
	TCHAR iniVer[10] = _T("");
	GetPrivateProfileString(_T("programinfo"), _T("version"), NULL, iniVer, 10, _T(".\\version.ini")); 

	if (_tcscmp(iniVer, _T("")) == 0)
	{
		MessageBox(0, "iniVer null", 0, 0);
		//DeleteFile(_T("version.ini"));
		return -1;
	}

	HKEY hOpend;
	DWORD regType = REG_SZ;
	TCHAR regVer[10] = _T("");
	DWORD cbSiz = 10;
	RegOpenKeyEx (HKEY_CURRENT_USER, _T("Software\\mindtalk"), 0, KEY_ALL_ACCESS, &hOpend);
	RegQueryValueEx(hOpend, _T("version"), 0, &regType, (LPBYTE)&regVer, &cbSiz);
	RegCloseKey(hOpend);

	if (strcmp(iniVer, regVer) != 0)
	{
		if (MessageBox(0, _T("An update is available. Download now ?"), _T("mindTalk updater"), MB_YESNO | MB_TOPMOST) == IDYES)
		{
			cTray.SetTrayIcon(hWnd, hIns, _T("mindTalk updater"));
			cTray.ShowBalloon(_T("mindTalk"), _T("Updating mindTalk. Please wait.."));

			KillClientServer();

			TCHAR upClient[2] = _T("");
			TCHAR upServer[2] = _T("");
			TCHAR upUIDat[2] = _T("");
			TCHAR upLogo[2] = _T("");
			TCHAR upExtras[2] = _T("");

			//----Client
			GetPrivateProfileString(_T("Updates"), _T("Client"), NULL, upClient, 2, _T(".\\version.ini"));
			if (_tcscmp(upClient, _T("1")) == 0)
			{
				GetFile(_T("http://mindtalk.googlecode.com/files/Client.exe"), _T("Client_.exe"));
				DeleteFile(_T("Client.exe"));
				MoveFile(_T("Client_.exe"), _T("Client.exe"));
			}

			//----Server
			GetPrivateProfileString(_T("Updates"), _T("Server"), NULL, upServer, 2, _T(".\\version.ini"));
			if (_tcscmp(upServer, _T("1")) == 0)
			{
				GetFile(_T("http://mindtalk.googlecode.com/files/Server.exe"), _T("Server_.exe"));
				DeleteFile(_T("Server.exe"));
				MoveFile(_T("Server_.exe"), _T("Server.exe"));
			}

			//----UIDat
			GetPrivateProfileString(_T("Updates"), _T("UiDat"), NULL, upUIDat, 2, _T(".\\version.ini"));
			if (_tcscmp(upUIDat, _T("1")) == 0)
			{
				GetFile(_T("http://mindtalk.googlecode.com/files/UI.dat"), _T("UI_.dat"));
				DeleteFile(_T("UI.dat"));
				MoveFile(_T("UI_.dat"), _T("UI.dat"));
			}

			//----Logo
			GetPrivateProfileString(_T("Updates"), _T("logo"), NULL, upLogo, 2, _T(".\\version.ini"));
			if (_tcscmp(upLogo, _T("1")) == 0)
			{
				GetFile(_T("http://mindtalk.googlecode.com/files/logo.png"), _T("logo_.png"));
				DeleteFile(_T("logo.png"));
				MoveFile(_T("logo_.png"), _T("logo.png"));
			}

			//----Extras
			GetPrivateProfileString(_T("Updates"), _T("extras"), NULL, upExtras, 2, _T(".\\version.ini"));
			if (_tcscmp(upExtras, _T("1")) == 0)
			{
				GetFile(_T("http://mindtalk.googlecode.com/files/extra1.dat"), _T("extra1.dat"));
				GetFile(_T("http://mindtalk.googlecode.com/files/extra2.dat"), _T("extra2.dat"));
			}

			Sleep(200);

			ShellExecute(0, _T("open"), _T("client.exe"), 0, 0, SW_SHOW);

			HKEY hKey;
			RegOpenKeyEx (HKEY_CURRENT_USER, _T("Software\\mindtalk"), 0, KEY_ALL_ACCESS, &hKey);
			RegSetValueEx (hKey, _T("version"), 0, REG_SZ, (LPBYTE)iniVer, (_tcslen(iniVer)+1)*sizeof(TCHAR));
			RegCloseKey(hKey); 

			cTray.DelTrayIcon();
		}
	}
	DeleteFile(_T("version.ini"));

	DestroyWindow(hWnd);

	return 0;
}