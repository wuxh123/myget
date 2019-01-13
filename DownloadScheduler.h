#if !defined(AFX_DOWNLOADSCHUDLER_H__DEA6AD06_1FFE_4CD2_B6B5_13183FB73E97__INCLUDED_)
#define AFX_DOWNLOADSCHUDLER_H__DEA6AD06_1FFE_4CD2_B6B5_13183FB73E97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DownloadSchudler.h : header file
//
#include <Winsock2.h>
class CMainFrame;
class CDownloadItemManager;
class CDownloadJet;

class CTreeItem;
class CListItem;
/////////////////////////////////////////////////////////////////////////////
// CDownloadScheduler window
#define WM_USER_ASYNC_SOCKET_MSG	WM_USER + 301

class CDownloadScheduler : public CWnd
{
// Construction
public:
	CDownloadScheduler();
	DECLARE_DYNCREATE(CDownloadScheduler)

	~CDownloadScheduler();
// Attributes
public:
	
protected:
	//管理ListItem和DownloadManager的关系
	CMap <CListItem *, CListItem *, CDownloadItemManager *, CDownloadItemManager *> m_mapDownloadItemMNGs;

	//管理DownloadItemManager和DownloadJet（实际上是socket)的关系，
	CMap <CDownloadJet *, CDownloadJet *, CDownloadItemManager *, CDownloadItemManager *> m_mapDownloadJets;

	//管理DownloadJet及其注册时间
	CMap <CDownloadJet *, CDownloadJet *, DWORD, DWORD> m_mapJetRegestedTimeout;

	CTreeItem *m_pTreeItem;

	HWND m_hMainWnd;


private:
	int m_iCountOfLastDownloadData;
	//For Get last download data count;
	CCriticalSection m_crtsectCanGetDataCount;
	CSingleLock m_slockCanGetDataCount;

	enum TimerID {ID_SchedulerTimer, ID_1sTimer};
	enum {SchedulerTimerSlot = 100};
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownloadScheduler)
	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetNotifyWnd(HWND hNotifyWnd);
	//virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
	void RemoveDownloadEntry(CDownloadJet *pJet, CDownloadItemManager *pMNG);
	void UnregisterTimeout(CDownloadJet *pJet);
	void RegisterTimeout(CDownloadJet *pJet, DWORD dwTimeout);
	BOOL AddDownloadEntry(CDownloadItemManager *pMNG, CDownloadJet *pJet, SOCKET socket);
	void StopDownload();
	void StartDownload();
	void RefreshMainframe(CListItem *pListItem, UINT uiState = 0);
	void SetDownloadTreeItem(CTreeItem *pTreeItem);
	void Quit();
	void MoveCompletedItem(CListItem *pListItem);
	void IncLastDownloadDataCounter(int iInc);
	// Generated message map functions
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//{{AFX_MSG(CDownloadScheduler)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnTimer(UINT nIDEvent); 
	afx_msg LRESULT OnAsyncSocketMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDownloadItemMsg(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void prv_CheckTimeout();
	int prv_GetLastDownloadDataCount();
	void prv_ClearLastDownloadDataCounter();
	void prv_NeedPauseDownloadingItems();
	void prv_CheckScheduledItem();
	void prv_StartDownloadReadyItems();
	BOOL prv_IsItemInDownloadList(CListItem *pListItem);
	void prv_StartDownload(CListItem *pListItem);
	void prv_DoSchedule();
	void prv_StopDownload(CListItem *pListItem);
	BOOL prv_IsValidateDownloadEntry(CDownloadItemManager *pMNG);
	void prv_RemoveDownloadEntry(CDownloadItemManager *pMNG);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOWNLOADSCHUDLER_H__DEA6AD06_1FFE_4CD2_B6B5_13183FB73E97__INCLUDED_)
