#if !defined(AFX_DownloadHttpJet_H__BB70E295_43B2_41A6_80DC_5F4C0C89B2BB__INCLUDED_)
#define AFX_DownloadHttpJet_H__BB70E295_43B2_41A6_80DC_5F4C0C89B2BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DownloadHttpJet.h : header file
//
	




class CDownloadItemManager;
#include "DownloadJet.h"

/////////////////////////////////////////////////////////////////////////////
// CDownloadHttpJet 

class CDownloadHttpJet : public CDownloadJet
{
	//DECLARE_DYNCREATE(CDownloadHttpJet)

public:
	CDownloadHttpJet(CDownloadItemManager *pParent = NULL);           
	virtual ~CDownloadHttpJet();

public:
	// 静态函数，用于64编码、解码
	static int Base64Encode( LPCTSTR lpszEncoding, CString& strEncoded );
	static int Base64Decode( LPCTSTR lpszDecoding, CString& strDecoded );

	// DWONLOAD函数
	BOOL ParseURL(LPCTSTR lpszURL,CString& strServer,CString& strObject,USHORT& nPort);
public:
	virtual void StopDownload();
	virtual void StartDownload();
	virtual void SetProxy(LPCTSTR lpszProxyServer, USHORT nProxyPort,BOOL bProxy=TRUE,BOOL bProxyAuthorization = FALSE,LPCTSTR lpszProxyUsername = NULL,LPCTSTR lpszProxyPassword = NULL,UINT nProxyType = PROXY_HTTPGET);

private:
	int prv_Send(SOCKET hSocket, char const * pszBuffer, int iBufferSize, DWORD dwTimeout);
	void prv_UnregisterTimeout();
	enum	JET_STATUS{none,
			connect,
			sent_request,
			receive,
			close} m_JetStatus;

	enum CDownloadHttpJet::JET_STATUS prv_ConnectServer();
	enum CDownloadHttpJet::JET_STATUS prv_SendRequest(BOOL bHead = FALSE);
	enum CDownloadHttpJet::JET_STATUS prv_ReceiveHeader();
	enum CDownloadHttpJet::JET_STATUS prv_ReceiveData();
	enum CDownloadHttpJet::JET_STATUS prv_Failed(int iErrorCode);
	int prv_GetHeader(SOCKET Socket, char *pszBuffer, int iBufferSize, int* iStatus);
//	UINT	prv_SendRequest(BOOL bHead = FALSE);
	UINT	prv_GetInfo(LPCTSTR lpszHeader,DWORD& dwContentLength,DWORD& dwStatusCode,CTime& TimeLastModified);
	CTime	prv_GetTime(LPCTSTR lpszTime);
	enum JET_STATUS prv_StartDownload();

	int prv_ConnectEx(SOCKET hSocket, char const * pszServer, int nPort,DWORD dwTimeout,BOOL fFixNtDNS = FALSE);
	// 用于BASE64编码、解码
	static UINT		m_nBase64Mask[];
	static CString	m_strBase64TAB;

public:
// Operations
public:
	void prv_StopDownload();
	virtual void Timeout();
	virtual int DownloadHandler(WPARAM wParam, LPARAM lParam);



};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DownloadHttpJet_H__BB70E295_43B2_41A6_80DC_5F4C0C89B2BB__INCLUDED_)
