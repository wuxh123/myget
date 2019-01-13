//***************************************************************************
//
// AUTHOR:  James White (feel free to remove or otherwise mangle any part)
//
//***************************************************************************
#include "stdafx.h"
#include "ColorButton.h"

//***********************************************************************
//**                         MFC Debug Symbols                         **
//***********************************************************************
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//***********************************************************************
//**                            DDX Method                            **
//***********************************************************************

//I do not know when this functio will be called.
void AFXAPI DDX_ColorButton(CDataExchange *pDX, int nIDC, COLORREF& crColour)
{
    HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
    ASSERT (hWndCtrl != NULL);                
    
    CColorButton* pColourButton = (CColorButton*) CWnd::FromHandle(hWndCtrl);
    if (pDX->m_bSaveAndValidate)
    {
		crColour = pColourButton->BackColor;
    }
    else // initializing
    {
		pColourButton->BackColor = crColour;
    }
}

//***********************************************************************
//**                             Constants                             **
//***********************************************************************
const int g_ciArrowSizeX = 6 ;
const int g_ciArrowSizeY = 3 ;

//***********************************************************************
//**                            MFC Macros                            **
//***********************************************************************
IMPLEMENT_DYNCREATE(CColorButton, CButton)

//***********************************************************************
// Method:	CColorButton::CColorButton(void)
// Notes:	Default Constructor.
//***********************************************************************
CColorButton::CColorButton(void):
	_Inherited(),
	m_BackColor(CLR_DEFAULT),
	m_DefaultColor(::GetSysColor(COLOR_APPWORKSPACE)),
	m_strDefaultText(_T("Automatic")),
	m_strCustomText(_T("More Colors...")),
	m_strText(_T("")),
	m_bPopupActive(FALSE),
	m_bTrackSelection(TRUE),
	m_bIsSetBkColor(TRUE)
{
}

//***********************************************************************
// Method:	CColorButton::~CColorButton(void)
// Notes:	Destructor.
//***********************************************************************
CColorButton::~CColorButton(void)
{
}

//***********************************************************************
// Method:	CColorButton::GetBackColor()
// Notes:	None.
//***********************************************************************
COLORREF CColorButton::GetBackColor(void) const
{
	return m_BackColor;
}


//***********************************************************************
// Method:	CColorButton::SetBackColor()
// Notes:	None.
//***********************************************************************
void CColorButton::SetBackColor(COLORREF Color)
{
	m_BackColor = Color;

	if (::IsWindow(m_hWnd)) 
        RedrawWindow();
}

//***********************************************************************
// Method:	CColorButton::GetForeColor()
// Notes:	None.
//***********************************************************************
COLORREF CColorButton::GetForeColor(void) const
{
	return m_ForeColor;
}


//***********************************************************************
// Method:	CColorButton::SetForeColor()
// Notes:	None.
//***********************************************************************
void CColorButton::SetForeColor(COLORREF Color)
{
	m_ForeColor = Color;

	if (::IsWindow(m_hWnd)) 
        RedrawWindow();
}

//***********************************************************************
// Method:	CColorButton::GetDefaultColor()
// Notes:	None.
//***********************************************************************
COLORREF CColorButton::GetDefaultColor(void) const
{
	return m_DefaultColor;
}

//***********************************************************************
// Method:	CColorButton::SetDefaultColor()
// Notes:	None.
//***********************************************************************
void CColorButton::SetDefaultColor(COLORREF Color)
{
	m_DefaultColor = Color;
}

//***********************************************************************
// Method:	CColorButton::SetCustomText()
// Notes:	None.
//***********************************************************************
void CColorButton::SetCustomText(LPCTSTR tszText)
{
	m_strCustomText = tszText;
}

//***********************************************************************
// Method:	CColorButton::SetDefaultText()
// Notes:	None.
//***********************************************************************
void CColorButton::SetDefaultText(LPCTSTR tszText)
{
	m_strDefaultText = tszText;
}


//***********************************************************************
// Method:	CColorButton::SetTrackSelection()
// Notes:	None.
//***********************************************************************
void CColorButton::SetTrackSelection(BOOL bTrack)
{
	m_bTrackSelection = bTrack;
}

