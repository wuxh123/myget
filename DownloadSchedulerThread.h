#if !defined(AFX_DOWNLOADSCHEDULERTHREAD_H__7A37DC6A_9B96_4DCB_AAFA_A280AA4807AA__INCLUDED_)
#define AFX_DOWNLOADSCHEDULERTHREAD_H__7A37DC6A_9B96_4DCB_AAFA_A280AA4807AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DownloadSchedulerThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CDownloadSchedulerThread thread
class CTreeItem;
class CDownloadScheduler;
class CDownloadSchedulerThread : public CWinThread
{
	DECLARE_DYNCREATE(CDownloadSchedulerThread)
public:
	CDownloadSchedulerThread();         
	virtual ~CDownloadSchedulerThread();
// Attributes
public:

// Operations
public:
	void SetNotifyWnd(HWND hNotifyWnd);
	void StartDownload();
	void SetDownloadTreeItem(CTreeItem *pTreeItem);
	void StopDownload();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownloadSchedulerThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	HWND				m_hNotifyWnd;
	CDownloadScheduler *m_pDownloadScheduler;
	
	// Generated message map functions
	//{{AFX_MSG(CDownloadSchedulerThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOWNLOADSCHEDULERTHREAD_H__7A37DC6A_9B96_4DCB_AAFA_A280AA4807AA__INCLUDED_)
