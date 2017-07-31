// GroupChatClientDlg.h : header file
//

#pragma once

#define UWM_UPDATE (WM_USER + 1)
#define UWM_CLOSE (WM_USER + 2)
#define UWM_NICKUPDATE	(WM_USER + 3)

class CClientDlg : public CDialog
{
public:
	CClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CHAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	//HICON m_hIcon;
	virtual BOOL PreTranslateMessage(MSG*);
	afx_msg void OnClose();
	virtual LRESULT WindowProc(UINT, WPARAM, LPARAM);

public:
	CString m_strBuddyList;
	CString m_strChatText;
	CString m_strUserInput;
	CStatic m_header_Nick;
	CStatic m_header_IP;	

	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedSaveLog();
	afx_msg void OnContextShowHide();
	afx_msg void OnContextAbout();
	afx_msg void OnContextQuit();
	afx_msg void OnPaint();

	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg LRESULT UpdateChatBuddy(WPARAM, LPARAM);
	afx_msg LRESULT UpdateNickName(WPARAM, LPARAM);
	CString m_strNickName;
};

extern CClientDlg oClDlg;