// OptionsOtherPage.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "OptionsOtherPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsOtherPage property page

IMPLEMENT_DYNCREATE(COptionsOtherPage, CPropertyPage)

COptionsOtherPage::COptionsOtherPage() : CPropertyPage(COptionsOtherPage::IDD)
{
	//{{AFX_DATA_INIT(COptionsOtherPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bConfirmQuitWhileDownloading			= FALSE;
	m_bConfirmQuitWhileOpenMultipleWindows	= FALSE;
	m_bConfirmAddJobUseIEMenu				= FALSE;
	m_bConfirmAddMultipleJobs				= FALSE;

	m_bShowSubCategoryInAlphaOrder			= FALSE;
	m_bHideWindowWhenMinimize				= FALSE;
	m_bMinimizeWhenX						= FALSE;
	m_bQuitAfterFinishedDownload			= FALSE;

	m_iDoubleClickDownloadingOrReadyItem	= 0;
	m_iDoubleClickDownloadedItem			= 0;
	m_iDoubleClickFailedItem				= 0;

	//m_psp.dwFlags |= PSP_USETITLE;
	//static CString strCaption;
	//strCaption = APP_GET_RSCSTR("Other_PPG", "Title");
	//m_psp.pszTitle = strCaption;
	SET_PROPERTYPAGE_TITLE("Other_PPG");

}

COptionsOtherPage::~COptionsOtherPage()
{
}

void COptionsOtherPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsOtherPage)
	DDX_Control(pDX, IDC_TV_OTHERS, m_tvOthers);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsOtherPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptionsOtherPage)
	ON_NOTIFY(NM_CLICK, IDC_TV_OTHERS, OnClickTvOthers)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// COptionsOtherPage message handlers

