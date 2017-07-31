// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "Connection.h"
#include "LoginDlg.h"
#include "TrayIcon.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
	, m_strNickName(_T(""))
{
}

//Only Object
CClientDlg oClDlg;

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_ONLINE_BUDDY, m_strBuddyList);
	DDX_Text(pDX, IDC_EDIT_DISPLAY, m_strChatText);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_strUserInput);
	DDX_Control(pDX, IDC_STATIC_HEADER_IP, m_header_IP);
	//DDX_Control(pDX, IDC_STATIC_HEADER_NICK, m_header_Nick);
	//DDV_MaxChars(pDX, m_strBuddyName, 4050);
	DDX_Text(pDX, IDC_STATIC_HEADER_NICK, m_strNickName);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	ON_WM_CLOSE()
	ON_WM_SIZE()

	ON_BN_CLICKED(IDC_BTN_CLEAR, &CClientDlg::OnBnClickedClear)
	ON_BN_CLICKED(IDC_BTN_SEND, &CClientDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_BTN_SAVE_LOG, &CClientDlg::OnBnClickedSaveLog)
	
	ON_COMMAND(ID_CONTEXT_SHOW, &CClientDlg::OnContextShowHide)
	ON_COMMAND(ID_CONTEXT_ABOUT, &CClientDlg::OnContextAbout)
	ON_COMMAND(ID_CONTEXT_QUIT, &CClientDlg::OnContextQuit)
	ON_MESSAGE(UWM_NICKUPDATE,UpdateNickName)
	ON_MESSAGE(UWM_UPDATE,UpdateChatBuddy)

	ON_WM_PAINT()
END_MESSAGE_MAP()


// CClientDlg message handlers

BOOL CClientDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			OnBnClickedSend();
			return TRUE;
		}

		if (pMsg->wParam == VK_ESCAPE)
		{
			pMsg->wParam = NULL;

			ShowWindow(SW_HIDE);
			cTray.SetTrayIcon(m_hWnd, L"mindTalk Client");
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CClientDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

				cTray.StopBlinkTrayIcon();
				cTray.DelTrayIcon();

			}break;

			case WM_RBUTTONUP:
			{
				PopupMenu.LoadMenu(IDR_TRAY);
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


BOOL CClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//SetIcon(m_hIcon, TRUE);		// Set big icon
	//SetIcon(m_hIcon, FALSE);		// Set small icon
	
	m_strChatText.Empty();
	m_strUserInput.Empty();

	CString csTempIP = L"Server: " + csServer;
	CString csTempNick = L"Your Nick: " + csNick;
	m_header_IP.SetWindowText(csTempIP.GetBuffer(csTempIP.GetLength()+1));
	//m_header_Nick.SetWindowText(csTempNick.GetBuffer(csTempNick.GetLength()+1));

	AfxBeginThread(GetChatThread,this);
	AfxBeginThread(BuddyUpdateThread,this);

	cTray.DelTrayIcon();
	
	return TRUE; 
}

void CClientDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	Rect rect(40,6,180,52);
	Graphics grpx(dc);

	Image *Img = Image::FromFile(L"logo2.png",FALSE);
	grpx.DrawImage(Img,rect);

	//CDialog::OnPaint();
}

void CClientDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (nType == SIZE_MINIMIZED)
	{
		ShowWindow(SW_HIDE);
		cTray.SetTrayIcon(m_hWnd, L"mindTalk Client");
	}
}

void CClientDlg::OnClose()
{
	if (AfxMessageBox(L"Sure to disconnect from current chat ?", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		cCon.CloseConnection();
		CDialog::OnClose();

		oLgDlg.ShowWindow(SW_RESTORE);
		oLgDlg.SetForegroundWindow();
	}
}

void CClientDlg::OnBnClickedSend() //Send BTN
{
	UpdateData();
	char *szUserMsg;
	int nLen = m_strUserInput.GetLength();
	szUserMsg = new char[nLen+1];
	wcstombs(szUserMsg, m_strUserInput, nLen+1);

	cCon.SendChat(szUserMsg, nLen+1);

	SetDlgItemText(IDC_EDIT_INPUT,_T(""));
	SetFocus();
}

void CClientDlg::OnBnClickedClear() //Clear BTN
{
	m_strChatText.Empty();
	SetDlgItemTextW(IDC_EDIT_DISPLAY, _T(""));
}

void CClientDlg::OnBnClickedSaveLog() //Disconnect BTN chenged to save chat
{
	FILE * fp;

	static TCHAR BASED_CODE szFilter[] = _T("Text File (*.txt)");

	CFileDialog Filegld(FALSE,NULL,NULL,OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, this);

	Filegld.DoModal();
	CString path = Filegld.GetPathName();
	if (path.GetLength()==0)
		return;

	path += _T(".txt");
	CFile file(path,CFile::modeCreate | CFile::modeWrite);
	file.Close();

	if ((fp=_wfopen(path,_T("a+")))==NULL)
		AfxMessageBox(_T("Log file could not be opened"));

	m_strChatText +=_T("\r\n");
	fputws(m_strChatText,fp);
	fclose(fp);
}

void CClientDlg::OnContextShowHide()
{
	ShowWindow(SW_RESTORE);
	SetForegroundWindow();

	cTray.StopBlinkTrayIcon();
	cTray.DelTrayIcon();
}

void CClientDlg::OnContextAbout()
{
	oAbDlg.DoModal();
}

void CClientDlg::OnContextQuit()
{
	cCon.CloseConnection();
	cTray.DelTrayIcon();
	PostQuitMessage(0);
}

LRESULT CClientDlg::UpdateChatBuddy(WPARAM wp,LPARAM lp)
{	
	SetDlgItemText(IDC_EDIT_ONLINE_BUDDY,m_strBuddyList);
	SetDlgItemText(IDC_EDIT_DISPLAY,m_strChatText);
	GetDlgItem(IDC_EDIT_ONLINE_BUDDY)->UpdateWindow();
	GetDlgItem(IDC_EDIT_DISPLAY)->UpdateWindow();
	
	((CEdit*)GetDlgItem(IDC_EDIT_DISPLAY))->SetSel(0,-1);
	//((CEdit*)GetDlgItem(IDC_EDIT_INPUT))->SetSel(-1,-1);
	return 0;
}

LRESULT CClientDlg::UpdateNickName(WPARAM wp, LPARAM lp)
{
	static BOOL bChk = TRUE;
	if (bChk)
	{
		SetDlgItemText(IDC_STATIC_HEADER_NICK, m_strNickName);
		bChk = FALSE;
	}
	return 0;
}
