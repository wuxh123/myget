// TabCtrl.cpp : implementation file
//

#include "stdafx.h"

#include "TabDef.h" 
#include "TabCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabSheet

CTabSheet::CTabSheet()
{
   m_nNumOfTabs = 0;
}

CTabSheet::~CTabSheet()
{
}


BEGIN_MESSAGE_MAP(CTabSheet, CTabCtrl)
	//{{AFX_MSG_MAP(CTabSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabSheet message handlers




BOOL CTabSheet::InsertTab(char *TabString)
{
	TC_ITEM  TabCtrlItem;

   TabCtrlItem.mask        =  TCIF_TEXT;
   TabCtrlItem.cchTextMax  = _MAX_WORD ;
   TabCtrlItem. pszText    =  TabString;
   
	if(m_nNumOfTabs == InsertItem(m_nNumOfTabs, &TabCtrlItem))
   {
      m_nNumOfTabs++;
      return TRUE;
   }
	else
      return FALSE;	
}

int CTabSheet::GetCount()
{
   return m_nNumOfTabs;
}
