
#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerDlg dialog
CList <ClientInfo*,ClientInfo*> ClientInfoStructureLst;
UINT ServerStart (LPVOID);
UINT BuddyUpdate (LPVOID);
UINT ClientExecution (LPVOID);

CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
: CDialog(CServerDlg::IDD, pParent)
, m_nBuddyNo(0)
, m_ctrlRunning(0)
, m_ctrlStopped(0)
, m_strServerLog(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	chk = 0;
	m_nSNotify = 0;
	m_bStartStop = TRUE;
	m_nMode = 0;
	m_bStSpBnClk = FALSE;
	m_bStopStatus = TRUE;
	m_nStartStopCheck = 0;
	m_bCheck = TRUE;
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BUDDY_NO, m_nBuddyNo);
	DDX_Text(pDX, IDC_EDIT_SERVER_LOG, m_strServerLog);
	DDX_Control(pDX, IDC_BUTTON_START, m_ctrlStartStop);
	DDX_Control(pDX, IDC_BUTTON_STOP_ACCEPTING, m_ctrlStopAccep);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_START, &CServerDlg::OnBnClickedButtonStart)
	ON_MESSAGE(UC_DLGUPDATE,UpdateDialog)
	ON_MESSAGE(WM_ICON_MESSAGE,OnIconClick)
	ON_COMMAND(WM_SHOW,OnShow)
	ON_COMMAND(ID_MENU_EXIT,ExitCall)
	//ON_BN_CLICKED(IDC_BUTTON_STOP, &CServerDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDOK, &CServerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CServerDlg::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_SOUND, &CServerDlg::OnBnClickedCheckSound)
	ON_BN_CLICKED(IDC_BUTTON_STOP_ACCEPTING, &CServerDlg::OnBnClickedButtonStopAccepting)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CServerDlg::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_RADIO_STOPPED, &CServerDlg::OnBnClickedRadioStopped)
	ON_BN_CLICKED(IDC_RADIO_RUNNING, &CServerDlg::OnBnClickedRadioRunning)
END_MESSAGE_MAP()


// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	m_VersionNo = MAKEWORD(2,2);
	int err= WSAStartup(m_VersionNo,&m_WSAdata);
	if (err!=0)
	{
		AfxMessageBox(_T("Error DLL initialization"));
		return FALSE;
	}
	if (LOBYTE(m_WSAdata.wVersion)!=2 || HIBYTE(m_WSAdata.wVersion)!=2)
	{
		AfxMessageBox(_T("No match for requested DLL"));
		WSACleanup( );
		return FALSE;
	}
	m_bChkBtnStat = FALSE;
	((CButton*)GetDlgItem(IDC_RADIO_STOPPED))->SetCheck(BST_CHECKED);
	m_ctrlStopAccep.EnableWindow(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CServerDlg::UpdateDialog(WPARAM wp,LPARAM lp)
{
	UpdateData(FALSE);
	((CEdit *)GetDlgItem(IDC_EDIT_SERVER_LOG))->SetSel(0,-1);
	return 0;
}

void CServerDlg::OnBnClickedButtonStart()
{
	// TODO: Add your control notification handler code here
	if (m_bStartStop)
	{
		AfxBeginThread(ServerStart,this,THREAD_PRIORITY_NORMAL);
		m_bChkBtnStat = TRUE;
		((CButton*)GetDlgItem(IDC_RADIO_RUNNING))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_RADIO_STOPPED))->SetCheck(BST_UNCHECKED);
		m_ctrlStartStop.SetWindowText(_T("Stop Server"));
		m_bStartStop = FALSE;
		m_ctrlStopAccep.EnableWindow(TRUE);
		m_bStopStatus = TRUE;
	}
	else
	{
		m_nStartStopCheck = 2;
		OnBnClickedButtonReset();
		if (m_bStopStatus)
		{
			OnBottonStop();
		}
		if (m_nStartStopCheck == 0)
		{
			CString Log;
			SYSTEMTIME time;
			GetLocalTime(&time);
			Log.Format(_T("-> Server stopped at %d:%d:%d on %d/%d/%d \r\n"),time.wHour,time.wMinute,time.wSecond,time.wDay,time.wDayOfWeek,time.wYear);
			m_strServerLog+=Log;
			PostMessage(UC_DLGUPDATE,0,0);
		}
		m_ctrlStartStop.SetWindowText(_T("Start Server"));
		m_ctrlStopAccep.EnableWindow(FALSE);
		m_bStartStop = TRUE;
		m_bCheck = TRUE;
		m_ctrlStopAccep.SetWindowText(_T("Stop\r\nAccepting\r\nConnections"));
	}

}