//***********************************************************************
// Method:	CColorButton::GetTrackSelection()
// Notes:	None.
//***********************************************************************
BOOL CColorButton::GetTrackSelection(void) const
{
	return m_bTrackSelection;
}

//***********************************************************************
//**                         CButton Overrides                         **
//***********************************************************************
void CColorButton::PreSubclassWindow() 
{
    ModifyStyle(0, BS_OWNERDRAW);      

    _Inherited::PreSubclassWindow();
}

//***********************************************************************
//**                         Message Handlers                         **
//***********************************************************************
BEGIN_MESSAGE_MAP(CColorButton, CButton)
    //{{AFX_MSG_MAP(CColorButton)
    ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
    ON_MESSAGE(CPN_SELENDOK,     OnSelEndOK)
    ON_MESSAGE(CPN_SELENDCANCEL, OnSelEndCancel)
    ON_MESSAGE(CPN_SELCHANGE,    OnSelChange)
END_MESSAGE_MAP()


//***********************************************************************
// Method:	CColorButton::OnSelEndOK()
// Notes:	None.
//***********************************************************************
LONG CColorButton::OnSelEndOK(UINT lParam, LONG /*wParam*/)
{
	m_bPopupActive = FALSE;

	COLORREF OldColor;
	if (m_bIsSetBkColor)
	{
		OldColor = m_BackColor;
	
		BackColor = (COLORREF)lParam;
	}
	else
	{
		OldColor = m_ForeColor;
	
		ForeColor = (COLORREF)lParam;
	}

    CWnd *pParent = GetParent();

    if (pParent) 
	{
        pParent->SendMessage(CPN_CLOSEUP, lParam, (WPARAM) GetDlgCtrlID());
        pParent->SendMessage(CPN_SELENDOK, lParam, (WPARAM) GetDlgCtrlID());
    }

	if (m_bIsSetBkColor)
	{
		if (OldColor != m_BackColor)
		{
			if (pParent) pParent->SendMessage(CPN_SELCHANGE, m_BackColor, (WPARAM) GetDlgCtrlID());
		}
	}
	else
	{
		if (OldColor != m_ForeColor)
		{
			if (pParent) pParent->SendMessage(CPN_SELCHANGE, m_ForeColor, (WPARAM) GetDlgCtrlID());
		}
	}

    return TRUE;
}


//***********************************************************************
// Method:	CColorButton::OnSelEndCancel()
// Notes:	None.
//***********************************************************************
LONG CColorButton::OnSelEndCancel(UINT lParam, LONG /*wParam*/)
{
	m_bPopupActive = FALSE;

	if (m_bIsSetBkColor)
	{
		BackColor = (COLORREF)lParam;
	}
	else
	{
		ForeColor = (COLORREF)lParam;
	}

    CWnd *pParent = GetParent();

    if (pParent) 
	{
        pParent->SendMessage(CPN_CLOSEUP, lParam, (WPARAM) GetDlgCtrlID());
        pParent->SendMessage(CPN_SELENDCANCEL, lParam, (WPARAM) GetDlgCtrlID());
    }

    return TRUE;
}


//***********************************************************************
// Method:	CColorButton::OnSelChange()
// Notes:	None.
//***********************************************************************
LONG CColorButton::OnSelChange(UINT lParam, LONG /*wParam*/)
{
    CWnd *pParent = GetParent();

	if (m_bTrackSelection) 
	{
		if (m_bIsSetBkColor)
		{
			BackColor = (COLORREF)lParam;
		}
		else
		{
			ForeColor = (COLORREF)lParam;
		}
	}

    

    if (pParent) pParent->SendMessage(CPN_SELCHANGE, lParam, (WPARAM) GetDlgCtrlID());

    return TRUE;
}

//***********************************************************************
// Method:	CColorButton::OnCreate()
// Notes:	None.
//***********************************************************************
int CColorButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CButton::OnCreate(lpCreateStruct) == -1)
        return -1;

    return 0;
}

