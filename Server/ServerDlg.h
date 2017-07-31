// GroupChatServerDlg.h : header file
//

#pragma once

#define UC_DLGUPDATE WM_USER+1
#define WM_ICON_MESSAGE WM_USER+2
#define  ID_MENU_EXIT WM_USER+3
#define WM_SHOW WM_USER+4

struct ClientInfo {
	SOCKET ChatSocket;
	SOCKET BuddyUpdateSocket;
	char ClientName[128];
	char ClientIp[20];
	int ChatPort;
	int ByddyUpdatePort;
	CDialog * pParent;
};

// CServerDlg dialog
class CServerDlg : public CDialog
{
// Construction
public:
	CServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bStartStop;
	int m_nMode;
	int m_nBuddyNo;
	int m_ctrlRunning;
	int m_ctrlStopped;
	int m_nSNotify;
	CString m_strServerLog;
	struct sockaddr FAR* m_clientadd;
	WORD m_VersionNo;
	WSADATA m_WSAdata;
	SOCKET sock;
	int chk;
	CCriticalSection lock;
	BOOL m_bStSpBnClk;
	BOOL m_bStopStatus;
	BOOL m_bCheck;
	BOOL m_bChkBtnStat;
	int m_nStartStopCheck;
	afx_msg void OnBnClickedButtonStart();
	virtual LRESULT UpdateDialog(WPARAM wp = 0,LPARAM lp = 0);
	//afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
protected:
	virtual void OnBottonStop();
	virtual void OnShow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT OnIconClick(WPARAM wParam = 0,LPARAM lParam = 0);
	virtual void ExitCall();
	int ShowTrayIcon();
	NOTIFYICONDATA m_NotificationData;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_ctrlStartStop;
	afx_msg void OnBnClickedCheckSound();
	afx_msg void OnBnClickedButtonStopAccepting();
	CButton m_ctrlStopAccep;
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedRadioStopped();
	afx_msg void OnBnClickedRadioRunning();
};
