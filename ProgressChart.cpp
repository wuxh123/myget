//Author:			Mehdi Mousavi
//Data of release:	8th of September, 2000
//Email:			Webmaster@modemmania.com
//					Mehdi_mousavi@hotmail.com

// CheckerCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ProgressChart.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressChart




CProgressChart::CProgressChart()
{
	//Sets the background brush of the client area
	m_backgroundBrush.CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
	
	//Resets m_nyPos for scrolling purposes
	m_nyPos = 0;

	//Resets m_nBlockStartPos so that when the WM_PAINT message
	//is triggered, the control starts to show blocks
	//from m_nBlockStartPos position
	m_nBlockStartPos = 0;

	//Sets the starting index of blocks
	m_nStartIndex = 0;

	//Total number of blocks
	m_nNumberofBlocks = 0;

	//Offset for scrolling purposes
	m_nOffset = 0;

	m_nTotalVisibleBlocks = 0;

	m_nBlocksPerRow = 0;


	CBitmap bitmap;
	VERIFY(bitmap.LoadBitmap(IDB_BLOCK_STATUS_BMP) != NULL);
	
	m_BlockStatusImageList.Create(BLOCK_WIDTH, BLOCK_HEIGHT, ILC_COLOR8 | ILC_MASK, 3, 1);

	m_BlockStatusImageList.Add(&bitmap, CLR_DEFAULT);
}

CProgressChart::~CProgressChart()
{
}

BOOL CProgressChart::Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	//Postcondition:
	//	Creates a window after being registered, as well as
	//	setting all the required variables used hereinafter.
	static CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW);
	//BOOL bRet = CWnd::CreateEx(WS_EX_CLIENTEDGE, 
	BOOL bRet = CWnd::CreateEx(NULL, //WS_VISIBLE | WS_DISABLED | WS_CHILD , 
								className,
								NULL,
								dwStyle,
								rect.left,
								rect.top,
								rect.right - rect.left,
								rect.bottom - rect.top,
								pParentWnd->GetSafeHwnd(),
								(HMENU) nID);

	m_nID = nID;
	m_pParentWnd = pParentWnd;
	
	
	return bRet;
}


BEGIN_MESSAGE_MAP(CProgressChart, CWnd)
	//{{AFX_MSG_MAP(CProgressChart)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CProgressChart message handlers

void CProgressChart::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	CRect rcInvalidate;
	dc.GetClipBox(&rcInvalidate);

	CRect rcRepaint;
	rcRepaint.IntersectRect(rcInvalidate, m_rcClient);

	//Fill the background color of the client area
	dc.FillRect(rcRepaint, &m_backgroundBrush);

	UINT nColumn = 0, nRow = 0;
	
	//Calculate the index of the last visible block
	//within the client area
	UINT nBlockEndPos = m_nBlockStartPos + m_nTotalVisibleBlocks + m_nBlocksPerRow;
	if(nBlockEndPos > m_nNumberofBlocks)
		nBlockEndPos = m_nNumberofBlocks;

	CRect rcBlock;
	for(UINT i = m_nBlockStartPos; i < nBlockEndPos; i++)
	{
		if (rcBlock.IntersectRect(rcRepaint, GetBlockRect(nRow, nColumn)))
		{
			ShowBlock(nRow, nColumn, m_arBlockStatus.GetAt(i), dc);
		}
	
		if((i + 1 - m_nBlockStartPos) % m_nBlocksPerRow == 0)
		{
			nRow++;
			nColumn = 0;
		}
		else
			nColumn++;
	}


	CWnd::OnPaint();
}