UINT ServerStart (LPVOID pPnt)
{
	CServerDlg * pParent=(CServerDlg *)pPnt;

	//int chk = AfxMessageBox(_T("Do you want to activate sound notification?"),MB_YESNO);
	//if (chk == 6)
	//{
	//	pParent->m_nSNotify = 1;
	//}

	pParent->sock=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in ClientAddress;
	ClientAddress.sin_family=AF_INET;
	ClientAddress.sin_port=htons(5000);
	ClientAddress.sin_addr.s_addr=INADDR_ANY;
	pParent->m_clientadd = (struct sockaddr FAR*)&ClientAddress;
	bind(pParent->sock,pParent->m_clientadd,sizeof(*pParent->m_clientadd));
	listen(pParent->sock,1);

	CString Log;
	SYSTEMTIME time;
	GetLocalTime(&time);
	if (pParent->m_bStSpBnClk)
	{
		Log.Format(_T("-> [%d/%d/%d (%d:%d:%d)] Server is running in open-for-connection mode.\r\n"),time.wDay,time.wDayOfWeek,time.wYear,time.wHour,time.wMinute,time.wSecond);
		pParent->m_bStSpBnClk = FALSE;
	}
	else
	{
		Log.Format(_T("-> Server Srarted at %d:%d:%d on %d/%d/%d \r\n"),time.wHour,time.wMinute,time.wSecond,time.wDay,time.wDayOfWeek,time.wYear);
	}
	pParent->m_strServerLog+=Log;
	pParent->SendMessage(UC_DLGUPDATE,0,0);

	while (1)
	{

		struct sockaddr_in ClintInformation,BuddyConnection;
		int sin_size = sizeof(struct sockaddr_in);
		SOCKET ClientChatSocket=accept(pParent->sock,(struct sockaddr *)&ClintInformation,&sin_size);
		if (ClientChatSocket==INVALID_SOCKET )
		{
			break;
		}
		SOCKET ClientBuddySocket=accept(pParent->sock,(struct sockaddr *)&BuddyConnection,&sin_size);
		if (ClientChatSocket==INVALID_SOCKET )
		{
			break;
		}
		char chk[1024];
		int nCount = recv(ClientChatSocket,chk,1024,0);

		if (nCount == -1)
		{
			break;
		}
		if (strcmp(chk,"$t0p")==0)
		{
			break;
		}
		else
		{
			pParent->m_nBuddyNo +=1;
			char * tempadd=inet_ntoa(ClintInformation.sin_addr);
			ClientInfo *pClientinfo = new ClientInfo;
			strcpy(pClientinfo->ClientIp,tempadd);
			pClientinfo->ChatPort=htons(ClintInformation.sin_port);
			pClientinfo->ChatSocket=ClientChatSocket;

			//TCHAR tcBuffer[512];
			//mbstowcs(tcBuffer,chk,strlen(chk)+1);

			POSITION pos;
			pos = ClientInfoStructureLst.GetHeadPosition();
			int nDuplicate = 0;
			ClientInfo tempClientInfo;
			char dChk[1024];
			char cReserve[1024];
			strcpy(cReserve,chk);
			while (pos != NULL)
			{
				strcpy(dChk,chk);
				pParent->lock.Lock();
				tempClientInfo = *ClientInfoStructureLst.GetNext(pos);
				pParent->lock.Unlock();
				//if (tempClientInfo == NULL)
				//{
				//	ASSERT(_T("ClientInfoStructureLst syncronization failed"));
				//	return -1;
				//}
				if (nDuplicate != 0)
				{
					char cTemp[125];
					itoa(nDuplicate,cTemp,10);
					strcat(dChk,cTemp);
				}
				if (strcmp(tempClientInfo.ClientName,dChk) == 0)
				{
					nDuplicate = nDuplicate+1;
				}

			}
			//delete tempClientInfo;
			if (nDuplicate != 0)
			{
				char cTemp[125];
				itoa(nDuplicate,cTemp,10);
				strcat(chk,cTemp);
			}
			int nIncrease;
			pos = ClientInfoStructureLst.GetHeadPosition();
			while (pos != NULL)
			{
				tempClientInfo = *ClientInfoStructureLst.GetNext(pos);
				if (strcmp(chk,tempClientInfo.ClientName)==0)
				{
					pos = ClientInfoStructureLst.GetHeadPosition();
					strcpy(chk,cReserve);
					nDuplicate += 1;
					char cTemp[125];
					itoa(nDuplicate,cTemp,10);
					strcat(chk,cTemp);
				}
			}

			strcpy(pClientinfo->ClientName,chk);


			pClientinfo->BuddyUpdateSocket = ClientBuddySocket;
			pClientinfo->ByddyUpdatePort = htons(BuddyConnection.sin_port);

			ClientInfoStructureLst.AddTail(pClientinfo);

			GetLocalTime(&time);
			Log.Format(_T("-> Connection accepted from "));
			Log+=tempadd;
			CString temp,tempname;
			tempname += chk;
			temp.Format(_T(" Port No %d Name %s at %d:%d:%d on %d/%d/%d \r\n"),htons(ClintInformation.sin_port),tempname.GetString(),time.wHour,time.wMinute,time.wSecond,time.wDay,time.wDayOfWeek,time.wYear);
			Log+=temp;
			pParent->m_strServerLog+=Log;
			pParent->SendMessage(UC_DLGUPDATE,0,0);
			pClientinfo->pParent = pParent;
			AfxBeginThread(BuddyUpdate,pClientinfo);
			AfxBeginThread(ClientExecution,pClientinfo);
			if (pParent->m_nSNotify == 1)
			{
				Beep(700,200);
			}
			//delete tempadd;
		}

	}
	closesocket(pParent->sock);
	return 0;
}


