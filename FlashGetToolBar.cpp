// FlashGetToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "FlashGetToolBar.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlashGetToolBar

CFlashGetToolBar::CFlashGetToolBar()
{
}

CFlashGetToolBar::~CFlashGetToolBar()
{
}


BEGIN_MESSAGE_MAP(CFlashGetToolBar, CToolBar)
	//{{AFX_MSG_MAP(CFlashGetToolBar)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlashGetToolBar message handlers


void CFlashGetToolBar::ResizeBar(int cx)
{
	if (m_ctlDownloadSpeed.GetSafeHwnd())
	{
		CRect rectBar;

		GetWindowRect(&rectBar);
		ScreenToClient(&rectBar);
		rectBar.left = 0;
		rectBar.right = rectBar.left + cx - 2;
		MoveWindow(&rectBar);

		CRect rectSpeed;
		rectSpeed = rectBar;
		rectSpeed.right = cx - 2;
		rectSpeed.left = rectSpeed.right - 125;
		m_ctlDownloadSpeed.MoveWindow(&rectSpeed);

	}
}

void CFlashGetToolBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (GetToolBarCtrl().HitTest(&point) >= 0)
	{
		CToolBar::OnLButtonDown(nFlags, point);
	}
}

void CFlashGetToolBar::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
//	CToolBar::OnLButtonDblClk(nFlags, point);
}

void CFlashGetToolBar::SetSpeed(float fSpeedByKB)
{
	m_ctlDownloadSpeed.SetPos((UINT)(fSpeedByKB * 100 / 8));
}


int CFlashGetToolBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CRect rect;
	rect.left = 0;
	rect.right = 125;
	rect.top  = 0;
	rect.bottom = 40;

	m_ctlDownloadSpeed.Create(WS_VISIBLE | WS_CHILD, rect, this, IDC_SPEEDBAR);
	m_ctlDownloadSpeed.SetRange(0, 100);

	return 0;
}
