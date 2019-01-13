#ifndef __TE__SOCKET__H__INCLUDED__
#define __TE__SOCKET__H__INCLUDED__


//包含必要的头文件	
#include <winsock2.h>			// Winsock2

//宏
#define Min(a, b)               (((a) < (b)) ? (a): (b))
#define Max(a, b)               (((a) > (b)) ? (a): (b))
#define INext(i, n)             ((((i) + 1) < (n)) ? ((i) + 1): 0)
#define IPrev(i, n)             (((i) > 0) ? ((i) - 1): ((n) - 1))

// 常量定义
const int TE_SOCKET_BUFFER_SIZE			= 4096;// 缓冲区缺省大小
const int TE_EOF						= 0x100;// 无效接收字符
const int SOCKET_SUCCESS				= 0;
const int TE_SOCKET_MAJOR_VERSION		= 2;
const int TE_SOCKET_MINOR_VERSION		= 2;

// 缺省超时参数 (超时单位：毫秒 )
const DWORD	TE_SHUTDOWN_RECV_TIMEOUT	= 4*1000;  // 优雅关闭时延迟4秒
const DWORD TE_BLOCKED_SNDRCV_SLEEP		= 100;	   // 100毫秒(发生阻塞时等待[睡眠]时间)
const DWORD TE_DEFAULT_TIMEOUT			= 120*1000;// 120秒 缺省超时
const DWORD TE_CONNECT_TIMEOUT			= 120*1000;// 120秒 连接超时
const DWORD TE_SEND_TIMEOUT				= 120*1000;// 120秒 发送超时
const DWORD TE_RECV_TIMEOUT				= 120*1000;// 120秒 接收超时 

// 数据结构(缓冲区循环使用)
typedef struct _tagBufSocketData
{
    SOCKET			hSocket;			// SOCKET
    int             iBufferSize;		// 数据缓冲大小
    char*           pszBuffer;			// 数据缓冲
    int             iBytesInBuffer;		// 缓冲区已有数据长度(字节)
    int             iReadIndex;			// 可以读取的下一缓冲区位置
    int             iBufferIndex;		// 可以使用的下一缓冲区位置	
}BSD,*PBSD;

// 获取及设置错误/内存管理辅助函数
int		TE_GetLastError(void);
void	TE_SetLastError(int iErrorCode);

// SOCKET函数 (基于Winsock2)
int		TE_InitLibrary();
void	TE_CleanupLibrary();
SOCKET	TE_CreateSocket(int iAddressFamily = AF_INET, 
						int iType	       = SOCK_STREAM, 
						int iProtocol	   = 0);
void	TE_CloseSocket(SOCKET hSocket, BOOL bHardClose = FALSE);
int     TE_SetSocketOption(SOCKET hSocket);
int     TE_RecvLL(SOCKET hSocket, char *pszBuffer, int iBufferSize);
int		TE_RecvData(SOCKET hSocket, char *pszBuffer, int iBufferSize, DWORD dwTimeout = TE_RECV_TIMEOUT);
int		TE_Recv(SOCKET hSocket, char *pszBuffer, int iBufferSize, DWORD dwTimeout = TE_RECV_TIMEOUT);
int     TE_SendLL(SOCKET hSocket, char const * pszBuffer, int iBufferSize);
int		TE_SendData(SOCKET hSocket, char const * pszBuffer, int iBufferSize, DWORD dwTimeout = TE_SEND_TIMEOUT);
int		TE_Send(SOCKET hSocket, char const * pszBuffer, int iBufferSize, DWORD dwTimeout = TE_SEND_TIMEOUT);
int		TE_Connect(SOCKET hSocket, const struct sockaddr * pSockName, int iNameLen,DWORD dwTimeout = TE_CONNECT_TIMEOUT);
int		TE_BindSocket(SOCKET hSocket, const struct sockaddr * SockName, int iNameLen);
int		TE_ListenSocket(SOCKET hSocket, int iConnections);
SOCKET	TE_Accept(SOCKET hSocket, struct sockaddr * pSockName, int *iNameLen,DWORD dwTimeout = TE_DEFAULT_TIMEOUT);
SOCKET	TE_AcceptEx(SOCKET hSocket, struct sockaddr * pSockName, int *iNameLen,HANDLE hEndEvent,DWORD dwTimeout = TE_DEFAULT_TIMEOUT);
int     TE_BlockSocket(SOCKET hSocket, BOOL bBlock);
int		TE_RecvDataFrom(SOCKET hSocket, struct sockaddr * pFrom, int iFromlen,
						char *pszBuffer, int iBufferSize, DWORD dwTimeout = TE_RECV_TIMEOUT);
int		TE_SendDataTo(SOCKET hSocket, const struct sockaddr * pTo,int iToLen,
					  char const * pszBuffer, int iBufferSize, DWORD dwTimeout = TE_RECV_TIMEOUT);

// 扩展函数
int		TE_BindSocketEx(SOCKET hSocket,int nPort);
int		TE_ConnectEx(SOCKET hSocket, char const * pServer, int nPort,DWORD dwTimeout = TE_CONNECT_TIMEOUT,BOOL fFixNtDNS = FALSE);

// BufSocket函数
PBSD	TE_BSocketAttach(SOCKET hSocket, int iBufferSize = TE_SOCKET_BUFFER_SIZE);
SOCKET	TE_BSocketDetach(PBSD pBSD,  BOOL bCloseSocket = FALSE);
int		TE_BSocketReadData(PBSD pBSD, DWORD dwTimeout = TE_RECV_TIMEOUT);
int		TE_BSocketGetChar(PBSD pBSD, DWORD dwTimeout = TE_RECV_TIMEOUT);
int		TE_BSocketGetString(PBSD pBSD, char *pszBuffer, int iBufferSize,int* iStatus, DWORD dwTimeout = TE_RECV_TIMEOUT);
int		TE_BSocketSendString(PBSD pBSD, const char *pszBuffer, DWORD dwTimeout = TE_SEND_TIMEOUT);
SOCKET	TE_BSocketGetAttachedSocket(PBSD pBSD);
int		TE_BSocketGetStringEx(PBSD pBSD, char *pszBuffer, int iBufferSize, int* iStatus, DWORD dwTimeout = TE_RECV_TIMEOUT);
int		TE_BSocketGetData(PBSD pBSD, char *pszBuffer, int iBufferSize,DWORD dwTimeout = TE_RECV_TIMEOUT);

// 其他
DWORD	TE_GetIP(const char* name,BOOL fFixNtDNS  = FALSE); // Used to Fix NT DNS Problem

#endif	//__TE__SOCKET__H__INCLUDED__
