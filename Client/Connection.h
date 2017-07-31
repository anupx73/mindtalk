#pragma once

class CConnection	{

public:
	CConnection();
	~CConnection();

	char *m_cSendReceiveBuff;

	struct sockaddr_in m_server;
	SOCKET m_sClientSoc;
	SOCKET m_sBuddyUpdateSoc;
	struct sockaddr FAR* server;
	struct sockaddr FAR* bserver;

	int InitSocket();
	int ConnectServer(char* sIP, char* bName);
	int SendChat(char* sChat, int ilen);
	int CloseConnection();
};
extern CConnection cCon;

UINT GetChatThread(LPVOID lP);
UINT BuddyUpdateThread (LPVOID lP);
extern HANDLE hSignal;