//***********************************************************************
// Method:	CColorButton::OnClicked()
// Notes:	None.
//***********************************************************************
BOOL CColorButton::OnClicked()
{
	m_bPopupActive = TRUE;

    CRect rDraw;
    GetWindowRect(rDraw);
	
	COLORREF *pColor;
	
	if (m_bIsSetBkColor)
	{
		pColor = &m_BackColor;
	}
	else
	{
		pColor = &m_ForeColor;
	}

    new CColourPopup(CPoint(rDraw.left, rDraw.bottom),		// Point to display popup
                     *pColor,								// Selected colour
                     this,									// parent
                     m_strDefaultText,						// "Default" text area
                     m_strCustomText);						// Custom Text

    CWnd *pParent = GetParent();

    if (pParent)
        pParent->SendMessage(CPN_DROPDOWN, (LPARAM)*pColor, (WPARAM) GetDlgCtrlID());

    return TRUE;
}



//***********************************************************************
// Method:	CColorButton::DrawItem()
// Notes:	None.
//***********************************************************************
void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	ASSERT(lpDrawItemStruct);

	CDC*    pDC      = CDC::FromHandle(lpDrawItemStruct->hDC);
	UINT    state    = lpDrawItemStruct->itemState;
	CRect	rItem	 = lpDrawItemStruct->rcItem;
    CRect   rClient;	 
	CRect	rArrow;

	if (m_bPopupActive)
		state |= ODS_SELECTED|ODS_FOCUS;

	//******************************************************
	//**                  Draw Outer Edge
	//******************************************************
	UINT uFrameState = DFCS_BUTTONPUSH|DFCS_ADJUSTRECT;

	if (state & ODS_SELECTED)
		uFrameState |= DFCS_PUSHED;

	if (state & ODS_DISABLED)
		uFrameState |= DFCS_INACTIVE;
	
	rItem.bottom = rItem.top + 2 * ::GetSystemMetrics(SM_CYEDGE) + ::GetSystemMetrics(SM_CYVSCROLL);
	rItem.DeflateRect(::GetSystemMetrics(SM_CXEDGE) / 2,
					  ::GetSystemMetrics(SM_CYEDGE) / 2);

	rClient = rArrow = rItem;
	rArrow.left = rArrow.right - ::GetSystemMetrics(SM_CXVSCROLL);

	rClient.right = rArrow.left;
	//rClient.right = rItem.right - rItem.Height();
	//rArrow.left = rClient.right;

	//Draw Frame rect firts
	pDC->ExtTextOut(lpDrawItemStruct->rcItem.left,
					lpDrawItemStruct->rcItem.top,
					ETO_OPAQUE, &(lpDrawItemStruct->rcItem),
					"", 0, NULL );
	
	CRect rFrame = rItem;
	rFrame.InflateRect(::GetSystemMetrics(SM_CXEDGE) / 2,
					  ::GetSystemMetrics(SM_CYEDGE) / 2);
	pDC->DrawEdge(&rFrame, EDGE_SUNKEN, BF_RECT);

	pDC->Draw3dRect(rItem, RGB(0, 0, 0), RGB(192, 192, 192));


	rArrow.top += ::GetSystemMetrics(SM_CYEDGE);
	rArrow.bottom = rArrow.top + ::GetSystemMetrics(SM_CYVSCROLL);

	pDC->DrawFrameControl(&rArrow,
						DFC_BUTTON,
						uFrameState);
	


	DrawArrow(pDC,
			  &rArrow,
			  0,
			  (state & ODS_DISABLED) 
			  ? ::GetSysColor(COLOR_GRAYTEXT) 
			  : RGB(0,0,0));

	//******************************************************
	//**                     Draw Color
	//******************************************************
	
	rClient.DeflateRect(::GetSystemMetrics(SM_CXEDGE) / 2,
					  ::GetSystemMetrics(SM_CYEDGE) / 2);

	rClient.bottom -= ::GetSystemMetrics(SM_CYEDGE) / 2;
	if ((state & ODS_DISABLED) == 0)
	{
		pDC->FillSolidRect(&rClient,
						   (m_BackColor == CLR_DEFAULT)
						   ? m_DefaultColor
						   : m_BackColor);

	}



	//******************************************************
	//**                     Draw Text
	//******************************************************
	if (!m_strText.IsEmpty())
	{
		COLORREF clrOldText = pDC->SetTextColor(m_ForeColor);
		COLORREF clrOldBack = pDC->SetBkColor(m_BackColor);

		pDC->DrawText(m_strText, rClient, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		pDC->SetTextColor(clrOldText);
		pDC->SetBkColor(clrOldBack);
	}

	//******************************************************
	//**                     Draw Focus
	//******************************************************
	rClient.DeflateRect(::GetSystemMetrics(SM_CXEDGE) / 2,
					  ::GetSystemMetrics(SM_CYEDGE) / 2);

	if (state & ODS_FOCUS) 
    {
		RECT rFocus = {rClient.left,
					   rClient.top,
					   rClient.right - 1,
					   rClient.bottom};
  
        pDC->DrawFocusRect(&rFocus);
    }

}


