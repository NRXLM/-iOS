//////////////////////////////////////////////////////////////////////////////////////
//		网络客户端回调管理类头文件 SocketSinkManage.h
//		bowen 2014-2-20
//////////////////////////////////////////////////////////////////////////////////////
#ifndef __BOWEN_HU_SOCKET_SINK_MANAGE_HEAD_FILE__
#define __BOWEN_HU_SOCKET_SINK_MANAGE_HEAD_FILE__

#include "MsgSockClientSink.h"
#include "TeachSockClientSink.h"
#include "BoardEventData.h"
#include "MediaDataManage.h"
#include "../PlayCwdFile/PlayCWThread.h"

#define HAVE_BBEVETN		1				//有黑板事件处理对象
//////////////////////////////////////////////////////////////////////////////////////
class CSocketSinkManage: public ISockSinkManage
#ifdef ANDROID_OS
	,public IAudioDataSink
#endif
{
private:
	IClientSocket				*m_pMsgSockClient;				//消息服务器网络接口
	IClientSocket				*m_pTeachSockClient;			//教学服务器网络接口
	CMsgSockClientSink			m_MsgSocketSink;				//消息服务器客户端回调对象
	CTeachSockClientSink		m_TeachSocketSink;				//教学服务器客户端回调对象
	IKernelDataSink				*m_pIKernelDataSink;				//消息回调接口
private:
	#if HAVE_BBEVETN
	CBoardEventData				m_BoardEventData;				//黑板事件数据
	#endif
	CMediaDataManage			m_MediaDataManage;				//媒体数据管理

private:
	static CSocketSinkManage	*m_pSelfObject;					//自己对象

private:
	DWORD						m_dwLastTime;					//最手操作时间

#ifdef ANDROID_OS
private:
	CAudioManager				m_AudioManager;					//音频管理
#endif

private:
	CPlayCWThread				m_PlayCWThread;					//播放课件对象

public:
	CSocketSinkManage(void);
	~CSocketSinkManage(void);

public:
	//创建实例
	static CSocketSinkManage *GetInstance();
	//释放实例
	static void ReleaseInstance();

public:
	//开始网络服务
	virtual bool StartNetService();
	//停止网络服务
	virtual bool StopNetService();
	//初始化SockSink
	virtual bool InitSocketSinkManage(LPCTSTR lpszLBSvrIp,WORD wLBSvrPort);
	//开始网络连接
	virtual bool ConnectToServer(CONNECTSVRTYPE cst=CST_LOGINSVR);
	//发送数据到AVSERVER
	virtual bool SendDataToAVServer(WORD wSubCmd,void * pData,WORD wDataSize,DWORD dwSequenceID=INVALID_DWORD);
	//发送数据到消息服务器
	virtual bool SendDataToMsgServer(WORD wSubCmd,void * pData,WORD wDataSize,DWORD dwSequenceID=INVALID_DWORD);
	//关闭网络连接
	virtual void CloseSocketConnect();
	//发送文本消息
	virtual bool SendTextMsgToMsgSvr(DWORD dwDstID,WORD wDstType,WORD wRelationType,WORD wMsgType,
		DWORD dwFileSize,DWORD dwDuration,DWORD dwSequenceID,const void * pMsgData,WORD wDataSize);
	//设置核心回调接口
    virtual bool SetKernelDataSink(void *pIKernelDataSink);
	//设置黑板数据回调接口
    virtual bool SetBoardDataSink(void *pIBoardEventSink);
    //释放网络对象
    virtual void FreeSockSinkManage();
	//计算黑板比例
	virtual float ComputeBoardScale(int scx,int scy,int dcx,int dcy,POINT &offset);
	//设置用户信息
	virtual bool SetUserInfoData(DWORD dwUserID,WORD wUserType,LPCTSTR lpszUserName,LPCTSTR lpszRealName,LPCTSTR lpszCheckCode);
	//设置教室信息
	virtual bool SetDstRoomData(DWORD dwDstRoomID,DWORD dwTeacherID);
	//设置媒体回调接口
	virtual bool SetMediaDataSink(void *pIMediaDataSink);
	#ifdef ANDROID_OS
	//开始录音
	virtual bool StartRecord();
	//停止录音
	virtual bool StopRecord();
	#endif
	//发送举手协议
	virtual bool SendUpHandData();
	//播放课件
	virtual bool PlayCwdFile(LPCTSTR lpszFileName);
	//暂停播放课件
	virtual bool PausePlayCwd(bool bPuased);
	//停止播放课件
	virtual bool StopPlayCwd();
	//检测网络连接
	virtual void CheckNetConnect();
	//退出登录
	virtual void LoginOut();

public:
	#ifdef ANDROID_OS
	//设置视频编码大小(nType=0 为RGB数据,nType=1为YUV数据)
	bool SetVideoEncodeSize(int cx,int cy,int nType);
	//开始视频编码
	bool StartVideoEncode();
	//停止视频编码
	bool StopVideoEncode();
	//添加视步视频数据
	int AddVideoData(const void *pVideoData,int nDataSize);
	#endif

public:
	//H264SpsPps数据
	virtual bool OnH264SpsPpsData(const void * pHeadData,DWORD dwDataSize);

#ifdef ANDROID_OS
public:
	//录音数据回调
	virtual bool __cdecl OnAacAudioData(const void * pAacData,DWORD dwDataSize);
#endif

private:
	//发送黑板事件
	bool SendBoardEventData(BOARDEVENTID beid,void *pEventData,DWORD dwDataSize);
	//发送操作事件
	bool SendOperateEventData(OPEREVENTID oeid,void *pEventData,DWORD dwDataSize,DWORD dwSequenceID=INVALID_DWORD);
};

#endif //__BOWEN_HU_SOCKET_SINK_MANAGE_HEAD_FILE__
