// DownloadJet.h: interface for the CDownloadJet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOWNLOADJET_H__DB5BA134_790C_4602_878C_D91939CEE761__INCLUDED_)
#define AFX_DOWNLOADJET_H__DB5BA134_790C_4602_878C_D91939CEE761__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TE_Socket.h"
#include "ListItem.h"

//自定义下载线程通信消息
#define WM_USER_DOWNLOAD_THREAD_MSG_BASE		WM_USER + 500

//从下载线程发来的状态信息。
#define WM_USER_DOWNLOAD_THREAD_GENERAL_INFO	WM_USER_DOWNLOAD_THREAD_MSG_BASE + 0	

//下载线程开始接收数据。
#define WM_USER_DOWNLOAD_THREAD_START_RECV		WM_USER_DOWNLOAD_THREAD_MSG_BASE + 1	

//下载线程完成当前指定范围的任务。
#define WM_USER_DOWNLOAD_THREAD_COMPLETED		WM_USER_DOWNLOAD_THREAD_MSG_BASE + 2	

//下载线程出现不可恢复的错误。
#define WM_USER_DOWNLOAD_THREAD_ERROR			WM_USER_DOWNLOAD_THREAD_MSG_BASE + 3	

//下载线程失败，但仍可以重试。
#define WM_USER_DOWNLOAD_THREAD_FAILED			WM_USER_DOWNLOAD_THREAD_MSG_BASE + 4	

//下载线程接受数据成功
#define WM_USER_DOWNLOAD_THREAD_RECV_OK			WM_USER_DOWNLOAD_THREAD_MSG_BASE + 5	

//下载线程被用户取消
#define WM_USER_DOWNLOAD_THREAD_CANCELED		WM_USER_DOWNLOAD_THREAD_MSG_BASE + 6	


//通信信息类别
#define STATE_MSG								0
#define COMMAND_MSG								1
#define ERROR_MSG								2
#define SERVER_MSG								3

// 结构定义
typedef struct _tagDownloadStatus
{
	UINT	nStatusType;
	DWORD	dwFileSize;
	DWORD	dwFileDownloadedSize;
	CString	strFileName;
//	UINT	nErrorCount;	
//	CString strError;
//	DWORD	dwErrorCode;
}DOWNLOADSTATUS,*PDOWNLOADSTATUS;

// 常量定义
// 缺省超时参数
const DWORD DOWNLOAD_CONNECT_TIMEOUT= 10*1000;// 10秒
const DWORD DOWNLOAD_SEND_TIMEOUT	= 10*1000;// 10秒
const DWORD DOWNLOAD_RECV_TIMEOUT	= 10*1000;// 10秒

// 下载结果
const UINT	DOWNLOAD_RESULT_SUCCESS	= 0;	// 成功
const UINT	DOWNLOAD_RESULT_SAMEAS	= 1;	// 要下载的文件已经存在并且与远程文件一致，不用下载
const UINT	DOWNLOAD_RESULT_STOP	= 2;	// 中途停止(用户中断)
const UINT	DOWNLOAD_RESULT_FAIL	= 3;	// 下载失败

// 发送请求
const UINT SENDREQUEST_SUCCESS	= 0; // 成功
const UINT SENDREQUEST_ERROR	= 1; // 一般网络错误，可以重试
const UINT SENDREQUEST_STOP		= 2; // 中途停止(用户中断) (不用重试)
const UINT SENDREQUEST_FAIL		= 3; // 失败 (不用重试)	 

// 消息
const WPARAM	MSG_INTERNET_STATUS	= (WPARAM)1;
const WPARAM	MSG_DOWNLOAD_STATUS = (WPARAM)2;
const WPARAM	MSG_DOWNLOAD_RESULT = (WPARAM)3;
const WPARAM	MSG_DOWNLOAD_MAX	= (WPARAM)32; //保留最大可供扩充

//下载状态
const UINT		STATUS_TYPE_FILENAME			= 1;
const UINT		STATUS_TYPE_FILESIZE			= 2;
const UINT		STATUS_TYPE_FILEDOWNLOADEDSIZE	= 3;
const UINT		STATUS_TYPE_ERROR_COUNT			= 4;
const UINT		STATUS_TYPE_ERROR_CODE			= 5;
const UINT		STATUS_TYPE_ERROR_STRING		= 6;

// 重试类别
const UINT RETRY_TYPE_NONE	= 0;
const UINT RETRY_TYPE_TIMES	= 1;
const UINT RETRY_TYPE_ALWAYS= 2;
//缺省的重试次数
const UINT DEFAULT_RETRY_MAX= 10; 


// HTTP STATUS CODE分类
const UINT	HTTP_OK			= 0;
const UINT	HTTP_ERROR		= 1;
const UINT	HTTP_REDIRECT	= 2;
const UINT	HTTP_FAIL		= 3;

// PROXY的类型
const UINT	PROXY_NONE			= 0;
const UINT	PROXY_HTTPGET		= 1;
const UINT	PROXY_HTTPCONNECT	= 2;
const UINT	PROXY_SOCKS4		= 3;
const UINT	PROXY_SOCKS4A		= 4;
const UINT	PROXY_SOCKS5		= 5;




