///////////////////////////////////////////////////////////////////////////////////
//			教学网络回调类实现文件 TeachSockClientSink.cpp
//			2014-3-12 hubo
///////////////////////////////////////////////////////////////////////////////////
#include "TeachSockClientSink.h"
#include "localdef.h"

///////////////////////////////////////////////////////////////////////////////////
CTeachSockClientSink::CTeachSockClientSink(void)
{
	m_bLogined=FALSE;
	m_pINetDataWndSink=NULL;
	m_pIMediaDataManage=NULL;
}

CTeachSockClientSink::~CTeachSockClientSink(void)
{
}

//初始化教学网络客户端
bool CTeachSockClientSink::InitTeachSockClient(IBlackboardManage *pINetDataWndSink,IMediaDataManage *pIMediaDataManage)
{
	m_pINetDataWndSink=pINetDataWndSink;
	m_pIMediaDataManage=pIMediaDataManage;
	return true;
}

//网络连接消息
bool CTeachSockClientSink::OnSocketConnect(int iErrorCode, LPCTSTR pszErrorDesc, IClientSocket * pIClientSocket)
{
	if(iErrorCode!=0)	//连接失败
	{
		NetMsgAutoParam(OT_CONNECT_FAILD,TEXT("连接教学服务器失败,%s\n"),pszErrorDesc);
		return false;
	}

	return LoginToTeachServer();
}

//网络读取消息
bool CTeachSockClientSink::OnSocketRead(CMD_Command Command, void * pBuffer,WORD wDataSize, IClientSocket * pIClientSocket)
{
	switch(Command.wSubCmd)
	{
	case TSVR_TEACHER_AUDIO:
	case TSVR_STUDENT_AUDIO:
		return OnProcStudentAudioData(Command.dwSequenceID,pBuffer,wDataSize);
	case TSVR_TEACHER_VIDEO:
		return OnProcUserVideoData(Command.dwSequenceID,pBuffer,wDataSize);
	case TSVR_T_BB_EVENT:
		return OnProcStudentBoardData(Command.wSubCmd,Command.dwSequenceID,pBuffer,wDataSize);
	case TSVR_T_OPT_EVENT:
		return OnProcOperateEventData(Command.wSubCmd,Command.dwSequenceID,pBuffer,wDataSize);
	case ACITVE_TEST:
		return OnProcActiveTest(pIClientSocket);
	case TSVR_H264_SPS:
		return OnProcH264SpsPpsData(pBuffer,wDataSize);
	case TSVR_H264_SPS_RESP:
		return true;
	case TSVR_STUDENT_LOGIN_RESP:
		return OnProcLoginSvrResp(Command.dwSequenceID,pBuffer,wDataSize);
	case TSVR_TEACH_EXIT_ROOM:
		return OnProcTeacherExitRoom(Command.dwSequenceID,pBuffer,wDataSize);
	#if TEST_RECV_DAT
	case TSVR_TEACHER_VIDEO:
		return OnProcTeacherVideoData(Command.dwSequenceID,pBuffer,dwDataSize);
	#endif
	}

	return true;
}

#if TEST_RECV_DAT
//处理教师端视频数
bool CTeachSockClientSink::OnProcTeacherVideoData(DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize)
{
	TRACE(TEXT("收到视频数据，大小：%ld\n"),dwDataSize);
	return true;
}
#endif

//网络关闭消息
bool CTeachSockClientSink::OnSocketClose(IClientSocket * pIClientSocket, bool bCloseByServer)
{
	if(!bCloseByServer) return false;

	OnProcTeacherExitRoom(0,NULL,0);
	return true;
}

//连接到教学服务器
bool CTeachSockClientSink::ConnectToTeachServer()
{
	ASSERT(m_pIClientSocket!=NULL);
	if(m_pIClientSocket==NULL) return false;
    
    if (strlen(g_ConfigParam.szTeachSvrUrl) <= 0) {
        NetMsgNotify(TEXT("Teacher Url is NULL"));
        return false;
    }
	NetMsgNotify(TEXT("Start connect to teaching server...\n"));
	return m_pIClientSocket->ConnectToServer(g_ConfigParam.szTeachSvrUrl,g_ConfigParam.wTeachSvrPort);
}

//关闭连接
bool CTeachSockClientSink::CloseTeachConnect()
{
	if(m_pIClientSocket!=NULL)
		m_pIClientSocket->CloseSocket(false);

	return true;
}

//登录到教学服务器
bool CTeachSockClientSink::LoginToTeachServer()
{
	TCHAR szMsgTxt[128]="";
	sprintf(szMsgTxt,TEXT("------Start login to Teaching server ClienType:%d\n"),g_ConfigParam.wClientType);
	NetMsgNotify(szMsgTxt);

	m_bLogined=FALSE;

	BYTE cbBuffer[256];
	BYTE *pData=cbBuffer;

	WORD wSendSize=0;
	WORD wLength=(WORD)strlen(g_ConfigParam.szUserName);

	pData=WriteDword(pData,g_ConfigParam.dwDstRoomID,wSendSize);
	pData=WriteDword(pData,g_ConfigParam.dwUserID,wSendSize);
	pData=WriteWord(pData,g_ConfigParam.wClientType,wSendSize);
	pData=WriteWord(pData,wLength,wSendSize);
	pData=WriteFromBuffer(pData,g_ConfigParam.szUserName,wLength,wSendSize);
	wLength=(WORD)strlen(g_ConfigParam.szCheckCode);
	pData=WriteWord(pData,wLength,wSendSize);
	pData=WriteFromBuffer(pData,g_ConfigParam.szCheckCode,wLength,wSendSize);

	return m_pIClientSocket->SendData(TEACH_MAIN_CMD,TSVR_STUDENT_LOGIN,0,cbBuffer,wSendSize);
}