//***********************************************************************
//**                          Static Methods                          **
//***********************************************************************

//***********************************************************************
// Method:	CColorButton::DrawArrow()
// Notes:	None.
//***********************************************************************
void CColorButton::DrawArrow(CDC* pDC, 
							 RECT* pRect, 
							 int iDirection,
							 COLORREF clrArrow /*= RGB(0,0,0)*/)
{
	POINT ptsArrow[3];

	CRect rectArrow;

	rectArrow.top = (pRect->top + pRect->bottom - g_ciArrowSizeY) / 2;
	rectArrow.bottom = rectArrow.top + g_ciArrowSizeY;
	rectArrow.left = (pRect->left + pRect->right - g_ciArrowSizeX) / 2;
	rectArrow.right = rectArrow.left + g_ciArrowSizeX;

	switch (iDirection)
	{
		case 0 : // Down
		{
			ptsArrow[0].x = rectArrow.left;
			ptsArrow[0].y = rectArrow.top;
			ptsArrow[1].x = rectArrow.right;
			ptsArrow[1].y = rectArrow.top;
			ptsArrow[2].x = (rectArrow.left + rectArrow.right)/2;
			ptsArrow[2].y = rectArrow.bottom;
			break;
		}

		case 1 : // Up
		{
			ptsArrow[0].x = rectArrow.left;
			ptsArrow[0].y = rectArrow.bottom;
			ptsArrow[1].x = rectArrow.right;
			ptsArrow[1].y = rectArrow.bottom;
			ptsArrow[2].x = (rectArrow.left + rectArrow.right)/2;
			ptsArrow[2].y = rectArrow.top;
			break;
		}

		case 2 : // Left
		{
			ptsArrow[0].x = rectArrow.right;
			ptsArrow[0].y = rectArrow.top;
			ptsArrow[1].x = rectArrow.right;
			ptsArrow[1].y = rectArrow.bottom;
			ptsArrow[2].x = rectArrow.left;
			ptsArrow[2].y = (rectArrow.top + rectArrow.bottom)/2;
			break;
		}

		case 3 : // Right
		{
			ptsArrow[0].x = rectArrow.left;
			ptsArrow[0].y = rectArrow.top;
			ptsArrow[1].x = rectArrow.left;
			ptsArrow[1].y = rectArrow.bottom;
			ptsArrow[2].x = rectArrow.right;
			ptsArrow[2].y = (rectArrow.top + rectArrow.bottom)/2;
			break;
		}
	}
	
	CBrush brsArrow(clrArrow);
	CPen penArrow(PS_SOLID, 1 , clrArrow);

	CBrush* pOldBrush = pDC->SelectObject(&brsArrow);
	CPen*   pOldPen   = pDC->SelectObject(&penArrow);
	
	pDC->SetPolyFillMode(WINDING);
	pDC->Polygon(ptsArrow, 3);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}

void CColorButton::SetText(LPCTSTR lpszText)
{
	m_strText = lpszText;

	if (::IsWindow(m_hWnd)) 
	{
        RedrawWindow();
	}
}

void CColorButton::SetChooseBkColor(BOOL bIsChooseBkColor)
{
	m_bIsSetBkColor = bIsChooseBkColor;
}
