// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "LoginDlg.h"
#include "Connection.h"
#include "ClientDlg.h"
#include "TrayIcon.h"
#include "AboutDlg.h"

CString csServer;
CString csNick;

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{
	RegOpenKeyEx (HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hRun);
	RegCreateKeyEx (HKEY_CURRENT_USER, L"Software\\mindTalk", 0, 0, 0, KEY_ALL_ACCESS, 0, &hOpen, 0);
}
//Only Object
CLoginDlg oLgDlg;

CLoginDlg::~CLoginDlg()
{
	RegCloseKey(hRun);
	RegCloseKey(hOpen);
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IP, m_csServerIP);
	DDX_Text(pDX, IDC_EDIT_NAME, m_csBuddyName);
	DDX_Control(pDX, IDC_CHECK_WIN, m_WinStart);
	DDX_Control(pDX, IDC_CHECK_LOGIN, m_AutoLogin);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	ON_WM_CLOSE()
	ON_WM_SIZE()

	ON_BN_CLICKED(IDC_BTN_LOGIN, &CLoginDlg::OnBnClickedBtnLogin)

	ON_COMMAND(ID_CONTEXT_QUIT, &CLoginDlg::OnContextQuit)
	ON_COMMAND(ID_CONTEXT_ABOUT, &CLoginDlg::OnContextAbout)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CLoginDlg message handlers

BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	wchar_t szSever[40] = L"";	DWORD cbSizS = 40;
	wchar_t szName[40] = L"";	DWORD cbSizN = 40;
	DWORD regType = REG_SZ;

	if (RegQueryValueExW(hOpen, L"ServerIP", 0, &regType, (LPBYTE)&szSever, &cbSizS) == ERROR_SUCCESS &&
		RegQueryValueExW(hOpen, L"NickName", 0, &regType, (LPBYTE)&szName, &cbSizN) == ERROR_SUCCESS )
	{
		SetDlgItemTextW(IDC_EDIT_IP, szSever);
		SetDlgItemTextW(IDC_EDIT_NAME, szName);
	}

	if (RegQueryValueExW(hRun, L"mindTalk", 0, NULL, NULL, NULL) == ERROR_SUCCESS)
		m_WinStart.SetCheck(BST_CHECKED);

	if (RegQueryValueExW(hOpen, L"AutoLogin", 0, NULL, NULL, NULL) == ERROR_SUCCESS)
	{
		m_AutoLogin.SetCheck(BST_CHECKED);
		
		cTray.SetTrayIcon(m_hWnd, L"mindTalk");
		cTray.ShowBalloon(NIIF_INFO, L"mindTalk", L"Auto Login in progress. Please wait..");

		OnBnClickedBtnLogin();
	}

	return TRUE;
}

void CLoginDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	Rect rect(5,20,85,77);
	Graphics grpx(dc);

	Image *Img = Image::FromFile(L"logo.png",FALSE);
	grpx.DrawImage(Img,rect);

	//CDialog::OnPaint();
}

BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			OnBnClickedBtnLogin();
			return TRUE;
		}

		if (pMsg->wParam == VK_ESCAPE)
		{
			pMsg->wParam = NULL;

			ShowWindow(SW_HIDE);
			cTray.SetTrayIcon (m_hWnd, L"mindTalk Login");
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CLoginDlg::OnClose()
{
	cCon.CloseConnection();

	//CDialog::OnClose();
	PostQuitMessage(0);
}

void CLoginDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (nType == SIZE_MINIMIZED)
	{
		ShowWindow(SW_HIDE);
		cTray.SetTrayIcon(m_hWnd, L"mindTalk Login");
	}
}

LRESULT CLoginDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	CMenu PopupMenu;
	CPoint point;

	if (message == NIF_MSG)
	{
		switch (LOWORD(lParam))
		{
			case WM_LBUTTONUP:
			{
				ShowWindow(SW_RESTORE);
				SetForegroundWindow();
				cTray.DelTrayIcon();

			}break;

			case WM_RBUTTONUP:
			{
				PopupMenu.LoadMenu(IDR_TRAY);
				
				PopupMenu.EnableMenuItem(ID_CONTEXT_SHOW, MF_GRAYED);

				CMenu *pPopupMenu = PopupMenu.GetSubMenu(0);
				::GetCursorPos(&point);
				SetForegroundWindow();
				pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this, NULL);
				PostMessage(WM_NULL);
			}break;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CLoginDlg::OnBnClickedBtnLogin()
{
	cCon.InitSocket();
	UpdateData();
	
	if (m_WinStart.GetCheck() == BST_CHECKED)
	{
		wchar_t pPath[MAX_PATH];
		GetModuleFileName (0, pPath, MAX_PATH);
		RegSetValueExW (hRun, L"mindTalk", 0, REG_SZ, (LPBYTE)pPath, sizeof (pPath)/sizeof (wchar_t));
	}
	else
		RegDeleteValue(hRun, L"mindTalk");

	if (m_AutoLogin.GetCheck() == BST_CHECKED)
	{
		const DWORD data = 1;
		RegSetValueExW (hOpen, L"AutoLogin", 0, REG_DWORD, (BYTE*)&data, sizeof (DWORD));
	}
	else
		RegDeleteValue(hOpen, L"AutoLogin");

	if (m_csServerIP.IsEmpty() || m_csBuddyName.IsEmpty())
		AfxMessageBox(L"No valid IP or Nick for Login.");

	else
	{		
		RegSetValueExW (hOpen, L"ServerIP", 0, REG_SZ, 
						(CONST BYTE *)(LPCWSTR)m_csServerIP, 
						(m_csServerIP.GetLength()+1) * sizeof (WCHAR));

		RegSetValueExW (hOpen, L"NickName", 0, REG_SZ, 
						(CONST BYTE *)(LPCWSTR)m_csBuddyName, 
						(m_csBuddyName.GetLength()+1) * sizeof (WCHAR));

		csServer = m_csServerIP;
		csNick = m_csBuddyName;
		char *serveradd = new char[m_csServerIP.GetLength()+1];
		char *buddyName = new char[m_csBuddyName.GetLength()+1];
		wcstombs(serveradd, m_csServerIP, m_csServerIP.GetLength()+1);
		wcstombs(buddyName, m_csBuddyName, m_csBuddyName.GetLength()+1);

		if (cCon.ConnectServer(serveradd, buddyName) != -3 )
		{
			ShowWindow(SW_HIDE);		
			oClDlg.DoModal(); 
		}
		else
		{
			AfxMessageBox(L"Server not found!. Plase try later.");
			cTray.DelTrayIcon();
		}
	}
}

void CLoginDlg::OnContextQuit()
{
	cCon.CloseConnection();
	cTray.DelTrayIcon();
	PostQuitMessage(0);
}

void CLoginDlg::OnContextAbout()
{
	oAbDlg.DoModal();
}
