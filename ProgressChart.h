#if !defined(AFX_CHECKERCTRL_H__AD4BC4E3_7750_11D4_8718_008048CA4A77__INCLUDED_)
#define AFX_CHECKERCTRL_H__AD4BC4E3_7750_11D4_8718_008048CA4A77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CheckerCtrl.h : header file
//

////////////////////////////////////////////////////////////////////////////
// CProgressChart window

//define Progress Chart constant value
#define BLOCK_HEIGHT			11
#define BLOCK_WIDTH				10

#define BLOCK_BYTE_SIZE			(4 * 1024)

#define BLOCK_UNCOMPLETED		0
#define BLOCK_COMPLETED			1
#define BLOCK_DOWNLOADING		2

class CProgressChart : public CWnd
{
// Construction
public:
	CProgressChart();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressChart)
	//}}AFX_VIRTUAL

// Implementation
public:
	void ResetBlocks();
	int GetTotalBlocks();
	void Resize();
	void SetTotalBlocks(const UINT nNumberofBlocks, const UINT nStartIndex = 0);
	void Update(const UINT nBlockNumber);
	void Reset();
	void SetBlock(const UINT nBlockNumber, DWORD dwBlockStatus);
	COLORREF GetBlock(const UINT nBlockNumber) const;
	BOOL Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID);
	virtual ~CProgressChart();

	// Generated message map functions
protected:
	CRect GetBlockRect(const UINT nRow, const UINT nColumn);
	const CPoint GetLefTopPointOfBlock(const UINT nRow, const UINT nColumn);
	CImageList m_BlockStatusImageList;
	//{{AFX_MSG(CProgressChart)
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL IsVisible(const UINT nBlockNumber);
	void ShowBlock(UINT nRow, UINT nColumn, UINT iBlockStatus, CDC &dc);

	CWnd	*m_pParentWnd;
	UINT	m_nID;
	UINT	m_nStartIndex;
	UINT	m_nTotalVisibleBlocks;
	UINT	m_nBlockStartPos;
	int		m_nyPos; 
	int		m_nOffset;
	CRect	m_rcClient;
	UINT		m_nBlocksPerRow;
	UINT		m_nBlocksPerColumn;
	UINT		m_nNumberofBlocks;
	CBrush	m_backgroundBrush;
	CArray	<UINT, UINT> m_arBlockStatus;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKERCTRL_H__AD4BC4E3_7750_11D4_8718_008048CA4A77__INCLUDED_)
