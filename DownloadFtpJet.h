#if !defined(AFX_DownloadFtpJet_H__BB70E295_43B2_41A6_80DC_5F4C0C89B2BB__INCLUDED_)
#define AFX_DownloadFtpJet_H__BB70E295_43B2_41A6_80DC_5F4C0C89B2BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DownloadFtpJet.h : header file
//
	

#include "Ftp.h"
class CDownloadItemManager;
#include "DownloadJet.h"

/////////////////////////////////////////////////////////////////////////////
// CDownloadFtpJet 

class CDownloadFtpJet;

typedef void (CDownloadFtpJet::*JET_STATE_HANDLER)(SOCKET aSocket);

typedef struct
{
	JET_STATE_HANDLER fStateHandlers[3]; /*fSucceed, fFailed, fError*/;
}JET_STATE_ENTRY, *P_JET_STATE_ENTRY;

class CDownloadFtpJet : public CDownloadJet
{
	//DECLARE_DYNCREATE(CDownloadFtpJet)

public:
	CDownloadFtpJet(CDownloadItemManager *pParent = NULL);           
	virtual ~CDownloadFtpJet();

public:

	// DWONLOADº¯Êý
	BOOL ParseURL(LPCTSTR lpszURL,CString& strServer,CString& strObject,USHORT& nPort);
public:
	virtual void StopDownload();
	virtual void StartDownload();
	virtual void SetProxy(LPCTSTR lpszProxyServer, USHORT nProxyPort,BOOL bProxy=TRUE,BOOL bProxyAuthorization = FALSE,LPCTSTR lpszProxyUsername = NULL,LPCTSTR lpszProxyPassword = NULL,UINT nProxyType = PROXY_HTTPGET);

private:
	int prv_Send(SOCKET hSocket, char const * pszBuffer, int iBufferSize, DWORD dwTimeout);
	void prv_UnregisterTimeout();
	enum	JET_STATUS{
			none = 0,
			connected,
			user_name,
			password,
			rest_0,
			rest_1,
			type_0,
			pasv_0,
			get_list,
			list,
			type_1,
			size,
			type_2,
			pasv_1,
			rest_2,
			retr,
			close} m_JetStatus;
	
	enum REPLY_STATUS
	{
		nSucceed = 0,
		nFailed,
		nError,
		nUncompletedCommand
	};

	void prv_ConnectServer(SOCKET aSocket);
	void prv_Connected(SOCKET aSocket);
	void prv_SentUserName(SOCKET aSocket);
	void prv_SentPassword(SOCKET aSocket);
	void prv_SentRest(SOCKET aSocket);
	void prv_SentType(SOCKET aSocket);
	void prv_SentPasv(SOCKET aSocket);
	void prv_SentList(SOCKET aSocket);
	void prv_SentSize(SOCKET aSocket);
	void prv_SentRetr(SOCKET aSocket);
	void prv_Failed(SOCKET aSocket);
	void prv_Error(SOCKET aSocket);
	void prv_GetList(SOCKET aSocket);
	BOOL GetIPandPort(char *pszIP, UINT *puiPort);

	int prv_GetReplyEndPosition(char *pszBuf, int iBufLen);
	enum REPLY_STATUS prv_CheckReply(WPARAM wParam, LPARAM lParam);

	static JET_STATE_ENTRY m_JetStatesTable[];
	static int			  m_nSizeOfStatesTable;
	//int prv_GetHeader(SOCKET aSocket, char *pszBuffer, int iBufferSize, int* iStatus);
//	UINT	prv_SendRequest(BOOL bHead = FALSE);
	//UINT	prv_GetInfo(LPCTSTR lpszHeader,DWORD& dwContentLength,DWORD& dwStatusCode,CTime& TimeLastModified);
	//CTime	prv_GetTime(LPCTSTR lpszTime);
	void prv_StartDownload();

	int prv_ConnectEx(SOCKET hSocket, char const * pszServer, int nPort,DWORD dwTimeout,BOOL fFixNtDNS = FALSE);

public:
// Operations
public:
	BOOL OwnSocket(SOCKET aSocket);
	void prv_StopDownload();
	virtual void Timeout();
	virtual int DownloadHandler(WPARAM wParam, LPARAM lParam);


private:
	BOOL m_bDataSocketReady;
	BOOL m_bControlSocketReady;
	CFtp m_Ftp;
	//CArray<BYTE, BYTE> m_arData;
	char *m_szBuffer;
	int m_iBufferSize;
	int m_iDataLen;

	SOCKET m_hDataSocket;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DownloadFtpJet_H__BB70E295_43B2_41A6_80DC_5F4C0C89B2BB__INCLUDED_)
