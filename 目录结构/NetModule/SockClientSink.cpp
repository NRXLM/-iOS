///////////////////////////////////////////////////////////////////////////////////
//			网络回调基类实现文件 SockClientSink.cpp
//			2014-3-12 hubo
///////////////////////////////////////////////////////////////////////////////////
#include "SockClientSink.h"
#include "custom_log.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
///////////////////////////////////////////////////////////////////////////////////
CSockClientSink::CSockClientSink(void)
{
	m_dwRecvTime=0;
	m_pIClientSocket=NULL;
}


CSockClientSink::~CSockClientSink(void)
{
}


//初始化回调类
bool CSockClientSink::InitSocketSink()
{
	m_pIClientSocket=(IClientSocket *)&m_ClientSocket;
	IClientSocketSink *pIClientSocketSink=(IClientSocketSink *)this;
	if(!m_ClientSocket.SetSocketSink(pIClientSocketSink)) 
	{
		return false;
	}

	return true;
}

//心跳处理
bool CSockClientSink::OnProcActiveTest(IClientSocket * pIClientSocket)
{
	NetMsgNotify("+++++++++++>>>>-------Active Test");
	if(pIClientSocket==NULL) return false;

	m_dwRecvTime=::GetTickCount();
	pIClientSocket->SendData(ALL_MAIN_CMD,ACITVE_TEST_RESP,0,NULL,0);

	return true;
}

//测试连接
bool CSockClientSink::TestNetConnect()
{
	DWORD dwRecvTime=m_dwRecvTime;
	DWORD dwTime=::GetTickCount();
	dwRecvTime=dwTime-dwRecvTime;
	if(dwRecvTime>=NET_TIMEOUT)
	{
		m_dwRecvTime=dwTime;
		m_ClientSocket.CloseSocket(false);
		return true;
	}

	return false;
}

//日志打印
void NetMsgNotify(LPCTSTR lpszMsg,ITEOPTIONTYPE nOptType)
{
#if IS_ANDROID
	if(OI_EXCEPTION==nOptType)
		LOGE(lpszMsg);
	else
		LOGD(lpszMsg);
#else
	printf("%s\n",lpszMsg);
#endif
}

//消息通知
void NetMsgAutoParam(ITEOPTIONTYPE nOptType,LPCTSTR lpszFormate,...)
{
	TCHAR szMsg[MSG_BUFF_SIZE];
	va_list arglist;
	va_start(arglist,lpszFormate);
	#ifdef _UNICODE
	vswprintf_s(szMsg,MSG_BUFF_SIZE,lpszFormate,arglist);
	#else
	vsprintf(szMsg,lpszFormate,arglist);
	#endif
	va_end(arglist);

	NetMsgNotify(szMsg,nOptType);
}