UINT BuddyUpdate (LPVOID pPnt)
{
	ClientInfo * pstrClientInfo;
	CServerDlg * pParent = NULL;
	if (pPnt != NULL)
	{
		pstrClientInfo = (ClientInfo *)pPnt;
		pParent = (CServerDlg *)pstrClientInfo->pParent;
	}
	POSITION pos;
	char * pNList; 
	pos = ClientInfoStructureLst.GetHeadPosition();
	CString BuddyList;
	char cHandShek[2];
	if(pParent)
	{
		pParent->lock.Lock();
	}
	while (pos != NULL)
	{
		ClientInfo * SendSocketInfo = ClientInfoStructureLst.GetNext(pos);
		if (SendSocketInfo != NULL)
		{
			BuddyList += SendSocketInfo->ClientName;
			BuddyList += "\r\n";
		}
	}
	if (pParent)
	{
		pParent->lock.Unlock();
	}

	int nLen = BuddyList.GetLength()+1;
	pNList = new char[nLen];
	memset(pNList,0,nLen);
	wcstombs(pNList,BuddyList,nLen);
	pos = ClientInfoStructureLst.GetHeadPosition();
	if(pParent)
	{
		pParent->lock.Lock();
	}
	while (pos != NULL)
	{
		ClientInfo *SendSocketInfo = ClientInfoStructureLst.GetNext(pos);
		if (SendSocketInfo == NULL)
		{
			ASSERT(_T("Could not find SendSocketInfo"));
			return -1;
		}
		int nBytesSent = send(SendSocketInfo->BuddyUpdateSocket,"GRP",4,0);
		char buf[128];
		memset(buf,0,128);

		recv(SendSocketInfo->BuddyUpdateSocket,cHandShek,1,0);

		//nBytesSent = strlen(buf);
		sprintf(buf,"%d",nLen);
		nBytesSent = send(SendSocketInfo->BuddyUpdateSocket,buf,128,0);
		// send list
		recv(SendSocketInfo->BuddyUpdateSocket,cHandShek,1,0);

		nBytesSent = send(SendSocketInfo->BuddyUpdateSocket,pNList,nLen,0);

		Sleep(10);
		/*recv(SendSocketInfo->BuddyUpdateSocket,cHandShek,1,0);*/
		int nlength  = strlen(SendSocketInfo->ClientName);
		nBytesSent = send(SendSocketInfo->BuddyUpdateSocket,SendSocketInfo->ClientName,nlength,0);
	}
	if (pParent)
	{
		pParent->lock.Unlock();
	}

	//if (pPnt != NULL)
	//{
	//AfxBeginThread(ClientExecution,pstrClientInfo);
	//}
	return 0;
}

