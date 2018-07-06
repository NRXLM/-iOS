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
	virtual bool SendData(WORD mainCmd,WORD subCmd,DWORD dwSequenceID,void * pData,WORD wDataSize) = 0;
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
	virtual bool OnSocketRead(CMD_Command Command, void * pBuffer, WORD wDataSize, IClientSocket * pIClientSocket)=0;
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
	virtual bool ConnectToServer(CONNECTSVRTYPE cst=CST_LOGINSVR)=0;
	//发送数据到AVSERVER
	virtual bool SendDataToAVServer(WORD wSubCmd,void * pData,WORD wDataSize,DWORD dwSequenceID=INVALID_DWORD)=0;
	//发送数据到消息服务器
	virtual bool SendDataToMsgServer(WORD wSubCmd,void * pData,WORD wDataSize,DWORD dwSequenceID=INVALID_DWORD)=0;
	//关闭网络连接
	virtual void CloseSocketConnect()=0;
	//发送文本消息
	virtual bool SendTextMsgToMsgSvr(DWORD dwDstID,WORD wDstType,WORD wRelationType,WORD wMsgType,
		DWORD dwFileSize,DWORD dwDuration,DWORD dwSequenceID,const void * pMsgData,WORD wDataSize)=0;
	//设置核心回调接口
    virtual bool SetKernelDataSink(void *pIKernelDataSink)=0;
	//设置黑板数据回调接口
    virtual bool SetBoardDataSink(void *pIBoardEventSink)=0;
    //释放网络对象
    virtual void FreeSockSinkManage()=0;
	//计算黑板比例
	virtual float ComputeBoardScale(int scx,int scy,int dcx,int dcy,POINT &offset)=0;
	//设置用户信息
	virtual bool SetUserInfoData(DWORD dwUserID,WORD wUserType,LPCTSTR lpszUserName,LPCTSTR lpszRealName,LPCTSTR lpszCheckCode)=0;
	//设置教室信息
	virtual bool SetDstRoomData(DWORD dwDstRoomID,DWORD dwTeacherID)=0;
	//设置媒体回调接口
	virtual bool SetMediaDataSink(void *pIMediaDataSink)=0;
	#if !MACIOS
	//开始录音
	virtual bool StartRecord()=0;
	//停止录音
	virtual bool StopRecord()=0;
	#endif
	//发送举手协议
	virtual bool SendUpHandData()=0;
	//播放课件
	virtual bool PlayCwdFile(LPCTSTR lpszFileName)=0;
	//暂停播放课件
	virtual bool PausePlayCwd(bool bPuased)=0;
	//停止播放课件
	virtual bool StopPlayCwd()=0;
	//检测网络连接
	virtual void CheckNetConnect()=0;
	//退出登录
	virtual void LoginOut()=0;
};

//黑板数据回调接口
struct IBoardEventSink
{
	//接收黑板线数据
	virtual bool OnRevBoardLineData(const void *pEventData,DWORD dwDataSize)=0;
	//接收黑板文本数据
	virtual bool OnRevBoardTextData(int x,int y,int cx,int cy,LPCTSTR lpszMsgTxt,int nTxtLength,int nFontSize)=0;
	//接收黑板图型数据
	virtual bool OnRevBoardShapeData(int nShapeType,int x,int y,int cx,int cy)=0;
	//接收黑板线型数据
	virtual bool OnRevBoardLineStyle(int nPageCount,int nLineWidth,DWORD dwLineColor,int nLineStyle)=0;
	//接收黑板缩放图片
	virtual bool OnRevBoardZoomImage(DWORD dwImgID,int x,int y,int cx,int cy,int nPageIndex)=0;
	//添加黑板图片
	virtual bool OnRevBoardAddImage(DWORD dwImgID,int x,int y,int cx,int cy,LPCTSTR lpszUrl,int nPageIndex)=0;
	//删除黑板图片
	virtual bool OnRevBoardDelImage(DWORD dwImgID)=0;
	//接收黑板无参数事件
	virtual bool OnProcNoDataEvent(int nEventID)=0;
	//接收黑板整型参数事件
	virtual bool OnProcIntDataEvent(int nEventID,int nValue)=0;
	//接收黑板DWORD型参数事件
	virtual bool OnProcDwordDataEvent(int nEventID,DWORD dwValue)=0;
	//接由黑板大小事件
	virtual bool OnProcBoardSize(int cx,int cy,int nPageCount)=0;
	//接收客户端播放事件
	virtual bool OnRecvClientPlayEvent(int nResType,LPCTSTR lpszResName,LPCTSTR lpszResUrl)=0;
	//教师选择学生回答事件
	virtual bool OnTeacherSelecStudent(DWORD dwUserID,LPCTSTR lpszRealName)=0;
	//教师选择学生回答事件
	virtual bool OnBeginClassEvent(DWORD dwDuratoin,MYBOOL bCanSpeak,MYBOOL bCanWrite,MYBOOL bUpHanding,MYBOOL bClassPause,int nBBPageCount,int nCurrentPage,int nCurrPeriod,int nscType)=0;
};

//音视频数据接口
struct IMediaDataSink
{
	//音频数据
	virtual bool OnAudioRTData(const void *pAudioData,DWORD dwDataSize,DWORD dwSequenceID)=0;
	//视频H264SpsPps数据
	virtual bool OnH264SpsPpsData(WORD wChannelID,const void * pSpsPps,DWORD dwDataSize)=0;
	//H264视频编码数据
	virtual bool OnVideoH264CodeData(WORD wChannelID,const void *pH264Data,DWORD dwDataSize)=0;
};

#ifndef __IPHONE_4_0
//底层数据回调接口
struct IKernelDataSink
{
	//消息通知
	virtual bool OnRecvMsgData(LPCTSTR lpszSrcName,LPCTSTR lpszMsg,WORD wMsgType,DWORD dwTime)=0;
};

#else
//底层数据回调接口
struct IKernelDataSink
{
	//消息通知
	virtual bool OnRecvMsgData(DWORD dwSrcID,DWORD dwDstID,WORD wSrcType,WORD wDstType,WORD wRelationType,WORD wMsgType,LPCSTR lpszMsgBody,DWORD dwFileSize,DWORD dwDuration,DWORD dwMsgID,DWORD dwTimpStap)=0;
    //接收消息Response
    virtual bool OnRecvMsgResp(int nResult,DWORD dwMsgID,DWORD dwSendTime,DWORD dwSequenceID)=0;
	//错误提示消息
	virtual bool OnErrMsgNotify(int nErrCode,LPCSTR lpszErrMsg)=0;
};

///////////////////////////////////////////////////////////////////
ISockSinkManage *AfxCreateSinkManage();
#endif

#endif
