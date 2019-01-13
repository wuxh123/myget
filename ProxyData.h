// ProxyData.h: interface for the CProxyData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROXYDATA_H__A001C8D3_A3B4_486C_9F48_DF80FF478EC3__INCLUDED_)
#define AFX_PROXYDATA_H__A001C8D3_A3B4_486C_9F48_DF80FF478EC3__INCLUDED_

#include "AppRegs.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//define proxy flag
#define AUTH_MASK		1
#define PASV_MASK		2
#define NO_CACHE_MASK	4



//define Proxy type
#define REG_RROXY_TYPE_BASE		0
#define REG_DIR_CONNECTION		REG_RROXY_TYPE_BASE
#define REG_FTP_OPEN_HOST		REG_RROXY_TYPE_BASE + 1
#define REG_FTP_USER_HOST_PORT1	REG_RROXY_TYPE_BASE + 2
#define REG_HTTP_GET			REG_RROXY_TYPE_BASE + 3
#define REG_FTP_USER_HOST_PORT2	REG_RROXY_TYPE_BASE + 4
#define REG_FTP_SITE_HOST		REG_RROXY_TYPE_BASE + 5
#define REG_FTP_SITE_USER_HOST	REG_RROXY_TYPE_BASE + 6
#define REG_DIR_SOCKS5			REG_RROXY_TYPE_BASE + 7
#define REG_DIR_HTTP_CONNECT	REG_RROXY_TYPE_BASE + 8


class CProxyData  
{
public:
	PPROXYSTRUCT GetProxyStruct();
	void SetPassword(LPCTSTR pszPassword);
	LPTSTR GetPassword();
	void SetUser(LPCTSTR pszUserName);
	LPTSTR GetUser();
	void SetTitle(LPCTSTR pszTitle);
	LPTSTR GetTitle();
	void SetServer(LPCTSTR pszServer);
	LPTSTR GetServer();
	void SetType(DWORD dwType);
	DWORD GetType();
	void SetPort(DWORD dwPort);
	UINT GetPort();
	void SetFlag(DWORD dwFlag);
	DWORD GetFlag();
	CProxyData();
	virtual ~CProxyData();

protected:
	PPROXYSTRUCT m_pProxyStruct;
};

#endif // !defined(AFX_PROXYDATA_H__A001C8D3_A3B4_486C_9F48_DF80FF478EC3__INCLUDED_)