void CServerDlg::OnBottonStop()
{
	// TODO: Add your control notification handler code here
	if (m_bStopStatus)
	{
		m_bStopStatus = FALSE;
	}
	sockaddr_in m_server;
	SOCKET selfsock = socket(AF_INET,SOCK_STREAM,0);
	SOCKET selfsock1 = socket(AF_INET,SOCK_STREAM,0);
	m_server.sin_family = AF_INET;
	m_server.sin_port = htons(5000);
	m_server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); 

	struct sockaddr FAR* server = (struct sockaddr FAR*)&m_server;
	struct sockaddr FAR* server1 = server;
	m_bChkBtnStat = FALSE;
	((CButton*)GetDlgItem(IDC_RADIO_RUNNING))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_RADIO_STOPPED))->SetCheck(BST_CHECKED);
	int connectiion=connect(selfsock,server,sizeof(m_server));
	if (connectiion != 0)
	{
		AfxMessageBox(_T("Stopping server failed..."));
		//AfxBeginThread(ServerStart,this,THREAD_PRIORITY_NORMAL);
		return;
	}
	connectiion=connect(selfsock1,server1,sizeof(m_server));
	if (connectiion != 0)
	{
		AfxMessageBox(_T("Stopping server failed..."));
		return;
	}
	send(selfsock,"$t0p",5,0);
	Sleep(10);
	if (m_nStartStopCheck > 0)
	{
		//called from start/stop accepting connections.
		m_nStartStopCheck -=1;
	}
	else
	{
		CString Log;
		SYSTEMTIME time;
		GetLocalTime(&time);
		if (m_bStSpBnClk)
		{
			Log.Format(_T("-> [%d/%d/%d (%d:%d:%d)] Server is running in saturation mode.\r\n"),time.wDay,time.wDayOfWeek,time.wYear,time.wHour,time.wMinute,time.wSecond);
			m_bStSpBnClk = FALSE;
		}
		else
		{
			Log.Format(_T("-> Server Stopped at %d:%d:%d on %d/%d/%d \r\n"),time.wHour,time.wMinute,time.wSecond,time.wDay,time.wDayOfWeek,time.wYear);
		}
		m_strServerLog+=Log;
		SendMessage(UC_DLGUPDATE,0,0);

	}
}


UINT ClientExecution (LPVOID pPnt)
{
	ClientInfo * pstrClientInfo = (ClientInfo *)pPnt;
	CServerDlg * pParent = (CServerDlg *)pstrClientInfo->pParent;
	POSITION pos;
	SYSTEMTIME time;
	char pReceiveBuff[4500];
	while (1)
	{
		int nReturnCode = recv(pstrClientInfo->ChatSocket,pReceiveBuff,4096,0);
		if (nReturnCode == SOCKET_ERROR || nReturnCode == 0)
		{
			//delete from list
			POSITION tempPos;
			if (ClientInfoStructureLst.IsEmpty())
			{
				return -1;
			}
			pos = ClientInfoStructureLst.GetHeadPosition();
			while (pos != NULL)
			{
				tempPos = pos;
				ClientInfo *SendSocketInfo = ClientInfoStructureLst.GetNext(pos);
				if (SendSocketInfo == NULL)
				{
					return -1;
				}
				if (SendSocketInfo->ClientIp == pstrClientInfo->ClientIp && SendSocketInfo->ChatPort == pstrClientInfo->ChatPort)
				{
					pParent->m_nBuddyNo -=1;
					CString Log;

					GetLocalTime(&time);
					Log.Format(_T("-> Connection closed from "));
					Log+=pstrClientInfo->ClientIp;
					CString temp,tempname;
					tempname += SendSocketInfo->ClientName; 
					temp.Format(_T(" Port No %d Name %s at %d:%d:%d on %d/%d/%d \r\n"),htons(pstrClientInfo->ChatPort),tempname.GetString(),time.wHour,time.wMinute,time.wSecond,time.wDay,time.wDayOfWeek,time.wYear);
					Log+=temp;
					pParent->m_strServerLog+=Log;
					pParent->PostMessage(UC_DLGUPDATE,0,0);
					pParent->lock.Lock();
					ClientInfoStructureLst.RemoveAt(tempPos);
					pParent->lock.Unlock();
					AfxBeginThread(BuddyUpdate,NULL);
					if (pParent->m_nSNotify == 1)
					{
						Beep(500,200);
					}
					return -1;
				}
			}
		}
		else
		{	//changed
			char SendBuff[4500] = "";
			/*CString strTime;
			GetLocalTime(&time);
			strTime.Format(_T("(%d:%d:%d) "),time.wHour,time.wMinute,time.wSecond);
			int nLen = strTime.GetLength();
			wcstombs(SendBuff,strTime,nLen);
			SendBuff[nLen]=NULL;*/
			strcat(SendBuff,pstrClientInfo->ClientName);
			strcat(SendBuff,": ");
			strcat(SendBuff,pReceiveBuff);

			pos = ClientInfoStructureLst.GetHeadPosition();
			while (pos != NULL)
			{
				ClientInfo *SendSocketInfo = ClientInfoStructureLst.GetNext(pos);
				if (SendSocketInfo != NULL)
				{
					send(SendSocketInfo->ChatSocket,SendBuff,4500,0);
				}
			}

		}

	}
	return 0;	
}
void CServerDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	POSITION pos;
	pos = ClientInfoStructureLst.GetHeadPosition();
	while (pos != NULL)
	{
		ClientInfo *SendSocketInfo = ClientInfoStructureLst.GetNext(pos);
		if (SendSocketInfo != NULL)
		{
			send(SendSocketInfo->ChatSocket,"&s^",4500,0);
			send(SendSocketInfo->BuddyUpdateSocket,"&s^",128,0);
		}
	}

	FILE * fp;

	static TCHAR BASED_CODE szFilter[] = _T("Text File (*.txt)");

	CFileDialog Filegld(FALSE,NULL,NULL,OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilter, this);
	//LPCSTR

	Filegld.DoModal();
	CString path = Filegld.GetPathName();
	if (path.GetLength()==0)
	{
		return;
	}
	path += _T(".txt");
	CFile file(path,CFile::modeCreate | CFile::modeWrite);
	file.Close();

	if ((fp=_wfopen(path,_T("a+")))==NULL)
	{
		AfxMessageBox(_T("Log file could not be opened"));
	}
	m_strServerLog +=_T("<<<<<<<<<< Server is terminated >>>>>>>>>>\r\n\r\n");
	fputws(m_strServerLog,fp);
	fclose(fp);

	OnOK();
}

void CServerDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	POSITION pos;
	pos = ClientInfoStructureLst.GetHeadPosition();
	while (pos != NULL)
	{
		ClientInfo *SendSocketInfo = ClientInfoStructureLst.GetNext(pos);
		if (SendSocketInfo != NULL)
		{
			send(SendSocketInfo->ChatSocket,"&s^",4500,0);
		}
		//send(SendSocketInfo->BuddyUpdateSocket,"&s^",128,0);
	}
	lock.Lock();
	ClientInfoStructureLst.RemoveAll();
	lock.Unlock();
	int i = AfxMessageBox(_T("ServerLog is not generated\r\nDo you want to generate ServerLog?\r\nTo generate ServerLog press yes"),MB_YESNO);
	if (i == 6)
	{
		CServerDlg::OnBnClickedOk();
	}

	OnCancel();
}

int CServerDlg::ShowTrayIcon()
{
	//Notification icon implemention in the system tray
	HWND hApplicationHandle = GetSafeHwnd();
	m_NotificationData.cbSize = sizeof(m_NotificationData);
	m_NotificationData.hIcon = m_hIcon;
	m_NotificationData.hWnd = hApplicationHandle;
	swprintf(m_NotificationData.szTip, _T("mindTalk - server") );
	m_NotificationData.uCallbackMessage = WM_ICON_MESSAGE;
	m_NotificationData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;

	Shell_NotifyIcon(NIM_ADD, &m_NotificationData);
	ShowWindow(SW_HIDE);
	return 0;
}

LRESULT CServerDlg::OnIconClick(WPARAM wParam,LPARAM lParam)
{
	switch(lParam)
	{
	case WM_LBUTTONDOWN:
		{
			ShowWindow(SW_NORMAL);
			SetForegroundWindow();
			Shell_NotifyIcon(NIM_DELETE, &m_NotificationData);
			break;
		}
	case WM_RBUTTONDOWN:
		{
			HMENU hMenu = CreatePopupMenu();
			AppendMenu(hMenu,MF_STRING,WM_SHOW,_T("Restore"));
			AppendMenu(hMenu,MF_SEPARATOR,NULL,_T(""));
			AppendMenu(hMenu, MF_STRING, ID_MENU_EXIT, _T("Exit"));
			CPoint pt;
			GetCursorPos(&pt);
			SetForegroundWindow();
			TrackPopupMenu(hMenu, 0, pt.x,pt.y,0,this->m_hWnd,NULL);
			break;
		}
	}
	return 0;
}

