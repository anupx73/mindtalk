// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "AboutDlg.h"
// CAboutDlg dialog

IMPLEMENT_DYNAMIC(CAboutDlg, CDialog)

CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
{

}

//Only object;
CAboutDlg oAbDlg;

CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CAboutDlg message handlers
void CAboutDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	Rect rect(5,12,85,77);
	Graphics grpx(dc);

	Image *Img = Image::FromFile(L"logo.png",FALSE);
	grpx.DrawImage(Img,rect);

	//CDialog::OnPaint();
}

void CAboutDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_CLOSE);
}

BOOL CAboutDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			CDialog::OnClose();
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}