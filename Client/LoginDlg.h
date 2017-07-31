#pragma once
#include "afxwin.h"

class CLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoginDlg();

// Dialog Data
	enum { IDD = IDD_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	LRESULT WindowProc(UINT, WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLogin();
	CString m_csBuddyName;
	CString m_csServerIP;
	afx_msg void OnClose();

	afx_msg void OnContextQuit();
	afx_msg void OnContextAbout();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);

	HKEY hRun, hOpen;
	CButton m_WinStart;
	CButton m_AutoLogin;

	afx_msg void OnPaint();
	//Image *Img;
};
extern CLoginDlg oLgDlg;

extern CString csServer;
extern CString csNick;