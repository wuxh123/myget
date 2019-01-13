// EventDrivedSocket.cpp: implementation of the CEventDrivedSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "myget.h"
#include "EventDrivedSocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEventDrivedSocket::CEventDrivedSocket(LPCTSTR lpszHost, SHORT nPort)
{
	m_strHost = lpszHost;
	m_nPort = nPort;
}

CEventDrivedSocket::~CEventDrivedSocket()
{

}

BOOL CEventDrivedSocket::OnRead()
{

	return TRUE;
}

BOOL CEventDrivedSocket::WriteData(BYTE *pbBuffer, UINT nDataCount)
{

	return TRUE;
}

BOOL CEventDrivedSocket::prv_WriteData()
{

	return FALSE;
}




UINT SOCKET_THREAD_PROC(LPVOID lpvArg)
{
	P_SOCKET_THREAD_STRUCT pSTS = (P_SOCKET_THREAD_STRUCT)lpvArg;


	return 0;
}

BOOL CEventDrivedSocket::StartMonitor()
{

	return TRUE;
}

BOOL CEventDrivedSocket::StopMonitor()
{

	return TRUE;
}
