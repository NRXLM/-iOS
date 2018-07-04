#ifndef CLIENT_MODULE_H_H_
#define CLIENT_MODULE_H_H_

#include "localdef.h"

/////////////////////////////////////////////////////////////////////
//define thread state
#define THREAD_WAIT                 0
#define THREAD_PREPARE              1
#define THREAD_OK                   2
#define THREAD_UNPREPARE            3

#define NET_TIMEOUT			60000			//网络测试超时时间

///////////////////////////////////////////////////////////////
//连接状态
enum enSocketState
{
	SocketState_NoConnect,					//没有连接
	SocketState_Connecting,					//正在连接
	SocketState_Connected,					//成功连接
};

///////////////////////////////////////////////////////////////////////
struct IClientSocket
{
	//set sink interface
	virtual bool SetSocketSink(void * pISocketSink)=0;
	//获取网络状态
	virtual enSocketState GetConnectState()=0;
	//conect to server
	virtual bool ConnectToServer(DWORD dwServerIP,WORD wPort) = 0;
	virtual bool ConnectToServer(LPCTSTR lpszServerIP,WORD wPort) = 0;
	//Send data function
	virtual bool SendData(WORD mainCmd,WORD subCmd,DWORD dwSequenceID,void * pData,DWORD dwDataSize) = 0;
	//Close socket
	virtual bool CloseSocket(bool bNotify) = 0;
	//开始网络线程
	virtual bool StartSocketThread()=0;
	//停止网络线程
	virtual bool StopSocketThread()=0;
	
};

struct IClientSocketSink
{
	//网络连接消息
	virtual bool OnSocketConnect(int iErrorCode, LPCTSTR pszErrorDesc, IClientSocket * pIClientSocket)=0;
	//网络读取消息
	virtual bool OnSocketRead(CMD_Command Command, void * pBuffer, DWORD dwDataSize, IClientSocket * pIClientSocket)=0;
	//网络关闭消息
	virtual bool OnSocketClose(IClientSocket * pIClientSocket, bool bCloseByServer)=0;
};

//网络回调管理接口
struct ISockSinkManage
{
	//开始网络服务
	virtual bool StartNetService()=0;
	//停止网络服务
	virtual bool StopNetService()=0;
	//初始化SockSink
	virtual bool InitSocketSinkManage(LPCTSTR lpszLBSvrIp,WORD wLBSvrPort)=0;
	//开始网络连接
	virtual bool ConnectToServer()=0;
	//发送H264数据数据
	virtual bool SendH264Data(const void * pH264Data,WORD wDataSize,H264VTYPE vType)=0;
	//发送文本消息
	virtual bool SendTextData(const void *pTxtData,WORD wDataSize)=0;
	//发送音频数据
	virtual bool SendAudioData(const void * pAudioData,WORD wDataSize)=0;
	//关闭网络连接
	virtual void CloseSocketConnect()=0;
    //释放网络对象
    virtual void FreeSockSinkManage()=0;
	//检测网络连接
	virtual void CheckNetConnect()=0;
	//退出登录
	virtual void LoginOut()=0;
	//登录状态
	virtual bool LoginStatus()=0;
	//设置底层回调接口
	virtual bool SetKernelDataSink(void * pIKernelDataSink)=0;
};





#ifndef __IPHONE_4_0
//底层数据回调接口
struct IKernelDataSink
{
	//消息通知
	virtual bool OnLoginRespNotify(int nResult)=0;
};

#else
//底层数据回调接口
struct IKernelDataSink
{
    //登录通知
    virtual bool OnLoginRespNotify(int nResult)=0;
};

///////////////////////////////////////////////////////////////////
ISockSinkManage *AfxCreateSinkManage();
#endif

#endif
