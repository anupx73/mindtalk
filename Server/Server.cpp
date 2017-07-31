
#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "CSCSkin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CServerApp

BEGIN_MESSAGE_MAP(CServerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CServerApp construction

CServerApp::CServerApp()
{
}

// The one and only CServerApp object

CServerApp theApp;

BOOL CServerApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	CSCSkin skin;
	skin.Init();
	skin.LoadSkinFromFile(L"UI.dat");
	skin.ApplySkin();

	CServerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	return FALSE;
}
