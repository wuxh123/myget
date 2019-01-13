// OptionsLoginPage.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "OptionsLoginPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsLoginPage property page

IMPLEMENT_DYNCREATE(COptionsLoginPage, CPropertyPage)

COptionsLoginPage::COptionsLoginPage() : CPropertyPage(COptionsLoginPage::IDD)
{
	//{{AFX_DATA_INIT(COptionsLoginPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//m_psp.dwFlags |= PSP_USETITLE;
	//static CString strCaption;
	//strCaption = APP_GET_RSCSTR("Login_PPG", "Title");
	//m_psp.pszTitle = strCaption;
	SET_PROPERTYPAGE_TITLE("Login_PPG");

}

COptionsLoginPage::~COptionsLoginPage()
{
}

void COptionsLoginPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsLoginPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsLoginPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptionsLoginPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsLoginPage message handlers
