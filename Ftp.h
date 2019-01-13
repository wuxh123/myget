// Ftp.h: interface for the CFtp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTP_H__71290CDE_05DC_44BC_9230_34C09BE43FEA__INCLUDED_)
#define AFX_FTP_H__71290CDE_05DC_44BC_9230_34C09BE43FEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock.h>

#define MAX_LINE_LEN 1024

#define STATUS_SUCCESS	0
#define STATUS_ERROR	1
#define STATUS_FAILED	2

#define SERVER_PORT 21


class CFtp  
{

public:
	int PWD();
	int MakeDir(const char *lpszDir);
	int RemoveDir(const char *lpszDir);
	int Port(const char *lpszHost, unsigned short uPort = SERVER_PORT);
	int Quit();
	int Passive();
	int NOOP();
	int CDUP();
	int ChangeWorkingDir(const char *lpszWorkingDir);
	int DeleteFile(const char *lpszFilePath);
	int Abort();
	int Help();
	int SetType(char chType);
	int Login(const char *lpszUserName, const char *lpszPassword);
	void GetIPandPort();
	int List();
	void SetControlFunc(void *pVoidData, BOOL (*pfn)(void *pVoidData, const char *lpszBuf));
	void SetDataFunc(void *pVoidData, BOOL (*pfn)(void *pVoidData, const char *lpszBuf));
	SOCKET GetCtrlSocket() {return m_socketCtrl;};
	void SetCtrlSocket(SOCKET socket) {m_socketCtrl = socket;};
	SOCKET GetDataSocket(){return m_socketData;};
	void SetDataSocket(SOCKET socket){m_socketData = socket;};

public:
	int CWD(CString str);
	CFtp();
	virtual ~CFtp();

	int ConnectToServer(const char *lpszServerIP, unsigned short uPort = 21);
public:
	DWORD m_dwTransDirectionFlag;
	char			m_szBuf[MAX_LINE_LEN];

protected:
	SOCKET			m_socketCtrl;
	SOCKET			m_socketData;
	char			m_szDataIP[20];
	unsigned short	m_uDataPort;
	char			*m_pData;
	DWORD			m_dwDataBufSize;
	DWORD			m_dwDataCount;
	BOOL			(*m_pControlFunc)(void *pVoidData, const char *lpszBuf);	
	BOOL			(*m_pDataFunc)(void *pVoidData, const char *lpszBuf);	
	void			*m_pControlFuncData;
	void			*m_pDataFuncData;
	


private:
	int RecvBuffer();
	int SendBuffer();
	int CheckReplyCode(const char *lpszBuf = NULL);
	int SendGeneralCommand(BOOL bCanWait = FALSE);

};

#endif // !defined(AFX_FTP_H__71290CDE_05DC_44BC_9230_34C09BE43FEA__INCLUDED_)