void CServerDlg::ExitCall()
{
	Shell_NotifyIcon(NIM_DELETE, &m_NotificationData);
	CDialog::OnOK();
}
void CServerDlg::OnShow()
{
	Shell_NotifyIcon(NIM_DELETE, &m_NotificationData);
	ShowWindow(SW_NORMAL);
	SetForegroundWindow();
}

void CServerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (nType == 1)
	{
		CServerDlg::ShowTrayIcon();
	}
}

BOOL CServerDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
void CServerDlg::OnBnClickedCheckSound()
{
	// TODO: Add your control notification handler code here
	static BOOL bToggle = TRUE;
	if (bToggle)
	{
		m_nSNotify = 1;
		bToggle = FALSE;
		CString Log;
		SYSTEMTIME time;
		GetLocalTime(&time);
		Log.Format(_T("-> Notification sound is switched on at %d:%d:%d on %d/%d/%d \r\n"),time.wHour,time.wMinute,time.wSecond,time.wDay,time.wDayOfWeek,time.wYear);
		m_strServerLog+=Log;
	}
	else
	{
		m_nSNotify = 0;
		bToggle = TRUE;
		CString Log;
		SYSTEMTIME time;
		GetLocalTime(&time);
		Log.Format(_T("-> Notification sound is switched off at %d:%d:%d on %d/%d/%d \r\n"),time.wHour,time.wMinute,time.wSecond,time.wDay,time.wDayOfWeek,time.wYear);
		m_strServerLog+=Log;
	}
	PostMessage(UC_DLGUPDATE,0,0);
}

void CServerDlg::OnBnClickedButtonStopAccepting()
{
	// TODO: Add your control notification handler code here
	m_bStSpBnClk = TRUE;
	if (m_bCheck)
	{
		if (m_bStopStatus)
		{
			OnBottonStop();
		}
		m_ctrlStopAccep.SetWindowText(_T("Start\r\nAccepting\r\nConnections"));
		m_bCheck = FALSE;
	}
	else
	{
		AfxBeginThread(ServerStart,this,THREAD_PRIORITY_NORMAL);
		m_bChkBtnStat = TRUE;
		((CButton*)GetDlgItem(IDC_RADIO_RUNNING))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_RADIO_STOPPED))->SetCheck(BST_UNCHECKED);
		m_ctrlStopAccep.SetWindowText(_T("Stop\r\nAccepting\r\nConnections"));
		m_bCheck = TRUE;
		m_bStopStatus = TRUE;
	}

}

void CServerDlg::OnBnClickedButtonReset()
{
	// TODO: Add your control notification handler code here
	POSITION pos;
	pos = ClientInfoStructureLst.GetHeadPosition();
	while (pos != NULL)
	{
		ClientInfo *SendSocketInfo = ClientInfoStructureLst.GetNext(pos);
		if (SendSocketInfo != NULL)
		{
			send(SendSocketInfo->ChatSocket,"&s^",4500,0);
		}
	}
	lock.Lock();
	ClientInfoStructureLst.RemoveAll();
	lock.Unlock();
	m_nBuddyNo = 0;
	if (m_nStartStopCheck > 0)
	{
		//called from start/stop accepting connections button.
		m_nStartStopCheck -= 1;
	}
	else
	{	
		CString Log;
		SYSTEMTIME time;
		GetLocalTime(&time);
		Log.Format(_T("-> Server Resetted at %d:%d:%d on %d/%d/%d \r\n"),time.wHour,time.wMinute,time.wSecond,time.wDay,time.wDayOfWeek,time.wYear);
		m_strServerLog+=Log;
		PostMessage(UC_DLGUPDATE,0,0);
	}
}

void CServerDlg::OnBnClickedRadioStopped()
{
	// TODO: Add your control notification handler code here
	if (!m_bChkBtnStat)
	{
		((CButton*)GetDlgItem(IDC_RADIO_STOPPED))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_RADIO_RUNNING))->SetCheck(BST_UNCHECKED);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_STOPPED))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_RADIO_RUNNING))->SetCheck(BST_CHECKED);
	}
}

void CServerDlg::OnBnClickedRadioRunning()
{
	// TODO: Add your control notification handler code here
	if (!m_bChkBtnStat)
	{
		((CButton*)GetDlgItem(IDC_RADIO_RUNNING))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_RADIO_STOPPED))->SetCheck(BST_CHECKED);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_RUNNING))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_RADIO_STOPPED))->SetCheck(BST_UNCHECKED);
	}

}
