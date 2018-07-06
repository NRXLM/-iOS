//////////////////////////////////////////////////////////////////////////////////////
//		控制服务器网络客户端回调类头文件 CtrlSockClientSink.h
//		bowen 2014-2-20
///////////////////////////////////////////////////////////////////////////////////////
#ifndef __BOWEN_HU_MSG_SOCKET_CLIENT_SINK_HEAD_FILE__
#define __BOWEN_HU_MSG_SOCKET_CLIENT_SINK_HEAD_FILE__

#include "SockClientSink.h"

//////////////////////////////////////////////////////////////////////////////////////
class CMsgSockClientSink: public CSockClientSink
{
private:
	MYBOOL						m_bLogining;					//登录标志
	CONNECTSVRTYPE				m_cst;							//连接服务器类型

private:
	IKernelDataSink				*m_pIKernelDataSink;				//消息回调接口
	ISockSinkManage				*m_pISockSinkManage;			//管理接口
	IMediaDataManage			*m_pIMediaDataManage;		//媒体数据管理接口

public:
	void						*m_pTimerHandle;				//消息服务器定时器
	void						*m_pTeachSvrTime;				//教学服务器定时器
	int							m_nTimerID;						//定时器ID

public:
	CMsgSockClientSink(void);
	~CMsgSockClientSink(void);

public:
	//初始化控制网络接口
	bool InitMsgSocketClient(ISockSinkManage *pISockSinkManage,IMediaDataManage *pIMediaDataManage);
	//连接到服务器
	bool ConnectToServer(CONNECTSVRTYPE cst);
	//设置聊天接口
	bool SetKernelDataSink(IKernelDataSink *pIKernelDataSink);
	//关闭连接
	bool CloseMsgConnect();

	//回调接口
public:
	//网络连接消息
	virtual bool OnSocketConnect(int iErrorCode, LPCTSTR pszErrorDesc, IClientSocket * pIClientSocket);
	//网络读取消息
	virtual bool OnSocketRead(CMD_Command Command,void * pBuffer,WORD wDataSize,IClientSocket * pIClientSocket);
	//网络关闭消息
	virtual bool OnSocketClose(IClientSocket *pIClientSocket, bool bCloseByServer);

private:
	//请求服务器地址
	bool RequestSvrIPPort();
	//登录到登录服务器
	bool LoginToLoginSvr();
	//登录到消息服务器
	bool LoginToMsgSvr();
	//处理Json消息
	bool OnProcJsonMsg(int nMsgType,LPCSTR lpszUtf8Txt,int nMsgSize);
	//获取定时器ID
	int GetTimerID(){return m_nTimerID;}

public:
	static bool OnMyTimer(void *pObject);
	static bool OnTeachSvrTime(void *pObject);
	static bool OnConnectMsgSvr(void *pObject);
	

private:
	//处理登录Response
	bool OnProcLoginResponse(DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize);
	//处理登录到消息服务器Response
	bool OnProcLoginMsgSvrResp(DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize);
	//处理请求服务器地址Response
	bool OnProcRequestSvrIPPortResp(DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize);
	//处理服务器转发消息
	bool OnProcServerSwitchMsg(WORD wCmdID,DWORD dwSequenceID,const void *pDataBuffer,WORD wDataSize);
	//处理发送消息Response
	bool OnProcSendMsgResp(DWORD dwSequenceID,const void *pDataBuffer,WORD wDataSize);

private:
	//处理教师进入教室
	bool OnProcTeacherInRoom(LPCTSTR lpszJsonTxt);
	//处理学生进入教室
	bool OnProcStudentInRoom(LPCTSTR lpszJsonTxt);

};

#endif //__BOWEN_HU_MSG_SOCKET_CLIENT_SINK_HEAD_FILE__