void CProgressChart::Resize()
{

	if (GetSafeHwnd() == NULL)
	{
		return;

	}

	CRect rcParentClient;
	m_pParentWnd->GetClientRect(&rcParentClient);

	MoveWindow(&rcParentClient);
	
	GetClientRect(m_rcClient);


	m_nBlocksPerRow = (m_rcClient.Width())/ BLOCK_WIDTH;
	m_nBlocksPerColumn = m_rcClient.Height() / BLOCK_HEIGHT;
	
	m_nTotalVisibleBlocks = m_nBlocksPerRow * m_nBlocksPerColumn;
	
	//Calculate the vertical scroll bar's range
	int nOffset = (m_nNumberofBlocks / m_nBlocksPerRow);
	if(m_nNumberofBlocks % m_nBlocksPerRow != 0)
		nOffset++;
	
	m_nOffset = nOffset - m_nBlocksPerColumn;
	if(m_nOffset > 0)
	{		
		EnableScrollBar(SB_VERT, ESB_ENABLE_BOTH);

		m_nBlockStartPos = 0;
		m_nyPos = 0;

		SCROLLINFO scrollInfo;
		scrollInfo.cbSize	= sizeof(SCROLLINFO);
		scrollInfo.fMask	= SIF_ALL;
		scrollInfo.nMax		= nOffset;
		scrollInfo.nMin		= 0;
		scrollInfo.nPage	= m_nBlocksPerColumn;
		scrollInfo.nPos		= 0;
		scrollInfo.nTrackPos= 0;

		SetScrollInfo(SB_VERT, &scrollInfo);
	}
	else
	{
		EnableScrollBar(SB_VERT, ESB_DISABLE_BOTH);
	}
}

void CProgressChart::SetTotalBlocks(const UINT nNumberofBlocks, const UINT nStartIndex)
{

	m_nNumberofBlocks = nNumberofBlocks;

	m_arBlockStatus.SetSize(m_nNumberofBlocks);

	for(UINT i = 0; i < m_nNumberofBlocks; i++)
	{
		m_arBlockStatus.SetAt(i, BLOCK_UNCOMPLETED);
	}

	//	Sets the starting index of blocks
	m_nStartIndex = nStartIndex;

	Resize();
	Invalidate();

}

void CProgressChart::ShowBlock(UINT nRow, UINT nColumn, UINT i, CDC &dc)
{
	//Postcondition:
	//	Places a block status image on nRow, nColumn ordered pair
	
	m_BlockStatusImageList.Draw(&dc, i, GetLefTopPointOfBlock(nRow, nColumn), ILD_NORMAL);
}

void CProgressChart::SetBlock(const UINT nBlockNumber, DWORD dwBlockStatus)
{
	//Postcondition:
	//	Sets the color of a specified block number to crColor
	ASSERT(nBlockNumber - m_nStartIndex < m_nNumberofBlocks);

	if (m_arBlockStatus.GetAt(nBlockNumber - m_nStartIndex) != dwBlockStatus)
	{
		m_arBlockStatus.SetAt(nBlockNumber - m_nStartIndex, dwBlockStatus);

		Update(nBlockNumber - m_nStartIndex);
	}
}

void CProgressChart::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

	int nyOffset;
	switch(nSBCode)
	{
	case SB_PAGEDOWN:
		if(m_nyPos < (int)m_nOffset)
		{
			if ((int)(m_nyPos + m_nBlocksPerColumn) < (int)m_nOffset)
			{
				nyOffset = m_nBlocksPerColumn;
			}
			else
			{
				nyOffset = m_nOffset - m_nyPos;
			}

			m_nyPos += nyOffset;
			m_nBlockStartPos += m_nBlocksPerRow * nyOffset;

			SetScrollPos(SB_VERT, m_nyPos);
			ScrollWindow(0, -nyOffset * BLOCK_HEIGHT);
		}
		break;

	case SB_PAGEUP:
		if(m_nyPos > 0)
		{
			if (int (m_nyPos) - int(m_nBlocksPerColumn) >0)
			{
				nyOffset = m_nBlocksPerColumn;
			}
			else
			{
				nyOffset = m_nyPos;
			}


			m_nBlockStartPos -= m_nBlocksPerRow * nyOffset;


			m_nyPos -= nyOffset;
			SetScrollPos(SB_VERT, m_nyPos);
			ScrollWindow(0, nyOffset * BLOCK_HEIGHT);
		}
		break;

	case SB_LINEUP:
		if(m_nyPos > 0)
		{
			m_nBlockStartPos -= m_nBlocksPerRow;

			m_nyPos--;
			SetScrollPos(SB_VERT, m_nyPos);
			ScrollWindow(0, BLOCK_HEIGHT);
		}
		break;

	case SB_LINEDOWN:
		if(m_nyPos < m_nOffset)
		{
			m_nBlockStartPos += m_nBlocksPerRow;

			m_nyPos++;
			SetScrollPos(SB_VERT, m_nyPos);
			ScrollWindow(0, -BLOCK_HEIGHT);
		}
		break;

	case SB_THUMBTRACK:
		if (m_nOffset >= (int)nPos)
		{
			nyOffset = m_nyPos - nPos;
			m_nyPos = nPos;
			m_nBlockStartPos += -nyOffset *  m_nBlocksPerRow;
			SetScrollPos(SB_VERT, m_nyPos);
			ScrollWindow(0, nyOffset * BLOCK_HEIGHT);
		}
		break;
	}

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);


}



