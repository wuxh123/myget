// DownloadItemManager.h: interface for the CDownloadItemManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DownloadItemManager_H__9B444145_79CC_4D50_B963_B01D851E00AF__INCLUDED_)
#define AFX_DownloadItemManager_H__9B444145_79CC_4D50_B963_B01D851E00AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <Winsock2.h>
#include "ListItem.h"
class CDownloadJet;
class CDownloadScheduler;


#define DOWNLOAD_JOB_END	WM_USER + 200
#define WM_DOWNLOAD_ITEM_MSG WM_USER + 201
/////////////////////////////////////////////////////////////////////////////
// CDownloadItemManager thread

class CDownloadItemManager
{


public:
	void RemoveDownloadEntry(CDownloadJet *pJet);
	void UnregisterTimeout(CDownloadJet *pJet);
	void RegisterTimeout(CDownloadJet *pJet, DWORD dwTimeout);
	BOOL AddDownloadEntry(CDownloadJet *pJet, SOCKET socket);
	CListItem * GetListItem();
	int StopDownload();
	int StartDownload();
	CDownloadItemManager(CDownloadScheduler *pDownloadScheduler = NULL, CListItem *pListItem = NULL);
	virtual ~CDownloadItemManager();


	void JetCompleted(CDownloadJet *pJet, LPCTSTR lpszMsg);
	void JetRecvOK(CDownloadJet *pJet, LPCTSTR lpszMsg);
	BOOL JetStartRecv(CDownloadJet *pJet, LPCTSTR lpszMsg);
	void JetGeneralInfo(CDownloadJet *pJet, LPCTSTR lpszMsg);
	void JetFailed(CDownloadJet *pJet, LPCTSTR lpszMsg);
	void JetCanceled(CDownloadJet *pJet, LPCTSTR lpszMsg);
	void JetResultFailed(CDownloadJet *pJet, LPCTSTR lpszMsg);

private:
	int 		m_iSimJets;	//同时可以进行的下载线程数w量。
	CListItem 	*m_pListItem;  	//当前处理的DownloadItem的指针。
	CFile 		*m_pFile;	//本地文件描述字。
	int			m_iResumeState; 	//-1: 不支持；0：不知道；1：支持。
	CDownloadScheduler *m_pDownloadScheduler;
	BOOL m_bQuit;
	//For Handle Message.
	CCriticalSection m_crtsectCanHandleMsg;
	CSingleLock m_slockCanHandleMsg;


	CArray <CDownloadJet *, CDownloadJet *> m_arJets;	
	int m_iDownloadFileSize;
	BOOL m_bHasInitMaps;
	CString m_strFilePath;
	CArray <BREAKPOSITION *, BREAKPOSITION *> *m_parDownloadBrks;
	CArray <BREAKPOSITION *, BREAKPOSITION *> m_arUnDownloadedBrks;

private:
	void prv_RefreshItem(CDownloadJet *pJet, LPCTSTR lpszMsg);
	void prv_AttachJetInfo(CDownloadJet *pJet, LPCTSTR lpszMsg);
	void prv_HandleThreadCompleted(CDownloadJet *pJet);
	void prv_UpdateDownloadBrkMap(CDownloadJet *pJet);
	void prv_ClearJob();
	BOOL prv_NewJet(BOOL bIsFirst);
	void prv_MergeUndownloadedBrks();
	BOOL prv_HasAssigned(PBREAKPOSITION pBrkPos);
	BOOL prv_InitMaps();
	BOOL prv_AssignDownloadRange(CDownloadJet *pJet);
	BOOL prv_InitDownloadJob();
	int prv_GetThreadIndex(CDownloadJet *pJet);

// Implementation
protected:

};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DownloadItemManager_H__9B444145_79CC_4D50_B963_B01D851E00AF__INCLUDED_)