class CDownloadItemManager;
class CDownloadJet  
{
public:
	CDownloadJet();
	virtual ~CDownloadJet();

	//缓冲大小 10K
	enum { download_buffer_size = (64 * 1024) };

public:
	virtual BOOL OwnSocket(SOCKET socket) {return (socket == m_hSocket);};
	virtual void Timeout() = 0;
	int GetSocket();
	virtual int DownloadHandler(WPARAM wParam, LPARAM lParam) =0;
	void SetForceDownload(BOOL bForceDownload);
	virtual void SetURL(LPCTSTR lpszDownloadUrl);
	virtual void StopDownload() = 0;
	virtual void StartDownload() = 0;

	BOOL IsSupportResume();
	PBREAKPOSITION GetRange();
	DWORD GetCurDownloadedSize();
	LPCTSTR GetDownloadBuf();
	DWORD GetCurPos();
	DWORD GetContentLength();
	void SetRange(PBREAKPOSITION pBrkPos);
	LPCTSTR GetHeaderInfo();
	void SetAuthorization(LPCTSTR lpszUsername,LPCTSTR lpszPassword,BOOL bAuthorization = TRUE );
	void SetReferer(LPCTSTR lpszReferer);
	void SetUserAgent(LPCTSTR lpszUserAgent);
	void SetTimeout(DWORD dwSendTimeout,DWORD dwReceiveTimeout,DWORD dwConnectTimeout);
	void SetRetry(UINT nRetryType, UINT nRetryDelay=0,UINT nRetryMax = 0);
	virtual void SetProxy(LPCTSTR lpszProxyServer, USHORT nProxyPort,BOOL bProxy=TRUE,BOOL bProxyAuthorization = FALSE,LPCTSTR lpszProxyUsername = NULL,LPCTSTR lpszProxyPassword = NULL,UINT nProxyType = PROXY_HTTPGET) = 0;

protected:
	// 下载参数
	// 待下载URL
	CString		m_strDownloadUrl;

	// 停止下载
	BOOL		m_bStopDownload;

	// 强制重新下载(不管已有的文件是否与远程文件相同)
	BOOL		m_bForceDownload;

	// 是否支持断点续传
	BOOL		m_bSupportResume;

	// 下载大小
	int			m_iDownloadedBufSize;			// 本次已经下载的大小

	PBREAKPOSITION m_pBrkPos;
	char		m_szReadBuf[CDownloadJet::download_buffer_size+1];

	DWORD		m_dwHeaderSize;				// 返回头的大小
	CString		m_strHeader;				// 保存头部信息

	// 文件日期(远程文件的信息)
	CTime		m_TimeLastModified;

	// Referer
	CString		m_strReferer;
	
	// UserAgent
	CString		m_strUserAgent;

	// 超时TIMEOUT	连接超时、发送超时、接收超时(单位：毫秒)
	DWORD		m_dwConnectTimeout;	
	DWORD		m_dwReceiveTimeout;
	DWORD		m_dwSendTimeout;

	// 重试机制
	UINT		m_nRetryType;	//重试类型(0:不重试 1:重试一定次数 2:总是重试)
	UINT		m_nRetryTimes;	//重试次数
	UINT		m_nRetryDelay;	//重试延迟(单位：毫秒)
	UINT		m_nRetryMax;    //重试的最大次数

	// 错误处理
	UINT		m_nErrorCount;	//错误次数
	CString		m_strError;		//错误信息

	// 是否进行验证 : Request-Header: Authorization
	BOOL		m_bAuthorization;
	CString		m_strUsername;
	CString		m_strPassword;

	// 是否使用代理 
	BOOL		m_bProxy;
	CString		m_strProxyServer;
	USHORT		m_nProxyPort;
	UINT		m_nProxyType;
	
	// 代理是否需要验证: Request-Header: Proxy-Authorization
	BOOL		m_bProxyAuthorization;
	CString		m_strProxyUsername;
	CString		m_strProxyPassword;


	// 下载过程中所用的变量
	CString		m_strServer;
	CString		m_strObject;
	USHORT		m_nPort;

	SOCKET		m_hSocket;	// 下载连接的SOCKET

	CDownloadItemManager *m_pParent;
	DWORD		m_dwContentLength;

protected:	
	void SendCancelMsg();
	void SendResultFailedMsg();
	CString EncapsulateJetInfo(int iType, LPCTSTR lpszContent);
	void SendHeaderInfoMsg(int iType);
	BOOL SendStartDownloadMsg();
	void SendCompletedMsg();
	void SendFailedMsg();
	void SendRecvOKMsg();

};

// 缺省端口号
const USHORT DEFAULT_HTTP_PORT = 80;
const USHORT DEFAULT_HTTPS_PORT= 443;
const USHORT DEFAULT_FTP_PORT  = 21;
const USHORT DEFAULT_SOCKS_PORT= 1080;

#endif // !defined(AFX_DOWNLOADJET_H__DB5BA134_790C_4602_878C_D91939CEE761__INCLUDED_)