//处理登录Response
bool CTeachSockClientSink::OnProcLoginSvrResp(DWORD dwSequenceID,void *pDataBuffer,WORD wDataSize)
{
	ASSERT(wDataSize>=8);
	if(wDataSize<8) return false;

	int nResult=0;
	WORD wLength=0;

	BYTE *pData=(BYTE *)pDataBuffer;
	pData=ReadInt32(pData,nResult);

	TCHAR szMsgTxt[128]="";
	sprintf(szMsgTxt,TEXT("------Teaching server OnProcLoginSvrResp,wDataSize:%d,Result:%d"),wDataSize,nResult);
	NetMsgNotify(szMsgTxt);
	if(nResult!=0)
	{
		m_pIMediaDataManage->OnErrMsgNotify(nResult,TEXT("登录教学服务器失败"));
		return false;
	}
	pData=ReadDword(pData,g_ConfigParam.dwRoomIndex);
	
	m_bLogined=TRUE;
	//连接检测定时器
	m_dwRecvTime=::GetTickCount();
	wDataSize-=8;
	return m_pINetDataWndSink->OnBlackboardEventData(BEID_BOARD_SIZE,pData,wDataSize,0);
	 
	NetMsgNotify(TEXT("Login to teaching server success\n"),OT_LOGIN_TEACHSVR_SUCCESS);
	
	return true;
}

//处理学生音频数据
bool CTeachSockClientSink::OnProcStudentAudioData(DWORD dwSequenceID,void *pDataBuffer,WORD wDataSize)
{ 
	if(m_pIMediaDataManage!=NULL)
	{
		m_pIMediaDataManage->OnAudioRTData(pDataBuffer,wDataSize,0);
	}
	return true;
}

//处理学生轨迹数据
bool CTeachSockClientSink::OnProcStudentBoardData(WORD wSubCmd,DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize)
{
	//绘制到界面
	if(m_pINetDataWndSink==NULL) return false;
	BOARDEVENTID beid;
	WORD wBoardCmd=0;
	BYTE *pData=(BYTE *)pDataBuffer;
	pData=ReadWord(pData,wBoardCmd);
	beid=(BOARDEVENTID)wBoardCmd;
	dwDataSize-=2;
	return m_pINetDataWndSink->OnBlackboardEventData(beid,pData,dwDataSize,0);
}

//处理操作事件数据
bool CTeachSockClientSink::OnProcOperateEventData(WORD wSubCmd,DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize)
{
	//绘制到界面
	ASSERT(m_pINetDataWndSink != NULL);
	if(m_pINetDataWndSink==NULL) return false;
	OPEREVENTID oeid;
	WORD wBoardCmd=0;
	BYTE *pData=(BYTE *)pDataBuffer;
	pData=ReadWord(pData,wBoardCmd);
	oeid=(OPEREVENTID)wBoardCmd;
	dwDataSize-=2;
	return m_pINetDataWndSink->OnRevOperateEventData(oeid,pData,dwDataSize,0);
}


//处理视频H264SpsPps数据
bool CTeachSockClientSink::OnProcH264SpsPpsData(void *pDataBuffer,DWORD dwDataSize)
{
	if(pDataBuffer==NULL || dwDataSize<5) return true;
	if(m_pIMediaDataManage==NULL) return true;

	const int MAXDATA=1024;
	BYTE cbBuffer[MAXDATA];
	DWORD dwChannelID=0;
	WORD wDataLen=0;
	BYTE *pData=(BYTE *)pDataBuffer;
	DWORD dwCount=pData[0];
	pData++;
	for(DWORD i=0;i<dwCount;i++)
	{
		pData=ReadDword(pData,dwChannelID);
		pData=ReadWord(pData,wDataLen);
		if(wDataLen>=MAXDATA) return true;
		pData=ReadToBuffer(pData,cbBuffer,wDataLen);
		m_pIMediaDataManage->OnH264SpsPpsData(dwChannelID,cbBuffer,wDataLen);
	}

	return true;
}

//处理用户视频数据
bool CTeachSockClientSink::OnProcUserVideoData(DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize)
{
	if(m_pIMediaDataManage==NULL) return true;

	DWORD dwChannelID=0;
	BYTE *pDstData=(BYTE *)pDataBuffer;
	memcpy(&dwChannelID,pDstData,sizeof(DWORD));
	dwChannelID=ntohl(dwChannelID);
	pDstData+=sizeof(DWORD);
	dwDataSize-=sizeof(DWORD);

	m_pIMediaDataManage->OnVideoH264CodeData(dwChannelID,pDstData,dwDataSize);

	return true;
}

//处理教师退出教室
bool CTeachSockClientSink::OnProcTeacherExitRoom(DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize)
{
	m_pIMediaDataManage->OnErrMsgNotify(-11,TEXT("教师退出教室，到服务端连接断开"));
	return false;	//返回 false 断开连接
}
