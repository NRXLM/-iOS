///////////////////////////////////////////////////////////////////////////////////
//			网络回调基类头文件 SockClientSink.h
//			2014-3-12 hubo
///////////////////////////////////////////////////////////////////////////////////
#ifndef __BOWEN_HU_SOCK_CLIENT_SINK_HEAD_FILE__
#define __BOWEN_HU_SOCK_CLIENT_SINK_HEAD_FILE__

#include "ClientSocket.h"

//////////////////////////////////////////////////////////////////////////////////
class CSockClientSink: public IClientSocketSink 
{
protected:
	CClientSocket				m_ClientSocket;					//连接对象
	IClientSocket				*m_pIClientSocket;				//网络连接接口

protected:
	DWORD						m_dwRecvTime;					//最后一次接收时间

public:
	CSockClientSink(void);
	~CSockClientSink(void);


public:
	//初始化回调类
	bool InitSocketSink();
	//获取网络接口
	IClientSocket *GetSocketInterface(){return m_pIClientSocket;}
	//测试连接
	bool TestNetConnect();

    
    
protected:
	//心跳处理
	bool OnProcActiveTest(IClientSocket * pIClientSocket);
};

#endif //__BOWEN_HU_SOCK_CLIENT_SINK_HEAD_FILE__
