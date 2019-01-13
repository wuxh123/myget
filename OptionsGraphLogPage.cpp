// OptionsGraphLogPage.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "OptionsGraphLogPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsGraphLogPage property page

IMPLEMENT_DYNCREATE(COptionsGraphLogPage, CPropertyPage)

COptionsGraphLogPage::COptionsGraphLogPage() : CPropertyPage(COptionsGraphLogPage::IDD)
{
	//{{AFX_DATA_INIT(COptionsGraphLogPage)
	m_bDropZoneDisplayGraph = FALSE;
	m_bDropZoneTransparent = FALSE;
	m_bLogSettingBkColor = FALSE;
	m_iBlockSize = 0;
	m_iGraphScale = 0;
	//}}AFX_DATA_INIT
	//m_psp.dwFlags |= PSP_USETITLE;
	//static CString strCaption;
	//strCaption = APP_GET_RSCSTR("Color_PPG", "Title");
	//m_psp.pszTitle = strCaption;
	SET_PROPERTYPAGE_TITLE("Color_PPG");

}

COptionsGraphLogPage::~COptionsGraphLogPage()
{
}

void COptionsGraphLogPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsGraphLogPage)
	DDX_Control(pDX, IDC_SPN_GRAPHSCALE, m_spnGraphScale);
	DDX_Control(pDX, IDC_SPN_BLOCKSIZE, m_spnBlockSize);
	DDX_Control(pDX, IDC_BTN_LOGSTATE, m_clrbtnLogState);
	DDX_Control(pDX, IDC_BTN_LOGSERVER, m_clrbtnLogServer);
	DDX_Control(pDX, IDC_BTN_LOGERROR, m_clrbtnLogError);
	DDX_Control(pDX, IDC_BTN_LOGCOMMAND, m_clrbtnLogCommand);
	DDX_Control(pDX, IDC_BTN_GRAPHFORE, m_clrbtnGraphFore);
	DDX_Control(pDX, IDC_BTN_GRAPHBACK, m_clrbtnGraphBack);
	DDX_Control(pDX, IDC_BTN_BLOCKBACK, m_clrbtnBlockBack);
	DDX_Check(pDX, IDC_CHK_DROPZONEDISPLAYGRAPH, m_bDropZoneDisplayGraph);
	DDX_Check(pDX, IDC_CHK_DROPZONETRANSPARENT, m_bDropZoneTransparent);
	DDX_Check(pDX, IDC_CHK_LOGSETTINGBKCOLOR, m_bLogSettingBkColor);
	DDX_Text(pDX, IDC_EDT_BLOCKSIZE, m_iBlockSize);
	DDV_MinMaxInt(pDX, m_iBlockSize, 1, 1024);
	DDX_Text(pDX, IDC_EDT_GRAPHSCALE, m_iGraphScale);
	DDV_MinMaxInt(pDX, m_iGraphScale, 1, 1000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsGraphLogPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptionsGraphLogPage)
	ON_BN_CLICKED(IDC_CHK_LOGSETTINGBKCOLOR, OnChkLogsettingbkcolor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsGraphLogPage message handlers

void COptionsGraphLogPage::prv_FillDlg()
{
	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();
	UDACCEL udAccel;

	//Block Size
	pAppRegs->GetVal(REG_COLOR_BLOCKSIZE,	(DWORD *)&m_iBlockSize);
	
	m_spnBlockSize.GetAccel(1, &udAccel);
	udAccel.nInc = 1;
	m_spnBlockSize.SetAccel(1, &udAccel);
	m_spnBlockSize.SetRange(1, 1024);

	//Scale
	pAppRegs->GetVal(REG_COLOR_SCALE,		(DWORD *)&m_iGraphScale);
	m_spnGraphScale.GetAccel(1, &udAccel);
	udAccel.nInc = 1;
	m_spnGraphScale.SetAccel(1, &udAccel);
	m_spnGraphScale.SetRange(1, 1000);

	//Colors(FG&BK)
	pAppRegs->GetVal(REG_COLOR_GRAPHFORE,	&m_dwGraphFore);
	pAppRegs->GetVal(REG_COLOR_GRAPHBACK,	&m_dwGraphBack);
	pAppRegs->GetVal(REG_COLOR_STATE,		&m_dwLogState);
	pAppRegs->GetVal(REG_COLOR_STATEBK,		&m_dwLogStateBk);
	pAppRegs->GetVal(REG_COLOR_COMMAND,		&m_dwLogCommand);
	pAppRegs->GetVal(REG_COLOR_COMMANDBK,	&m_dwLogCommandBk);
	pAppRegs->GetVal(REG_COLOR_SERVER,		&m_dwLogServer);
	pAppRegs->GetVal(REG_COLOR_SERVERBK,	&m_dwLogServerBk);
	pAppRegs->GetVal(REG_COLOR_ERROR,		&m_dwLogError);
	pAppRegs->GetVal(REG_COLOR_ERRORBK,		&m_dwLogErrorBk);
	pAppRegs->GetVal(REG_COLOR_BLOCKBACK,	&m_dwBlockBack);

	pAppRegs->GetVal(REG_DROPZONE_TRANSPARENT, (DWORD *)&m_bDropZoneTransparent);
	m_bDropZoneDisplayGraph = pAppRegs->IsFlagOn(REG_FLAG_GENERAL_EXT_DROPZONESHOWFLOWCHART);
	
	
	m_clrbtnGraphFore.SetBackColor((COLORREF)m_dwGraphFore);
	m_clrbtnGraphBack.SetBackColor((COLORREF)m_dwGraphBack);
	m_clrbtnBlockBack.SetBackColor((COLORREF)m_dwBlockBack);
	
	m_clrbtnLogState.SetBackColor((COLORREF)m_dwLogStateBk);
	m_clrbtnLogState.SetText("State Text");
	m_clrbtnLogState.SetForeColor((COLORREF)m_dwLogState);

	m_clrbtnLogCommand.SetBackColor((COLORREF)m_dwLogCommandBk);
	m_clrbtnLogCommand.SetText("Command Text");
	m_clrbtnLogCommand.SetForeColor((COLORREF)m_dwLogCommand);

	m_clrbtnLogServer.SetBackColor((COLORREF)m_dwLogServerBk);
	m_clrbtnLogServer.SetText("Server Text");
	m_clrbtnLogServer.SetForeColor((COLORREF)m_dwLogServer);

	m_clrbtnLogError.SetBackColor((COLORREF)m_dwLogErrorBk);
	m_clrbtnLogError.SetText("Error Text");
	m_clrbtnLogError.SetForeColor((COLORREF)m_dwLogError);

	m_clrbtnLogCommand.SetChooseBkColor(m_bLogSettingBkColor);
	m_clrbtnLogState.SetChooseBkColor(m_bLogSettingBkColor);
	m_clrbtnLogServer.SetChooseBkColor(m_bLogSettingBkColor);
	m_clrbtnLogError.SetChooseBkColor(m_bLogSettingBkColor);
	
	UpdateData(FALSE);
	
}

BOOL COptionsGraphLogPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	ReloadResource();
	prv_FillDlg();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void COptionsGraphLogPage::OnChkLogsettingbkcolor() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_clrbtnLogCommand.SetChooseBkColor(m_bLogSettingBkColor);
	m_clrbtnLogState.SetChooseBkColor(m_bLogSettingBkColor);
	m_clrbtnLogServer.SetChooseBkColor(m_bLogSettingBkColor);
	m_clrbtnLogError.SetChooseBkColor(m_bLogSettingBkColor);
}

