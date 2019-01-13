// MyGetView.cpp : implementation of the CMyGetView class
//

#include "stdafx.h"
#include "MyGet.h"

#include "MyGetDoc.h"
#include "MyGetView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyGetView

IMPLEMENT_DYNCREATE(CMyGetView, CView)

BEGIN_MESSAGE_MAP(CMyGetView, CView)
	//{{AFX_MSG_MAP(CMyGetView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyGetView construction/destruction

CMyGetView::CMyGetView()
{
	// TODO: add construction code here

}

CMyGetView::~CMyGetView()
{
}

BOOL CMyGetView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMyGetView drawing

void CMyGetView::OnDraw(CDC* pDC)
{
	CMyGetDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CMyGetView diagnostics

#ifdef _DEBUG
void CMyGetView::AssertValid() const
{
	CView::AssertValid();
}

void CMyGetView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyGetDoc* CMyGetView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyGetDoc)));
	return (CMyGetDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyGetView message handlers
