
#ifndef _LIST_ITEM_H
#define _LIST_ITEM_H

#include "ItemHeader.h"


//define Download item attribute bit.
#define ATTR_SUPPORTRESUME	1
#define ATTR_EXCUTED		2
#define ATTR_NEEDLOGIN		4
#define ATTR_NOTUSEMIRROR	8

//define File Complete Flag
#define UNCOMPLETED		1
#define DOWNLOADING		2
#define COMPLETED		4
#define DOWNLOAD_ERR	8

//define Download item status icon index
#define DOWNLOAD_STATUS_ICON_INDEX_BASE		4
#define READY_FOR_DOWNLOAD_ICON				DOWNLOAD_STATUS_ICON_INDEX_BASE
#define DOWNLOADING_ICON					DOWNLOAD_STATUS_ICON_INDEX_BASE + 1
#define COMPLETED_ICON						DOWNLOAD_STATUS_ICON_INDEX_BASE + 2
#define DOWNLOAD_ERR_ICON					DOWNLOAD_STATUS_ICON_INDEX_BASE + 3
#define PAUSED_ICON							DOWNLOAD_STATUS_ICON_INDEX_BASE + 4
#define SCHEDULED_ICON						DOWNLOAD_STATUS_ICON_INDEX_BASE + 5

//define START STATE
#define START_MANUAL	0
#define START_RIGHTNOW	1
#define START_SCHEDULED	2

//Enum value for resume support
#define UNKNOWN -1
#define SUPPORTRESUME 0
#define UNSUPPORTRESUME 1

//define Item Status
#define ITEM_STATUS_DOWNLOAD_ERROR		DOWNLOAD_ERR_ICON
#define ITEM_STATUS_COMPLETED			COMPLETED_ICON
#define ITEM_STATUS_DOWNLOADING			DOWNLOADING_ICON
#define ITEM_STATUS_PAUSED				PAUSED_ICON
#define ITEM_STATUS_READY				READY_FOR_DOWNLOAD_ICON
#define ITEM_STATUS_SCHEDULED			SCHEDULED_ICON
		
#define MAX_SIM_DOWNLOAD_JETS			10

typedef struct {
	CString m_strSubstitutedURL;
	BYTE	pbUnknown[4];
} SUBSTITUTEDURL, *PSUBSTITUTEDURL;

typedef struct{
	int iStart;
	int iEnd;
} BREAKPOSITION, *PBREAKPOSITION;


class CListItem : public CItemHeader
{
public:
	BOOL IsDirty();
	void SetDirty(BOOL bDirty);
	int GetCountOfJets();
	LPCTSTR GetJetInfo(int iIndex);
	void SetFileStatus(int iStatus);
	void SetFileSize(DWORD dwFileSize);
	CObject *GetBrkPosArray();
	void AddJetInfo(int iIndex, LPCTSTR lpszJetInfo);
	void ClearJetInfo();
	void SetItemCreateTime(time_t tItemCreateTime);
	static LPCTSTR CListItem::GetRenameByURL(LPCTSTR lpszUrl);	
	void SetComment(LPCTSTR lpszComment);
	void SetRename(LPCTSTR lpszRename);
	void AddSubstitutedURL(LPCTSTR lpszSubUrl, int iUnknown = 0);
	void ClearSubstitutedURLs();
	void SetREF(LPCTSTR lpszRefer);
	void SetUrl(LPCTSTR lpszUrl);
	void SetHttpProxyIndex(WORD wIndex);
	void SetFtpProxyIndex(WORD wIndex);
	void SetStartState(WORD wStartState);
	void SetPassword(LPCTSTR lpszPassword);
	void SetUserName(LPCTSTR lpszUserName);
	void SetSplitsCount(int iSplitsCount);
	void SetDownloadedFolder(LPCTSTR lpszFolder);
	void SetDownloadedToCategory(int iCategoryID);
	void SetAttr(int iAttrIndex, BOOL bIsOn = TRUE);
	WORD GetHttpProxyIndex();
	WORD GetFtpProxyIndex();
	WORD GetStartState();
	int GetSplitsCount();
	int GetDownloadedToCategory();
	BOOL IsAttrOn(int iAttrIndex);
	BOOL SaveToFile(CJCDFile *pJCDFile);
	PBREAKPOSITION GetBreakPos(DWORD dwIndex);
	BOOL LoadFromFile(CJCDFile *pJCDFile);
	int GetCountOfBreaks();
	int GetStatus();
	LPCTSTR GetSubstitutedURLs(int iIndex);
	int GetCountOfSubstitutedURLs();
	LPCTSTR GetUserName();
	LPCTSTR GetRename();
	LPCTSTR GetPasswd();
	BOOL IsCompleted();
	int GetFileSize();
	int GetDownloadingTimeCosted();
	int GetFileDownloadedSize();
	time_t GetLocalFileCompletedTime();
	time_t GetLocalFileCreatedTime();
	time_t GetFileTime();
	LPCTSTR GetDownloadedFolder();
	int IsSupportResumed();
	LPCTSTR GetREF();
	LPCTSTR GetURL();
	int GetItemStatusIconIndex();
	LPCTSTR GetComment();
	int GetFileTotalLengthByString(LPTSTR pBuf, int iMaxBufLength);

	CListItem();

	~CListItem()
	{
		prv_FreeMEM();
	};

protected:
	CString m_strURL;
	int		m_iAttribute;
	CString m_strUserName;
	CString m_strPasswd;
	int		m_iDownloadedToCategory;
	CString m_strRename;
	int		m_iConcurrentThreadNumber;
	CString m_strDownloadFolder;
	CString m_strComment;
	WORD	m_wHttpProxyIndex;
	WORD	m_wStartState;
	int		m_iIsFileCompleted;
	int		m_iSizeOfFile;
	BYTE	m_pbUnknown4[1];
	int		m_iFileCreateTime;
	int		m_iItemCreateTime;
	int		m_iItemCompleteTime;
	CString m_strREF;
	int		m_iDownloadingTimeCosted;
	WORD	m_wFtpProxyIndex;
	CArray <PSUBSTITUTEDURL, PSUBSTITUTEDURL> m_arSubStitutedUrl;
	CArray <PBREAKPOSITION, PBREAKPOSITION> m_arBrkPosArray;

	CStringArray m_arJetsInfo;
	int		m_iRetries;
	BOOL	m_bDirty;

private:
	void prv_FreeMEM();
	BOOL prv_InitDefault();

};
#endif