BOOL COptionsOtherPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	prv_FillDlg();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsOtherPage::prv_FillDlg()
{
	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();

	m_bConfirmQuitWhileDownloading			= pAppRegs->IsFlagOn(REG_FLAG_GENERAL_EXT_CONFIRMQUITWHILEDOWNLOADING);
	m_bConfirmQuitWhileOpenMultipleWindows	= pAppRegs->IsFlagOn(REG_FLAG_GENERAL_EXT_CONFIRMQUITWHILEOPENMULIPLEWINDOWS);
	m_bConfirmAddJobUseIEMenu				= pAppRegs->IsFlagOn(REG_FLAG_GENERAL_EXT_CONFIRMDOWNLOADUSEIEMENU);
	m_bConfirmAddMultipleJobs				= pAppRegs->IsFlagOn(REG_FLAG_GENERAL_EXT_CONFIRMQUITWHILEADDMUTIPLEJOBS);

	m_bShowSubCategoryInAlphaOrder			= pAppRegs->IsFlagOn(REG_FLAG_GENERAL_SHOWSUBCATEGORYINALPHAORDER);
	m_bHideWindowWhenMinimize				= pAppRegs->IsFlagOn(REG_FLAG_GENERAL_EXT_HIDEWINDOWWHILEMINIMIZE);
	m_bQuitAfterFinishedDownload			= pAppRegs->IsFlagOn(REG_FLAG_DIALUP_QUITAFTERFINISHEDDOWNLOAD);

	pAppRegs->GetVal(REG_GENERAL_MINIMIZEWHENX, (DWORD *)&m_bMinimizeWhenX);
	m_bMinimizeWhenX = !m_bMinimizeWhenX;

	m_iDoubleClickDownloadingOrReadyItem	= pAppRegs->GetDoubleClickFlag(REG_DOUBLECLICK_DOWNLOADINGORREADYITEM);
	m_iDoubleClickDownloadedItem			= pAppRegs->GetDoubleClickFlag(REG_DOUBLECLICK_DOWNLOADEDITEM);
	m_iDoubleClickFailedItem			= pAppRegs->GetDoubleClickFlag(REG_DOUBLECLICK_FAILEDITEM);

	CBitmap bmpChoiceType;
	bmpChoiceType.LoadBitmap(IDB_CHOICE_TYPE_BMP);
	m_ilOther.Create(IDB_OTHERS, 16, 1, CLR_DEFAULT);
	m_ilOther.Add(&bmpChoiceType, CLR_DEFAULT);

	m_tvOthers.SetImageList(&m_ilOther, TVSIL_NORMAL);

	static struct InitOtherTable InitTable[] =
	{
		{0, 5,	"String",		61389,	COMMON_ITEM,		0, NULL,									""},
		{1, 8,	"String",		61390,	CHECKBOX_ITEM,		0, &m_bConfirmQuitWhileDownloading,			""},
		{1, 8,	"String",		61391,	CHECKBOX_ITEM,		0, &m_bConfirmQuitWhileOpenMultipleWindows,	""},
		{1, 8,	"String",		61392,	CHECKBOX_ITEM,		0, &m_bConfirmAddJobUseIEMenu,				""},
		{1, 8,	"String",		61393,	CHECKBOX_ITEM,		0, &m_bConfirmAddMultipleJobs,				""},
		{0, 0,	"Other_PPG",	1197,	COMMON_ITEM,		0, NULL,									"When double-clicking on an item"},
		{1, 1,	"Other_PPG",	1198,	COMMON_ITEM,		0, NULL,									"Download Job"},
		{2, 10, "Other_PPG",	1099,	RADIOBUTTON_ITEM,	0, &m_iDoubleClickDownloadingOrReadyItem,	"Start/Stop"},
		{2, 10, "Other_PPG",	1100,	RADIOBUTTON_ITEM,	1, &m_iDoubleClickDownloadingOrReadyItem,	"Edit"},
		{2, 10, "Other_PPG",	1101,	RADIOBUTTON_ITEM,	2, &m_iDoubleClickDownloadingOrReadyItem,	"Comment"},
		{1, 2,	"Other_PPG",	1200,	COMMON_ITEM,		0, NULL,									"Successful download job"},
		{2, 10, "Other_PPG",	1102,	RADIOBUTTON_ITEM,	0, &m_iDoubleClickDownloadedItem,			"View"},
		{2, 10, "Other_PPG",	1103,	RADIOBUTTON_ITEM,	1, &m_iDoubleClickDownloadedItem,			"Edit"},
		{2, 10, "Other_PPG",	1104,	RADIOBUTTON_ITEM,	2, &m_iDoubleClickDownloadedItem,			"Comment"},
		{1, 2,	"Other_PPG",	1199,	COMMON_ITEM,		0, NULL,									"Failed Download job"},
		{2, 10, "Other_PPG",	1105,	RADIOBUTTON_ITEM,	0, &m_iDoubleClickFailedItem,				"Open Folder"},
		{2, 10, "Other_PPG",	1106,	RADIOBUTTON_ITEM,	1, &m_iDoubleClickFailedItem,				"Edit"},
		{2, 10, "Other_PPG",	1107,	RADIOBUTTON_ITEM,	2, &m_iDoubleClickFailedItem,				"Comment"},
		{0, 4,	"String",		61384,	COMMON_ITEM,		0, NULL,									""},
		{1, 8,	"Other_PPG",	1260,	CHECKBOX_ITEM,		0, &m_bShowSubCategoryInAlphaOrder,			"Display Sub-Categories in alphabetical order(Need to restart FlashGet)"},
		{1, 8,	"String",		61385,	CHECKBOX_ITEM,		0, &m_bHideWindowWhenMinimize,				""},
		{1, 8,	"String",		61386,	CHECKBOX_ITEM,		0, &m_bMinimizeWhenX,						""},
		{1, 8,	"String",		61387,	CHECKBOX_ITEM,		0, &m_bQuitAfterFinishedDownload,			""}

	};


	static int iSizeOfInitTable = sizeof(InitTable) / sizeof(InitTable[0]);

	m_pInitTable = InitTable;
	m_iSizeOfInitTable = iSizeOfInitTable;

	HTREEITEM hParentTvItem = TVI_ROOT;
	HTREEITEM hLastItem = TVI_ROOT;
	int iLastDepth = 0;
	int iUpdatedImageIndex = 0;

	for (int i = 0; i < iSizeOfInitTable; i ++)
	{
		if (InitTable[i].iDepth > iLastDepth)
		{
			hParentTvItem = hLastItem;
		}
		else
		{
			for (int j = 0; j < iLastDepth - InitTable[i].iDepth; j ++)
			{
				hParentTvItem = m_tvOthers.GetNextItem(hParentTvItem, TVGN_PARENT);
			}
		}
		
		iUpdatedImageIndex = InitTable[i].iInitImage;

		switch(InitTable[i].iType)
		{
		case CHECKBOX_ITEM:
			if (*InitTable[i].piVal)
			{
				iUpdatedImageIndex = InitTable[i].iInitImage - 1;
			}

			break;
		case RADIOBUTTON_ITEM:
			if (*InitTable[i].piVal == InitTable[i].iGroupIndex)
			{
				iUpdatedImageIndex = InitTable[i].iInitImage - 1;
			}

			break;
		}

		LPCTSTR lpszLabel = APP_GET_RSCSTR(InitTable[i].lpszSecton, InitTable[i].uiKeyID);

		if (lpszLabel == NULL)
		{
			lpszLabel = InitTable[i].lpszDefaultLabel;
		}

		hLastItem = m_tvOthers.InsertItem(
			lpszLabel , iUpdatedImageIndex, iUpdatedImageIndex, hParentTvItem);
		InitTable[i].hTvItem = hLastItem;

		m_tvOthers.Expand(hParentTvItem, TVE_EXPAND);

		iLastDepth = InitTable[i].iDepth;
	}

	UpdateData(FALSE);
}