void COptionsGraphLogPage::ReloadResource()
{
	//Dlg Items
	LPCTSTR lpszDlgID = _T("Color_PPG");

	SET_DLG_ITEM_TEXT(1197);
	SET_DLG_ITEM_TEXT(1198);
	SET_DLG_ITEM_TEXT(1199);
	SET_DLG_ITEM_TEXT(1200);
	SET_DLG_ITEM_TEXT(1201);
	SET_DLG_ITEM_TEXT(1202);
	SET_DLG_ITEM_TEXT(1148);
	SET_DLG_ITEM_TEXT(1068);
	SET_DLG_ITEM_TEXT(1069);
	SET_DLG_ITEM_TEXT(1070);
	SET_DLG_ITEM_TEXT(1071);
	SET_DLG_ITEM_TEXT(1075);
	SET_DLG_ITEM_TEXT(1217);
	SET_DLG_ITEM_TEXT(1203);
	SET_DLG_ITEM_TEXT(1281);

}

void COptionsGraphLogPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	m_dwGraphFore = m_clrbtnGraphFore.GetBackColor();
	m_dwGraphBack = m_clrbtnGraphBack.GetBackColor();
	m_dwBlockBack = m_clrbtnBlockBack.GetBackColor();
	
	m_dwLogStateBk = m_clrbtnLogState.GetBackColor();
	m_dwLogState	= m_clrbtnLogState.GetForeColor();

	m_dwLogCommandBk	= m_clrbtnLogCommand.GetBackColor();
	m_dwLogCommand		= m_clrbtnLogCommand.GetForeColor();

	m_dwLogServerBk	= m_clrbtnLogServer.GetBackColor();
	m_dwLogServer	= m_clrbtnLogServer.GetForeColor();

	m_dwLogErrorBk	= m_clrbtnLogError.GetBackColor();
	m_dwLogError	= m_clrbtnLogError.GetForeColor();

	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();
	
	//Block Size
	pAppRegs->SetVal(REG_COLOR_BLOCKSIZE,	(DWORD)m_iBlockSize);

	//Scale
	pAppRegs->SetVal(REG_COLOR_SCALE,		(DWORD)m_iGraphScale);
	
	//Colors(FG&BK)
	pAppRegs->SetVal(REG_COLOR_GRAPHFORE,	m_dwGraphFore);
	pAppRegs->SetVal(REG_COLOR_GRAPHBACK,	m_dwGraphBack);
	pAppRegs->SetVal(REG_COLOR_STATE,	m_dwLogState);
	pAppRegs->SetVal(REG_COLOR_STATEBK,	m_dwLogStateBk);
	pAppRegs->SetVal(REG_COLOR_COMMAND,	m_dwLogCommand);
	pAppRegs->SetVal(REG_COLOR_COMMANDBK,	m_dwLogCommandBk);
	pAppRegs->SetVal(REG_COLOR_SERVER,	m_dwLogServer);
	pAppRegs->SetVal(REG_COLOR_SERVERBK,	m_dwLogServerBk);
	pAppRegs->SetVal(REG_COLOR_ERROR,	m_dwLogError);
	pAppRegs->SetVal(REG_COLOR_ERRORBK,	m_dwLogErrorBk);
	pAppRegs->SetVal(REG_COLOR_BLOCKBACK,	m_dwBlockBack);

	pAppRegs->SetVal(REG_DROPZONE_TRANSPARENT, (DWORD)m_bDropZoneTransparent);

	pAppRegs->SetFlag(REG_FLAG_GENERAL_EXT_DROPZONESHOWFLOWCHART, m_bDropZoneDisplayGraph);

	CPropertyPage::OnOK();
}
