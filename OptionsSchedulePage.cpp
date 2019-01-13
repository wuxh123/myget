// OptionsSchedulePage.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "OptionsSchedulePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsSchedulePage property page

IMPLEMENT_DYNCREATE(COptionsSchedulePage, CPropertyPage)

COptionsSchedulePage::COptionsSchedulePage() : CPropertyPage(COptionsSchedulePage::IDD)
{
	//{{AFX_DATA_INIT(COptionsSchedulePage)
	m_bEnableStopTimer = FALSE;
	m_bStartDownloadAtMonday = FALSE;
	m_bStartDownloadAtTuesday = FALSE;
	m_bStartDownloadAtWednesday = FALSE;
	m_bStartDownloadAtThursday = FALSE;
	m_bStartDownloadAtFriday = FALSE;
	m_bStartDownloadAtSaturday = FALSE;
	m_bStartDownloadAtSunday = FALSE;
	m_tDownloadStartAt = 0;
	m_tDownloadStopAt = 0;
	//}}AFX_DATA_INIT

	//m_psp.dwFlags |= PSP_USETITLE;
	//static CString strCaption;
	//strCaption = APP_GET_RSCSTR("Schedule_PPG", "Title");
	//m_psp.pszTitle = strCaption;
	SET_PROPERTYPAGE_TITLE("Schedule_PPG");

}

COptionsSchedulePage::~COptionsSchedulePage()
{
}

void COptionsSchedulePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsSchedulePage)
	DDX_Check(pDX, IDC_CHK_ENABLESTOPTIMER, m_bEnableStopTimer);
	DDX_Check(pDX, IDC_CHK_STARTDOWNLOADAT_MONDAY, m_bStartDownloadAtMonday);
	DDX_Check(pDX, IDC_CHK_STARTDOWNLOADAT_TUESDAY, m_bStartDownloadAtTuesday);
	DDX_Check(pDX, IDC_CHK_STARTDOWNLOADAT_WEDNESDAY, m_bStartDownloadAtWednesday);
	DDX_Check(pDX, IDC_CHK_STARTDOWNLOADAT_THURSDAY, m_bStartDownloadAtThursday);
	DDX_Check(pDX, IDC_CHK_STARTDOWNLOADAT_FRIDAY, m_bStartDownloadAtFriday);
	DDX_Check(pDX, IDC_CHK_STARTDOWNLOADAT_SATURDAY, m_bStartDownloadAtSaturday);
	DDX_Check(pDX, IDC_CHK_STARTDOWNLOADAT_SUNDAY, m_bStartDownloadAtSunday);
	DDX_DateTimeCtrl(pDX, IDC_DTP_DOWNLOADSTARTAT, m_tDownloadStartAt);
	DDX_DateTimeCtrl(pDX, IDC_DTP_DOWNLOADSTOPAT, m_tDownloadStopAt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsSchedulePage, CPropertyPage)
	//{{AFX_MSG_MAP(COptionsSchedulePage)
	ON_BN_CLICKED(IDC_CHK_ENABLESTOPTIMER, OnChkEnablestoptimer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsSchedulePage message handlers

BOOL COptionsSchedulePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	ReloadResource();
	prv_FillDlg();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsSchedulePage::prv_FillDlg()
{
	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();

	//Start Time
	pAppRegs->GetVal(REG_GENERAL_SCHEDULESTARTTIME, (DWORD *)&m_tDownloadStartAt);

	//Stop Time
	pAppRegs->GetVal(REG_GENERAL_SCHEDULESTOPTIME, (DWORD *)&m_tDownloadStopAt);

	//Enable download in week;
	m_bStartDownloadAtMonday	= pAppRegs->IsFlagOn(REG_FLAG_SCHEDULE_ENABLEDOWNLOADATMONDAY);
	m_bStartDownloadAtTuesday	= pAppRegs->IsFlagOn(REG_FLAG_SCHEDULE_ENABLEDOWNLOADATTUESDAY);
	m_bStartDownloadAtWednesday = pAppRegs->IsFlagOn(REG_FLAG_SCHEDULE_ENABLEDOWNLOADATWEDNESDAY);
	m_bStartDownloadAtThursday	= pAppRegs->IsFlagOn(REG_FLAG_SCHEDULE_ENABLEDOWNLOADATTHURSDAY);
	m_bStartDownloadAtFriday	= pAppRegs->IsFlagOn(REG_FLAG_SCHEDULE_ENABLEDOWNLOADATFRIDAY);
	m_bStartDownloadAtSaturday	= pAppRegs->IsFlagOn(REG_FLAG_SCHEDULE_ENABLEDOWNLOADATSATURDAY);
	m_bStartDownloadAtSunday	= pAppRegs->IsFlagOn(REG_FLAG_SCHEDULE_ENABLEDOWNLOADATSUNDAY);

	m_bEnableStopTimer			= pAppRegs->IsFlagOn(REG_FLAG_SCHEDULE_ENABLESTOPTIMER);

	prv_UpdateStopTimer();

	UpdateData(FALSE);
}

void COptionsSchedulePage::ReloadResource()
{
	//Dlg Items
	LPCTSTR lpszDlgID = _T("Schedule_PPG");

	SET_DLG_ITEM_TEXT(1197);
	SET_DLG_ITEM_TEXT(1110);
	SET_DLG_ITEM_TEXT(1111);
	SET_DLG_ITEM_TEXT(1112);
	SET_DLG_ITEM_TEXT(1113);
	SET_DLG_ITEM_TEXT(1114);
	SET_DLG_ITEM_TEXT(1115);
	SET_DLG_ITEM_TEXT(1116);
	SET_DLG_ITEM_TEXT(1082);

}

void COptionsSchedulePage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();

	//Start Time
	pAppRegs->SetVal(REG_GENERAL_SCHEDULESTARTTIME, (DWORD)m_tDownloadStartAt.GetTime());

	//Stop Time
	pAppRegs->SetVal(REG_GENERAL_SCHEDULESTOPTIME, (DWORD)m_tDownloadStopAt.GetTime());

	//Enable download in week;
	pAppRegs->SetFlag(REG_FLAG_SCHEDULE_ENABLEDOWNLOADATMONDAY,		m_bStartDownloadAtMonday);
	pAppRegs->SetFlag(REG_FLAG_SCHEDULE_ENABLEDOWNLOADATTUESDAY,	m_bStartDownloadAtTuesday);
	pAppRegs->SetFlag(REG_FLAG_SCHEDULE_ENABLEDOWNLOADATWEDNESDAY,	m_bStartDownloadAtWednesday);
	pAppRegs->SetFlag(REG_FLAG_SCHEDULE_ENABLEDOWNLOADATTHURSDAY,	m_bStartDownloadAtThursday);
	pAppRegs->SetFlag(REG_FLAG_SCHEDULE_ENABLEDOWNLOADATFRIDAY,		m_bStartDownloadAtFriday);
	pAppRegs->SetFlag(REG_FLAG_SCHEDULE_ENABLEDOWNLOADATSATURDAY,	m_bStartDownloadAtSaturday);
	pAppRegs->SetFlag(REG_FLAG_SCHEDULE_ENABLEDOWNLOADATSUNDAY,		m_bStartDownloadAtSunday);


	pAppRegs->SetFlag(REG_FLAG_SCHEDULE_ENABLESTOPTIMER,			m_bEnableStopTimer);

	CPropertyPage::OnOK();
}

void COptionsSchedulePage::OnChkEnablestoptimer() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	prv_UpdateStopTimer();
}

void COptionsSchedulePage::prv_UpdateStopTimer()
{
	GetDlgItem(IDC_DTP_DOWNLOADSTOPAT)->EnableWindow(m_bEnableStopTimer);
}