COLORREF CProgressChart::GetBlock(const UINT nBlockNumber) const
{
	//Precondition:
	//	nBlockNumber must be in the range of the defined blocks
	//Postcondition:
	//	Takes the color of the specified index
	ASSERT(nBlockNumber >= m_nStartIndex && nBlockNumber - m_nStartIndex <= m_nNumberofBlocks);
	return m_arBlockStatus.GetAt(nBlockNumber - m_nStartIndex);
}


const CPoint CProgressChart::GetLefTopPointOfBlock(const UINT nRow, const UINT nColumn)
{
	CPoint pt;
	pt.x = nColumn * BLOCK_WIDTH;
	pt.y = nRow * BLOCK_HEIGHT;
	
	return pt;
}

BOOL CProgressChart::IsVisible(const UINT nBlockNumber)
{
	//Calculate the index of the last visible block
	//within the client area
	UINT nBlockEndPos = m_nBlockStartPos + m_nTotalVisibleBlocks + m_nBlocksPerRow;
	if(nBlockEndPos > m_nNumberofBlocks)
		nBlockEndPos = m_nNumberofBlocks;

	if(nBlockNumber >= m_nBlockStartPos && nBlockNumber <= nBlockEndPos)
		return TRUE;
	else
		return FALSE;
}



void CProgressChart::Reset()
{
	for(UINT i = 0; i < m_nNumberofBlocks; i++)
		m_arBlockStatus.SetAt(i, BLOCK_UNCOMPLETED);
}

void CProgressChart::Update(const UINT nBlockNumber)
{
	//Precondition:
	//	nBlockNumber must be in the range of the defined blocks
	//Postcondition:
	//	Updates the color of a specified index on CRT, if and only
	//	if it's already visible
	ASSERT(nBlockNumber >= m_nStartIndex && nBlockNumber - m_nStartIndex <= m_nNumberofBlocks);

	if(IsVisible(nBlockNumber))
	{
		UINT nRow = (nBlockNumber - m_nStartIndex) / m_nBlocksPerRow - m_nyPos;
		UINT nColumn = (nBlockNumber - m_nStartIndex) % m_nBlocksPerRow;
		
		CClientDC dc(this);

		ShowBlock(nRow, nColumn, m_arBlockStatus.GetAt(nBlockNumber - m_nStartIndex), dc);
	}
}



void CProgressChart::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetCursor(::LoadCursor(NULL, IDC_ARROW));
	CWnd::OnMouseMove(nFlags, point);
}





CRect CProgressChart::GetBlockRect(const UINT nRow, const UINT nColumn)
{
	//Postcondition:
	//	Returns the rectangular area of a specified block
	//	placed in nRow(th) row and nColumn(th) column.
	CRect rect;
	rect.left = nColumn * BLOCK_WIDTH;
	rect.top = nRow * BLOCK_HEIGHT;
	rect.right = rect.left + BLOCK_WIDTH;
	rect.bottom = rect.top + BLOCK_HEIGHT;
	
	return rect;
}


int CProgressChart::GetTotalBlocks()
{
	return m_nNumberofBlocks;
}

void CProgressChart::ResetBlocks()
{
	for(UINT i = 0; i < m_nNumberofBlocks; i++)
	{
		m_arBlockStatus.SetAt(i, BLOCK_UNCOMPLETED);
	}

}
