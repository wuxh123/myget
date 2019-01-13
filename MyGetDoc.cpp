// MyGetDoc.cpp : implementation of the CMyGetDoc class
//

#include "stdafx.h"
#include "MyGet.h"

#include "MyGetDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyGetDoc

IMPLEMENT_DYNCREATE(CMyGetDoc, CDocument)

BEGIN_MESSAGE_MAP(CMyGetDoc, CDocument)
	//{{AFX_MSG_MAP(CMyGetDoc)
	ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveas)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_SEND_MAIL, OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, OnUpdateFileSendMail)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyGetDoc construction/destruction

CMyGetDoc::CMyGetDoc()
{
	// TODO: add one-time construction code here

}

CMyGetDoc::~CMyGetDoc()
{
}

BOOL CMyGetDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMyGetDoc serialization

void CMyGetDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMyGetDoc diagnostics

#ifdef _DEBUG
void CMyGetDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMyGetDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyGetDoc commands

void CMyGetDoc::OnFileSaveas() 
{
	// TODO: Add your command handler code here
	CFileDialog dlgFile(
		FALSE,
		_T(".jcd"),
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("FlashGet Files (*.jcd)|*.jcd|All Files (*.*)|*.*||"));
	
	if (dlgFile.DoModal() != IDOK)
		return;

	CMyGetApp *pApp = (CMyGetApp *)AfxGetApp();

	pApp->m_strDBName = dlgFile.GetFileName();

	pApp->SaveDB();		
}

void CMyGetDoc::OnFileSave() 
{
	// TODO: Add your command handler code here
	CMyGetApp *pApp = (CMyGetApp *)AfxGetApp();

	pApp->SaveDB();		
}
