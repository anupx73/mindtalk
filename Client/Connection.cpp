#include "stdafx.h"
#include "Client.h"
#include "Connection.h"
#include "ClientDlg.h"
#include "TrayIcon.h"

CConnection cCon;
HANDLE hSignal;

CConnection::CConnection()
{}

CConnection::~CConnection()
{}

int CConnection::InitSocket()
{
	m_sClientSoc = socket(AF_INET,SOCK_STREAM,0);
	m_sBuddyUpdateSoc = socket(AF_INET,SOCK_STREAM,0);
	
	if (m_sClientSoc == INVALID_SOCKET || m_sBuddyUpdateSoc == INVALID_SOCKET)
		return -1;

	m_cSendReceiveBuff = new char[4096];

	return 0;
}

int CConnection::ConnectServer(char* sIP, char* bName)
{

	struct hostent *hptr; 
	//char abc[128];
	
	hptr = gethostbyname(sIP);
	
	//memcpy( &sockAddrDest.sin_addr, hptr->h_addr, hptr->h_length );


	m_server.sin_family = AF_INET;
	m_server.sin_port = htons(5000);
	m_server.sin_addr.S_un.S_addr = inet_addr(sIP); 
	memcpy( &m_server.sin_addr, hptr->h_addr, hptr->h_length );
	server = (struct sockaddr FAR*)&m_server;
	bserver = server;

	if (connect(m_sClientSoc,server,sizeof(m_server)) == SOCKET_ERROR)
		return -3;
	
	if (connect(m_sBuddyUpdateSoc,bserver,sizeof(m_server)) == SOCKET_ERROR)
		return -2;

	if (send(m_sClientSoc, bName, 1024,0) == SOCKET_ERROR)
		return -1;

	hSignal = ::CreateEvent(0, TRUE, FALSE, 0);
	return 0;
}

int CConnection::SendChat(char* sChat, int ilen)
{
	send(m_sClientSoc,sChat,ilen,0);
	
	return 0;
}

int CConnection::CloseConnection()
{
	closesocket(m_sClientSoc);
	closesocket(m_sBuddyUpdateSoc);
	
	::SetEvent(hSignal);
	
	return 0;
}

UINT GetChatThread(LPVOID lP)
{
	CClientDlg *pParent = (CClientDlg *)lP;
	char ReceiveBuff[4500];
	TCHAR *tcBuffer;

	while (WaitForSingleObject(hSignal, 0) != WAIT_OBJECT_0)
	{
		int nRcvBufLen = recv(cCon.m_sClientSoc,ReceiveBuff,4500, 0);

		if (strcmp(ReceiveBuff,"&s^") == 0)
		{
			//TRACE(_T("Server Closed.."));
			pParent->PostMessageW(UWM_CLOSE, 0, 0);
			return -2;
		}

		if (nRcvBufLen > 0)
		{
			tcBuffer = new TCHAR[nRcvBufLen+1];
			mbstowcs(tcBuffer, ReceiveBuff, nRcvBufLen+1);
			CString strTemp;
			strTemp.Format(_T("%s\r\n"), tcBuffer);
			pParent->m_strChatText += strTemp;
			pParent->PostMessage(UWM_UPDATE,0,0);
			
			//TRACE(_T("Msg Posted.."));
			if (!pParent->IsWindowVisible())
				cTray.StartBlinkTrayIcon(600);
		}

	}
	return 0;
}

UINT BuddyUpdateThread (LPVOID lP)
{
	CClientDlg *pParent = (CClientDlg *)lP;
	char ReceiveBuff[128];
	TCHAR *tcBuffer;
	TCHAR tcNickName[128];
	while(WaitForSingleObject(hSignal, 0) != WAIT_OBJECT_0)
	{
		recv(cCon.m_sBuddyUpdateSoc,ReceiveBuff,128, 0);
		
		if (strcmp(ReceiveBuff,"GRP")==0)
		{
			send(cCon.m_sBuddyUpdateSoc,"",1,0);
			recv(cCon.m_sBuddyUpdateSoc,ReceiveBuff,128,0);

			int nLen = atoi(ReceiveBuff);
			tcBuffer = new TCHAR[nLen+1];
			
			char *BuddyBuf = new char[nLen+1];

			send(cCon.m_sBuddyUpdateSoc,"",1,0);
			recv(cCon.m_sBuddyUpdateSoc,BuddyBuf,nLen+1, 0);

			mbstowcs(tcBuffer, BuddyBuf, nLen+1);
			pParent->m_strBuddyList.Format(_T("%s"), tcBuffer);
			
			//send(cCon.m_sBuddyUpdateSoc,"",1,0);
			Sleep(10);
			recv(cCon.m_sBuddyUpdateSoc,ReceiveBuff,128, 0);
			nLen = strlen(ReceiveBuff);
			mbstowcs(tcNickName, ReceiveBuff, nLen);
			tcNickName[nLen] = NULL;
			pParent->m_strNickName.Format(_T("Your Nick : "));
			pParent->m_strNickName += tcNickName;
			pParent->PostMessage(UWM_NICKUPDATE,0,0);
			pParent->PostMessage(UWM_UPDATE,0,0);
		}
	}

	return 0;
}