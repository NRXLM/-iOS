/////////////////////////////////////////////////////////////////////////////////////
////			教学网络回调类实现文件 TeachSockClientSink.cpp
////			2014-3-12 hubo
/////////////////////////////////////////////////////////////////////////////////////
//#include "TeachSockClientSink.h"
//#include "localdef.h"
//
/////////////////////////////////////////////////////////////////////////////////////
//CTeachSockClientSink::CTeachSockClientSink(void)
//{
//	m_bLogined=FALSE;
//	m_pIMediaDataManage=NULL;
//}
//
//CTeachSockClientSink::~CTeachSockClientSink(void)
//{
//}
//
////初始化教学网络客户端
//bool CTeachSockClientSink::InitTeachSockClient(IMediaDataManage *pIMediaDataManage)
//{
//	m_pIMediaDataManage=pIMediaDataManage;
//	return true;
//}
//
////网络连接消息
//bool CTeachSockClientSink::OnSocketConnect(int iErrorCode, LPCTSTR pszErrorDesc, IClientSocket * pIClientSocket)
//{
//	if(iErrorCode!=0)	//连接失败
//	{
//		NetMsgAutoParam(OT_CONNECT_FAILD,TEXT("连接教学服务器失败,%s\n"),pszErrorDesc);
//		return false;
//	}
//
//	return LoginToTeachServer();
//}
//
//
//////处理请求服务器地址Response
////bool CTeachSockClientSink::OnProcRequestSvrIPPortResp(DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize)
////{
////	ASSERT(dwDataSize==6);
////	if(dwDataSize!=6) return false;
////
////	struct in_addr addrin;
////	DWORD dwSvrType=0;
////	DWORD dwSvrIp=0;
////	BYTE *pData=(BYTE *)pDataBuffer;
////	pData=ReadDword(pData,dwSvrIp);
////	pData=ReadWord(pData,m_wVssPort);
////	addrin.s_addr=dwSvrIp;
////
////	CHAR *pTmpIP=inet_ntoa(addrin);
////	LOGD(TEXT("Get vss ip and port:[%s:%d]"),pTmpIP,m_wVssPort);
////
////	if(dwSvrIp==0)
////	{
////		LOGE(TEXT("Get vss ip faild，please agin..."));
////		OnNotifyScreenCap(JNI_ERR_1009);
////		return false;
////	}
////	if(m_wVssPort==0)
////	{
////		LOGE(TEXT("Get vss port faild，please agin..."));
////		OnNotifyScreenCap(JNI_ERR_1009);
////		return false;
////	}
////	lstrcpy(m_szVssIp,pTmpIP);
////
////	LOGD(TEXT("Get vss ip and port success..."));
////	OnNotifyScreenCap(JNI_SC_2004);
////	SETTIMER(IDT_CONN_SERVER,25,1,ST_SWTSVR,(ITimerSink *)this);		//连接到媒体流服务器定时器
////	return false;
////}
//
////处理登录Response
//bool CTeachSockClientSink::OnProcLoginToServerResp(DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize)
//{
//	int nResult=0;
//	BOOL bHaveVoice=0;
//	DWORD dwRoomIndex=0;
//
//	BYTE *pData=(BYTE *)pDataBuffer;
//	pData=ReadInt32(pData,nResult);
//	pData=ReadInt32(pData,bHaveVoice);
//	if(nResult!=0)
//	{
//		LOGE(TEXT("Login to vss faild,err code：%d"),nResult);
//		TCHAR szMsg[MAX_PATH]=TEXT("");
//		if(nResult==SERVICE_NOT_EXIST)
//			lstrcpy(szMsg,TEXT("service code was not exists"));
//		else
//			lstrcpy(szMsg,TEXT("servicer is busy,please wait a moment..."));
//		LOGE(szMsg);
//		OnNotifyScreenCap(JNI_ERR_1009);
//		return false;
//	}
//
//	m_bLogined=TRUE;
//
//	if(m_dwSpsSize>0)	//发送SpsPps数据
//	{
//		CMD_Command cmd={0};
//		cmd.wMainCmd=ALL_MAIN_CMD;
//		cmd.wSubCmd=TSVR_S_H264_SPS;
//		m_pIClientSocket->SendData(cmd,m_pSpsData,(WORD)m_dwSpsSize);
//	}
//
//	//启动录屏
//	bool bSuccess=m_pINetNotifySink->StartAVCapThread();
//	if(bSuccess)
//	{
//		if(bHaveVoice)
//		{
//			g_GlobalData.bHaveVoice=bHaveVoice;
//			//开始启动音频组件
//			m_pINetNotifySink->StartAudioService();
//		}
//		LOGD(TEXT("请求接入到客服成功"));
//		OnNotifyScreenCap(JNI_SC_2005);
//	}
//	else
//	{
//
//		LOGE(TEXT("请求接入到客服失败"));
//		OnNotifyScreenCap(JNI_SC_1013);
//	}
//
//	return true;
//}
//
//
//
////网络读取消息
//bool CTeachSockClientSink::OnSocketRead(CMD_Command Command, void * pBuffer,WORD wDataSize, IClientSocket * pIClientSocket)
//{
//	switch(Command.wSubCmd)
//	{
//	case TSVR_SERVICE_AUDIO:
//		return OnProcStudentAudioData(Command.dwSequenceID,pBuffer,wDataSize);
//   case LBS_CLIENT_RES_SVRIP_RESP:
//		return OnProcRequestSvrIPPortResp(Command.dwSequenceID,pBuffer,dwDataSize);
//	case TSVR_USER_LOGIN_RESP:
//		return <#expression#>
//	case TSVR_TEACHER_VIDEO:
//		return OnProcUserVideoData(Command.dwSequenceID,pBuffer,wDataSize);
//	case ACITVE_TEST:
//		return OnProcActiveTest(pIClientSocket);
//	case TSVR_H264_SPS:
//		return OnProcH264SpsPpsData(pBuffer,wDataSize);
//	case TSVR_H264_SPS_RESP:
//		return true;
//	case TSVR_TEACHER_LOGIN_RESP:
//		return OnProcLoginSvrResp(Command.dwSequenceID,pBuffer,wDataSize);
//	case TSVR_TEACH_EXIT_ROOM:
//		return OnProcTeacherExitRoom(Command.dwSequenceID,pBuffer,wDataSize);
//    case TSVR_STUDENT_LOGIN_RESP:
//        return OnScreenLoginResponse(Command.dwSequenceID, pBuffer, wDataSize);
//	}
//
//	return true;
//}
//
////网络关闭消息
//bool CTeachSockClientSink::OnSocketClose(IClientSocket * pIClientSocket, bool bCloseByServer)
//{
//	m_bLogined=FALSE;
//
//	if(!bCloseByServer) return false;
//
//	OnProcTeacherExitRoom(0,NULL,0);
//	return true;
//}
//
////连接到教学服务器
//bool CTeachSockClientSink::ConnectToTeachServer()
//{
//	ASSERT(m_pIClientSocket!=NULL);
//	if(m_pIClientSocket==NULL) return false;
//
//	NetMsgNotify(TEXT("Start connect to teaching server...\n"));
//	return m_pIClientSocket->ConnectToServer(g_ConfigParam.szTeachSvrUrl,g_ConfigParam.wTeachSvrPort);
//}
//
////关闭连接
//bool CTeachSockClientSink::CloseTeachConnect()
//{
//	if(m_pIClientSocket!=NULL)
//		m_pIClientSocket->CloseSocket(false);
//
//	return true;
//}
//
////发送数据
//bool CTeachSockClientSink::SendData(WORD mainCmd,WORD subCmd,DWORD dwSequenceID,void * pData,WORD wDataSize)
//{
//	if(!m_bLogined) return false;
//
//	return m_pIClientSocket->SendData(mainCmd,subCmd,dwSequenceID,pData,wDataSize);
//}
//
////设置底层回调接口
//bool CTeachSockClientSink::SetKernelDataSink(void * pIKernelDataSink)
//{
//	m_pIKernelDataSink=(IKernelDataSink *)pIKernelDataSink;
//	return (m_pIKernelDataSink!=NULL);
//}
//
////登录到教学服务器
//bool CTeachSockClientSink::LoginToTeachServer()
//{
//	TCHAR szMsgTxt[128]="";
//	sprintf(szMsgTxt,TEXT("------Start login to Teaching server ClienType:%d\n"),g_ConfigParam.wClientType);
//	NetMsgNotify(szMsgTxt);
//
//	m_bLogined=FALSE;
//
//
//    BYTE cbBuffer[12];
//    BYTE *pData=cbBuffer;
//
//    WORD wSendSize=0;
//    pData=WriteDword(pData,23456,wSendSize);
//    pData=WriteDword(pData,20,wSendSize);
//
//    DWORD dwCmdID=BuildCommandID(ALL_MAIN_CMD,TSVR_STUDENT_LOGIN);
//
////    BYTE cbBuffer[256];
////    BYTE *pData=cbBuffer;
////
////    WORD wSendSize=0;
////    WORD wLength=(WORD)strlen(g_ConfigParam.szUserName);
////
////    pData=WriteDword(pData,g_ConfigParam.dwDstRoomID,wSendSize);
////    pData=WriteDword(pData,g_ConfigParam.dwUserID,wSendSize);
////    pData=WriteWord(pData,g_ConfigParam.wClientType,wSendSize);
////    pData=WriteWord(pData,wLength,wSendSize);
////    pData=WriteFromBuffer(pData,g_ConfigParam.szUserName,wLength,wSendSize);
////    wLength=(WORD)strlen(g_ConfigParam.szCheckCode);
////    pData=WriteWord(pData,wLength,wSendSize);
////    pData=WriteFromBuffer(pData,g_ConfigParam.szCheckCode,wLength,wSendSize);
//
//	return m_pIClientSocket->SendData(ALL_MAIN_CMD,TSVR_STUDENT_LOGIN,0,cbBuffer,wSendSize);
//}
//
////视频登录处理Response
//bool CTeachSockClientSink::OnScreenLoginResponse(DWORD dwSequenceID, void *pDataBuffer, WORD wDataSize)
//{
//
//    __int32 nResult=0;
//    DWORD dwRoomIndex=0;
//
//    BYTE *pData=(BYTE *)pDataBuffer;
//    pData=ReadInt32(pData,nResult);
//
//
//    if(nResult!=0)
//    {
//        printf("------Screen server OnScreenLoginResponse,wDataSize:%d,Result:%d",wDataSize,nResult);
//        return false;
//    }
//
//    m_bLogined=TRUE;
//
////    if(m_dwSpsSize>0)    //发送SpsPps数据
////    {
////        DWORD dwCmdID=BuildCommandID(ALL_MAIN_CMD,TSVR_S_H264_SPS);
////        m_pIClientSocket->SendData(dwCmdID,0,m_pSpsData,(WORD)m_dwSpsSize);
////    }
//
//
//    //连接检测定时器
//    m_dwRecvTime=::GetTickCount();
//
//    if(m_pIKernelDataSink!=NULL)
//        m_pIKernelDataSink->OnLoginRespNotify(nResult);
//
//    printf("录屏服务登录成功！\n");
////    NetMsgNotify(TEXT("Login to teaching server success\n"),OT_LOGIN_TEACHSVR_SUCCESS);
//
//    return true;
//}
//
////处理登录Response
//bool CTeachSockClientSink::OnProcLoginSvrResp(DWORD dwSequenceID,void *pDataBuffer,WORD wDataSize)
//{
//	ASSERT(wDataSize>=8);
//	if(wDataSize<8) return false;
//
//	int nResult=0;
//	WORD wLength=0;
//
//	BYTE *pData=(BYTE *)pDataBuffer;
//	pData=ReadInt32(pData,nResult);
//
//	TCHAR szMsgTxt[128]="";
//	sprintf(szMsgTxt,TEXT("------Teaching server OnProcLoginSvrResp,wDataSize:%d,Result:%d"),wDataSize,nResult);
//	NetMsgNotify(szMsgTxt);
//	if(nResult!=0)
//	{
//		m_pIMediaDataManage->OnErrMsgNotify(nResult,TEXT("登录教学服务器失败"));
//		return false;
//	}
//	pData=ReadDword(pData,g_ConfigParam.dwRoomIndex);
//
//	m_bLogined=TRUE;
//	//连接检测定时器
//	m_dwRecvTime=::GetTickCount();
//
//	if(m_pIKernelDataSink!=NULL)
//		m_pIKernelDataSink->OnLoginRespNotify(nResult);
//
//	NetMsgNotify(TEXT("Login to teaching server success\n"),OT_LOGIN_TEACHSVR_SUCCESS);
//
//	return true;
//}
//
////处理学生音频数据
//bool CTeachSockClientSink::OnProcStudentAudioData(DWORD dwSequenceID,void *pDataBuffer,WORD wDataSize)
//{
//	if(m_pIMediaDataManage!=NULL)
//	{
//		m_pIMediaDataManage->OnAudioRTData(pDataBuffer,wDataSize,0);
//	}
//	return true;
//}
//
////处理视频H264SpsPps数据
//bool CTeachSockClientSink::OnProcH264SpsPpsData(void *pDataBuffer,DWORD dwDataSize)
//{
//	if(pDataBuffer==NULL || dwDataSize<5) return true;
//	if(m_pIMediaDataManage==NULL) return true;
//
//	const int MAXDATA=1024;
//	BYTE cbBuffer[MAXDATA];
//	DWORD dwChannelID=0;
//	WORD wDataLen=0;
//	BYTE *pData=(BYTE *)pDataBuffer;
//	DWORD dwCount=pData[0];
//	pData++;
//	for(DWORD i=0;i<dwCount;i++)
//	{
//		pData=ReadDword(pData,dwChannelID);
//		pData=ReadWord(pData,wDataLen);
//		if(wDataLen>=MAXDATA) return true;
//		pData=ReadToBuffer(pData,cbBuffer,wDataLen);
//		m_pIMediaDataManage->OnH264SpsPpsData(dwChannelID,cbBuffer,wDataLen);
//	}
//
//	return true;
//}
//
////处理用户视频数据
//bool CTeachSockClientSink::OnProcUserVideoData(DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize)
//{
//	if(m_pIMediaDataManage==NULL) return true;
//
//	DWORD dwChannelID=0;
//	BYTE *pDstData=(BYTE *)pDataBuffer;
//	memcpy(&dwChannelID,pDstData,sizeof(DWORD));
//	dwChannelID=ntohl(dwChannelID);
//	pDstData+=sizeof(DWORD);
//	dwDataSize-=sizeof(DWORD);
//
//	m_pIMediaDataManage->OnVideoH264CodeData(dwChannelID,pDstData,dwDataSize);
//
//	return true;
//}
//
////处理教师退出教室
//bool CTeachSockClientSink::OnProcTeacherExitRoom(DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize)
//{
//	m_pIMediaDataManage->OnErrMsgNotify(-11,TEXT("教师退出教室，到服务端连接断开"));
//	return false;	//返回 false 断开连接
//}

