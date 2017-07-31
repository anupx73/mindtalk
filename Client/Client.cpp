// Client.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CSCSkin.h"
#include "Client.h"
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CClientApp

BEGIN_MESSAGE_MAP(CClientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CClientApp construction

CClientApp::CClientApp()
{
}


CClientApp theApp;

// CClientApp initialization

UINT UpdaterThread(LPVOID lParam)
{
	Sleep(4000);
	ShellExecute(0, L"open", L"updater.exe", L"lol090", 0, SW_HIDE);

	return 0;
}

BOOL CClientApp::InitInstance()
{
	CWinApp::InitInstance();
	WIN32_FIND_DATA FindFileData;

	if (!AfxSocketInit())
	{
		AfxMessageBox(L"Socket initializing failed! Closing application.");
		return FALSE;
	}

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	
	if (FindFirstFile(L"UI.dll", &FindFileData) == INVALID_HANDLE_VALUE ||
		FindFirstFile(L"UI.dat", &FindFileData) == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(L"Missing UI Library Component. Reinstall mindTalk!", MB_ICONERROR);
		return FALSE;
	}
	
	AfxBeginThread(UpdaterThread, 0);

	CSCSkin skin;
	skin.Init();
	skin.LoadSkinFromFile(L"UI.dat");
	skin.ApplySkin();

	m_pMainWnd = &oLgDlg;
	oLgDlg.DoModal();
		
	return FALSE;
}