void COptionsOtherPage::OnClickTvOthers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint ptClick;
	GetCursorPos(&ptClick);
	//ClientToScreen(&ptClick);
	m_tvOthers.ScreenToClient(&ptClick);


	HTREEITEM hFocusItem = m_tvOthers.HitTest(ptClick);
	for (int i = 0; i < m_iSizeOfInitTable; i ++)
	{
		if (m_pInitTable[i].hTvItem == hFocusItem)
		{
			switch(m_pInitTable[i].iType)
			{
			case CHECKBOX_ITEM:
				*m_pInitTable[i].piVal = !(*m_pInitTable[i].piVal);

				if (*m_pInitTable[i].piVal)
				{
					m_tvOthers.SetItemImage(hFocusItem, m_pInitTable[i].iInitImage - 1, m_pInitTable[i].iInitImage - 1);
				}
				else
				{
					m_tvOthers.SetItemImage(hFocusItem, m_pInitTable[i].iInitImage, m_pInitTable[i].iInitImage);
				}
				break;

			case RADIOBUTTON_ITEM:
				*m_pInitTable[i].piVal = m_pInitTable[i].iGroupIndex;
				
				for (int j = 0; j < m_iSizeOfInitTable; j ++)
				{
					if (m_pInitTable[j].piVal == m_pInitTable[i].piVal)
					{
						if (m_pInitTable[j].iGroupIndex == *m_pInitTable[i].piVal)
						{
							m_tvOthers.SetItemImage(m_pInitTable[j].hTvItem, m_pInitTable[i].iInitImage - 1, m_pInitTable[i].iInitImage - 1);
						}
						else
						{
							m_tvOthers.SetItemImage(m_pInitTable[j].hTvItem, m_pInitTable[i].iInitImage, m_pInitTable[i].iInitImage);
						}
					}
				}
				
				break;
			}			
			break;
		}
	}
	*pResult = 0;
}
