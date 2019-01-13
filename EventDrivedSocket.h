// EventDrivedSocket.h: interface for the CEventDrivedSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENTDRIVEDSOCKET_H__37BC01BD_0382_4766_80F3_548A1D4638E7__INCLUDED_)
#define AFX_EVENTDRIVEDSOCKET_H__37BC01BD_0382_4766_80F3_548A1D4638E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int SOCKET_EVENT = 0;
const int WRITE_EVENT = 1;
const int QUIT_EVENT = 2;

class CEventDrivedSocket;

typedef struct
{
	HANDLE hEvents[3];
	HANDLE hSocket;
	CEventDrivedSocket *pEDS;
}SOCKET_THREAD_STRUCT, *P_SOCKET_THREAD_STRUCT;


UINT SOCKET_THREAD_PROC(LPVOID lpvArg);

class CEventDrivedSocket  
{
public:
	virtual BOOL StartMonitor();
	virtual BOOL WriteData(BYTE *pbBuffer, UINT nDataCount);
	CEventDrivedSocket(LPCTSTR lpszHost, SHORT nPort);
	virtual ~CEventDrivedSocket();

protected:
	virtual BOOL StopMonitor();
	virtual BOOL OnRead();

private:
	virtual BOOL prv_WriteData();

private:
	HANDLE m_hevQuit;
	HANDLE m_hevWrite;
	HANDLE m_hevSocket;
	HANDLE m_hSocket;

	CString m_strHost;
	SHORT m_nPort;
};

#endif // !defined(AFX_EVENTDRIVEDSOCKET_H__37BC01BD_0382_4766_80F3_548A1D4638E7__INCLUDED_)
