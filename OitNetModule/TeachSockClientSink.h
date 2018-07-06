///////////////////////////////////////////////////////////////////////////////////
//			教学网络回调类头文件 TeachSockClientSink.h
//			2014-3-12 hubo
///////////////////////////////////////////////////////////////////////////////////
#ifndef __BOWEN_HU_TEACH_SOCK_CLIENT_SINK_HEAD_FILE__
#define __BOWEN_HU_TEACH_SOCK_CLIENT_SINK_HEAD_FILE__

#include "SockClientSink.h"

//////////////////////////////////////////////////////////////////////////////////
class CTeachSockClientSink: public CSockClientSink
{
private:
	MYBOOL						m_bLogined;					//登录标志
	IBlackboardManage			*m_pINetDataWndSink;		//窗口接口数据回调
	IMediaDataManage			*m_pIMediaDataManage;		//媒体数据管理接口

public:
	CTeachSockClientSink(void);
	~CTeachSockClientSink(void);

public:
	//初始化教学网络客户端
	bool InitTeachSockClient(IBlackboardManage *pINetDataWndSink,IMediaDataManage *pIMediaDataManage);
	//连接到教学服务器
	bool ConnectToTeachServer();
	//关闭连接
	bool CloseTeachConnect();

	//回调接口
public:
	//网络连接消息
	virtual bool OnSocketConnect(int iErrorCode, LPCTSTR pszErrorDesc, IClientSocket * pIClientSocket);
	//网络读取消息
	virtual bool OnSocketRead(CMD_Command Command, void * pBuffer,WORD wDataSize, IClientSocket * pIClientSocket);
	//网络关闭消息
	virtual bool OnSocketClose(IClientSocket * pIClientSocket, bool bCloseByServer);

private:
	//登录到教学服务器
	bool LoginToTeachServer();

private:
	//处理登录Response
	bool OnProcLoginSvrResp(DWORD dwSequenceID,void *pDataBuffer,WORD wDataSize);
	//处理学生音频数据
	bool OnProcStudentAudioData(DWORD dwSequenceID,void *pDataBuffer,WORD wDataSize);
	//处理学生黑板数据
	bool OnProcStudentBoardData(WORD wSubCmd,DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize);
	//处理操作事件数据
	bool OnProcOperateEventData(WORD wSubCmd,DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize);
	//处理视频H264SpsPps数据
	bool OnProcH264SpsPpsData(void *pDataBuffer,DWORD dwDataSize);
	//处理用户视频数据
	bool OnProcUserVideoData(DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize);
	//处理教师退出教室
	bool OnProcTeacherExitRoom(DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize);
	#if TEST_RECV_DAT
	//处理教师端视频数
	bool OnProcTeacherVideoData(DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize);
	#endif
};

#endif //__BOWEN_HU_TEACH_SOCK_CLIENT_SINK_HEAD_FILE__
