
#include "NetDataSyncSink.h"
#include "TimerEngine.h"
#include "localdef.h"
#include <CoreGraphics/CoreGraphics.h>
#if !MACIOS
#define LOG_TAG "OIT/JNI/NetDataSync"
#include "log.h"
#endif




///////////////////////////////////////////////////////////////////////////////////////////
CNetDataSyncSink::CNetDataSyncSink(void)
{
	m_bLogined=false;
	m_pIClientSocket=NULL;
	
	m_wVssPort=0;
	m_szVssIp[0]=0;
	m_dwChannelID=123456;
	m_dwSpsSize=0;
	m_pSpsData=NULL;
	m_pINetNotifySink=NULL;
	m_pIClientSocket=NULL;
	m_pINetDataSink=NULL;
	
	strcpy(m_szMediaPath,"/sdcard/download/");
}

CNetDataSyncSink::~CNetDataSyncSink(void)
{
}

//≥ı ºªØœﬂ≥Ã
bool CNetDataSyncSink::InitDataSyncThread(INetNotifySink *pINetNotifySink,const char *pMediaPath)
{
	CGContextRef UIGraphicsGetCurrentContext();
	m_pINetNotifySink=pINetNotifySink;
	if(pMediaPath!=NULL)
	{
		strcpy(m_szMediaPath,pMediaPath);
		int nLength=(int)strlen(m_szMediaPath);
		if(nLength<3) return false;
		if(m_szMediaPath[nLength-1]!='/')
			strcat(m_szMediaPath,"/");
	}
	
	IClientSocketSink *pIClientSocketSink=(IClientSocketSink *)this;
	m_ClientSocket.SetSocketSink(pIClientSocketSink);
	
	return (m_pINetNotifySink!=NULL);
}

//设置音频接口
bool CNetDataSyncSink::SetNetDataSink(INetDataSink *pINetDataSink)
{
	m_pINetDataSink=pINetDataSink;
	return (m_pINetDataSink!=NULL);
}

//Socket connection message
bool CNetDataSyncSink::OnSocketConnect(int iErrorCode,LPCTSTR pszErrorDesc,IClientSocket *pIClientSocket)
{
	if(iErrorCode!=0)
	{
		switch(m_cst)
		{
			case CST_LBSVR:
			{
				SwitchLoadServerIpPort();	//切换均衡服务器
				SETTIMER(IDT_CONN_SERVER,5000,1,CST_LBSVR,(ITimerSink *)this);
				LOGE(TEXT("Connect to lbs faild,%s,5s try!\n"),pszErrorDesc);
				OnNotifyScreenCap(JNI_SC_1011);
			}
				break;
			case CST_SWTSVR:
			{
				int nConnType=CST_SWTSVR;
				if(++m_dwAvsConnNum>=3)
				{
					nConnType=CST_LBSVR;
					m_dwAvsConnNum=0;
				}
//				启动定时器进行重连
				SETTIMER(IDT_CONN_SERVER,5000,1,CST_SWTSVR,(ITimerSink *)this);
//				打印错误信息
				LOGE(TEXT("Connect to vss faild,%s,5s try\n"),pszErrorDesc);
				OnNotifyScreenCap(JNI_SC_1012);
			}
				break;
			case CST_MSGSVR:
			case CST_VERSVR:
				break;
		}
		
		return false;
	}
	
	bool bSuccess=true;
	switch(m_cst)
	{
		case CST_LBSVR:
			bSuccess=RequestSvrIPPort();
			break;
		case CST_SWTSVR:
			bSuccess=LoginToAvsSvr();
			break;
		case CST_MSGSVR:
		case CST_VERSVR:
			break;
	}
	
	return bSuccess;
}

//Socket read message
bool CNetDataSyncSink::OnSocketRead(CMD_Command Command,void * pBuffer,DWORD dwDataSize,IClientSocket *pIClientSocket)
{
	switch(Command.wSubCmd)
	{
		case TSVR_SERVICE_AUDIO:
			return OnProcSerivceAudio(Command.dwSequenceID,pBuffer,dwDataSize);
		case TSVR_T_BB_EVENT:
			return OnProcCtrlEventData(Command.dwSequenceID,pBuffer,dwDataSize);
		case TSVR_USER_LOGIN_RESP:
			return OnProcLoginToServerResp(Command.dwSequenceID,pBuffer,dwDataSize);
		case LBS_CLIENT_RES_SVRIP_RESP:
			return OnProcRequestSvrIPPortResp(Command.dwSequenceID,pBuffer,dwDataSize);
		case TSVR_STOP_HELPER:
			return OnProcStopHelperNotify(Command.dwSequenceID,pBuffer,dwDataSize);
		case ACITVE_TEST:
			return OnProcActiveTest(pIClientSocket);
	}
	return true;
}

//Socket close message
bool CNetDataSyncSink::OnSocketClose(IClientSocket *pIClientSocket,bool bCloseByServer)
{
	m_bLogined=FALSE;
	
	if(m_cst==CST_SWTSVR)
	{
		m_pINetNotifySink->StopAVCapThread();
		m_pINetNotifySink->StopAudioSevice();
		
		LOGE(TEXT("Server was closed,5s try..."));
		SETTIMER(IDT_CONN_SERVER,5000,1,CST_SWTSVR,(ITimerSink *)this);
	}
	
	
	
	return true;
}

//∂® ±∆˜ ¬º˛
bool CNetDataSyncSink::OnEventTimer(WORD wTimerID,WPARAM wBindParam)
{
	if(wTimerID==IDT_CONN_SERVER)
	{
		CONNECTSVRTYPE ct=(CONNECTSVRTYPE)wBindParam;
		ConnectToServer(ct);
	}
	
	return true;
}

//¡¨Ω”µΩLB∑˛ŒÒ∆˜
bool CNetDataSyncSink::ConnectToServer(CONNECTSVRTYPE cst)
{
	LOGI("---->>>>>>>---Start ConnectToServer call...");
	ASSERT(m_pIClientSocket!=NULL);
	if(m_pIClientSocket==NULL) return false;
	CHAR szSvrUrl[64]="";
	WORD wSvrPort=0;
	m_cst=cst;
	switch(cst)
	{
		case CST_LBSVR:
			m_pIClientSocket->CloseSocket(false);
			strcpy(szSvrUrl,g_GlobalData.mLbsInfo.szLbsIp);
			wSvrPort=g_GlobalData.mLbsInfo.wLbsPort;
			LOGD("Start connected to lbs,ServerIp:%s:%d",szSvrUrl,wSvrPort);
			OnNotifyScreenCap(JNI_SC_2001);
			break;
		case CST_SWTSVR:
			m_bLogined=FALSE;
			strcpy(szSvrUrl,m_szVssIp);
			wSvrPort=m_wVssPort;
			LOGD("Start connected to vss,ServerIp:%s:%d",szSvrUrl,wSvrPort);
			OnNotifyScreenCap(JNI_SC_2002);
			break;
		default:
			return false;
	}
	
	return m_pIClientSocket->ConnectToServer(szSvrUrl,wSvrPort);
}

//∆Ù∂ØÕ¯¬Áœﬂ≥Ã
bool CNetDataSyncSink::StartNetThread()
{
#if DT_NET_MODE
	if(g_GlobalData.mLbsInfo.wLbsPort==0)
	{
		LOGE("----xxxx---Server port is invalid.");
		return false;
	}
	if(m_ClientSocket.GetConnectState()!=SocketState_NoConnect)
		return true;
	
	m_pIClientSocket=(IClientSocket *)&m_ClientSocket;
	m_bLogined=false;
	if(!m_ClientSocket.IsRuning())
	{
		if(!m_ClientSocket.StartSocketThread())
		{
			LOGE("Network thread start faild");
			return false;
		}
		else
		{
			LOGE("Network thread start success");
		}
	}
	
#endif
	
	return true;
}

//πÿ±’Õ¯¬Á¡¨Ω”
bool CNetDataSyncSink::CloseConnect()
{
	m_ClientSocket.CloseSocket(false);
	return m_ClientSocket.StopSocketThread();
}


//«Î«Û∑˛ŒÒ∆˜µÿ÷∑
bool CNetDataSyncSink::RequestSvrIPPort()
{
	LOGD("Connect to lbs success£¨Request avss server ip and port...");
	OnNotifyScreenCap(JNI_SC_2003);
	BYTE cbBuffer[256];
	BYTE *pData=cbBuffer;
	
    WORD wSendSize=0;
    
    int nJobNumSize=lstrlen(g_GlobalData.szJobNum);
    int nMobileSize=lstrlen(g_GlobalData.szMobile);
    int nExtValLen=lstrlen(g_GlobalData.szExtVal);
	
    pData=WriteByte(pData, (BYTE)nJobNumSize,wSendSize);
    pData=WriteFromBuffer(pData, g_GlobalData.szJobNum, nJobNumSize,wSendSize);
    pData=WriteByte(pData, (BYTE)nMobileSize,wSendSize);
    pData=WriteFromBuffer(pData, g_GlobalData.szMobile, nMobileSize,wSendSize);
	pData=WriteWord(pData, (WORD)g_GlobalData.nBusinessType,wSendSize);
    pData=WriteByte(pData,(BYTE)nExtValLen,wSendSize);
    pData=WriteFromBuffer(pData,g_GlobalData.szExtVal,nExtValLen,wSendSize);
	
#if !MACIOS
	CMD_Command cmd;
	cmd.wMainCmd=ALL_MAIN_CMD;
	cmd.wSubCmd=LBS_CLIENT_RES_SVRIP;
	cmd.dwSequenceID=0;
	return m_pIClientSocket->SendData(cmd,cbBuffer,wSendSize);
#else
	return m_pIClientSocket->SendData(ALL_MAIN_CMD,LBS_CLIENT_RES_SVRIP,0,cbBuffer,wSendSize);
#endif
}

//¥¶¿Ì«Î«Û∑˛ŒÒ∆˜µÿ÷∑Response
bool CNetDataSyncSink::OnProcRequestSvrIPPortResp(DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize)
{
	ASSERT(dwDataSize>=10);
	if(dwDataSize<10) return false;
	
	struct in_addr addrin;
	
    DWORD dwServiceCode=0;
	DWORD dwSvrIp=0;
    WORD wReReqJobCount=0;
	BYTE *pData=(BYTE *)pDataBuffer;
	pData=ReadDword(pData,dwSvrIp);
	pData=ReadWord(pData,m_wVssPort);
    pData=ReadDword(pData,dwServiceCode);
    pData=ReadWord(pData, wReReqJobCount);
    
    g_GlobalData.wReReqJobCount=wReReqJobCount;
	addrin.s_addr=dwSvrIp;
	
	CHAR *pTmpIP=inet_ntoa(addrin);
	LOGD(TEXT("Get vss ip and port:[%s:%d]\n"),pTmpIP,m_wVssPort);
    
    if(dwServiceCode==0)
    {
        if(wReReqJobCount>0 && g_GlobalData.wReqJobFaildCount++<g_GlobalData.wReReqJobCount)
        {
            LOGE(TEXT("Get vss service code faild ,agin request to lvs...\n"));
            SETTIMER(IDT_CONN_SERVER,100,1,CST_LBSVR,(ITimerSink *)this);   //重新连接到LBS服务器定时器
        }
        else
        {
            LOGE(TEXT("Get vss service code faild !please agin...\n"));
            OnNotifyScreenCap(JNI_SC_1014);
        }
        return false;
    }
	if(dwSvrIp==0)
	{
		LOGE(TEXT("Get vss ip faild !please agin...\n"));
		OnNotifyScreenCap(JNI_SC_1009);
		return false;
	}
	if(m_wVssPort==0)
	{
		LOGE(TEXT("Get vss port faild !please agin...\n"));
		OnNotifyScreenCap(JNI_SC_1009);
		return false;
	}
    
    g_GlobalData.dwServiceCode=dwServiceCode;
    
	lstrcpy(m_szVssIp,pTmpIP);
	
	LOGD(TEXT("Get vss ip and port success...\n"));
	OnNotifyScreenCap(JNI_SC_2004);
	SETTIMER(IDT_CONN_SERVER,500,1,CST_SWTSVR,(ITimerSink *)this);		//连接到媒体流服务器定时器
	return false;
}


//µ«¬ºµΩ∑˛ŒÒ∆˜
bool CNetDataSyncSink::LoginToAvsSvr()
{
	LOGD(TEXT("Start login to vss...\n"));
	OnNotifyScreenCap(JNI_SC_2005);
	
	BYTE cbBuffer[128];
	BYTE *pData=cbBuffer;
	
	WORD wSendSize=0;
	pData=WriteDword(pData,g_GlobalData.dwServiceCode,wSendSize);
	
	DWORD bufLen=0;
	printf("Mobile:%s\n",g_GlobalData.szMobile);
	//写入电话号
	bufLen=(BYTE)strlen(g_GlobalData.szMobile);
	pData=WriteByte(pData,bufLen,wSendSize);
	pData=WriteFromBuffer(pData, g_GlobalData.szMobile,bufLen, wSendSize);
	pData=WriteWord(pData, (WORD)g_GlobalData.nBusinessType,wSendSize);
	
#if !MACIOS
	CMD_Command cmd;
	cmd.wMainCmd=ALL_MAIN_CMD;
	cmd.wSubCmd=TSVR_USER_LOGIN;
	cmd.dwSequenceID=0;
	return m_ClientSocket.SendData(cmd,cbBuffer,wSendSize);
#else
	return m_ClientSocket.SendData(ALL_MAIN_CMD,TSVR_USER_LOGIN,0,cbBuffer,wSendSize);
#endif
}

//¥¶¿Ì–ƒÃ¯≤‚ ‘
bool CNetDataSyncSink::OnProcActiveTest(IClientSocket * pIClientSocket)
{
#if !MACIOS
	CMD_Command cmd;
	cmd.wMainCmd=ALL_MAIN_CMD;
	cmd.wSubCmd=ACITVE_TEST_RESP;
	cmd.dwSequenceID=0;
	m_ClientSocket.SendData(cmd);
#else
	m_ClientSocket.SendData(ALL_MAIN_CMD,ACITVE_TEST_RESP,0,NULL,0);
#endif
	return true;
}

//¥¶¿Ìµ«¬ºResponse
bool CNetDataSyncSink::OnProcLoginToServerResp(DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize)
{
	int nResult=0;
	MYBOOL bHaveVoice=0;
	
	BYTE *pData=(BYTE *)pDataBuffer;
	pData=ReadInt32(pData,nResult);
	pData=ReadInt32(pData,bHaveVoice);
	if(nResult!=0)
	{
		LOGE(TEXT("Login to vss faild,err code%d\n"),nResult);
		char szMsg[MAX_PATH]=TEXT("");
        if(nResult==CONFIRM_TIMEOUT)
        {
            lstrcpy(szMsg,TEXT("request helper was timeout\n"));
            if(g_GlobalData.wReReqJobCount>0 && g_GlobalData.wReqJobFaildCount++<g_GlobalData.wReReqJobCount)
            {
                SETTIMER(IDT_CONN_SERVER,100,1,CST_LBSVR,(ITimerSink *)this);   //重新连接到LBS服务器定时器
                return false;
            }
        }
		else if(nResult==SERVICE_NOT_EXIST)
			lstrcpy(szMsg,TEXT("service code was not exists\n"));
		else
			lstrcpy(szMsg,TEXT("servicer is busy,please wait a moment...\n"));
		std::cout<<szMsg<<std::endl;
		OnNotifyScreenCap(JNI_SC_1009);
		return false;
	}
	
	m_bLogined=TRUE;
	OnNotifyScreenCap(JNI_SC_2006);
	if(m_dwSpsSize>0)	//∑¢ÀÕSpsPps ˝æ›
	{
#if !MACIOS
		CMD_Command cmd={0};
		cmd.wMainCmd=ALL_MAIN_CMD;
		cmd.wSubCmd=TSVR_S_H264_SPS;
		m_pIClientSocket->SendData(cmd,m_pSpsData,(WORD)m_dwSpsSize);
#else
		m_ClientSocket.SendData(ALL_MAIN_CMD,TSVR_S_H264_SPS,0,m_pSpsData,m_dwSpsSize);
#endif
	}
	
	//开始启动视频
	bool bSuccess=m_pINetNotifySink->StartAVCapThread();
	if(bSuccess)
	{
		if(bHaveVoice)
		{
			g_GlobalData.bHaveVoice=bHaveVoice;
			//开始启动音频
			m_pINetNotifySink->StartAudioService();
		}
		LOGD(TEXT("Login to av server success\n"));
	}
	else
	{
		
		LOGE(TEXT("Login to av server faild\n"));
		OnNotifyScreenCap(JNI_SC_1013);
	}
	
	return true;
}

//¥¶¿Ì‘∂≥Ãøÿ÷∆ ¬º˛ ˝æ›
bool CNetDataSyncSink::OnProcCtrlEventData(DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize)
{
	return true;
}

//¥¶¿ÌÕ£÷π∏®÷˙∑˛ŒÒÕ®÷™
bool CNetDataSyncSink::OnProcStopHelperNotify(DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize)
{
	if(dwDataSize!=12) return true;
	
	DWORD dwClientCode=0;
	DWORD dwServiceCode=0;
	BYTE *pData=(BYTE *)pDataBuffer;
	pData=ReadDword(pData,dwClientCode);
	pData=ReadDword(pData,dwServiceCode);
	
	LOGE(TEXT("Servicer was %u stop service.\n"),dwServiceCode);
	
	//Õ®÷™JAVA≤„
	OnNotifyScreenCap(JNI_SC_2007);
	return true;
}

//¥¶¿ÌøÕªß“Ù∆µ
bool CNetDataSyncSink::OnProcSerivceAudio(DWORD dwSequenceID,void *pDataBuffer,DWORD dwDataSize)
{
	if(m_pINetDataSink!=NULL)
	{
		m_pINetDataSink->AudioDataNotify((const BYTE *)pDataBuffer,dwDataSize);
	}
	
	return true;
}

bool CNetDataSyncSink::OnAacAudioData(BYTE *pAacData,int nDataSize)
{
	if(!m_bLogined && !g_GlobalData.bHaveVoice) return true;
	if (m_ClientSocket.GetConnectState() == SocketState_NoConnect) {
		return false;
	}
#if !MACIOS
	CMD_Command cmd;
	cmd.wMainCmd=ALL_MAIN_CMD;
	cmd.wSubCmd=TSVR_USER_AUDIO;
	cmd.dwSequenceID=PT_AUDIO;
	return m_ClientSocket.SendData(cmd,pAacData,(DWORD)nDataSize);
	
#else
	return m_ClientSocket.SendData(ALL_MAIN_CMD,TSVR_USER_AUDIO,0,pAacData,(DWORD)nDataSize);
#endif
}

// ”∆µ ˝æ›ªÿµ˜
bool CNetDataSyncSink::On264VideoData(BYTE *pVideoData,int nDataSize)
{
	if(!m_bLogined) return false;
	if(m_ClientSocket.GetConnectState() == SocketState_NoConnect)
	{
		return false;
	}
#if !MACIOS
	CMD_Command cmd;
	cmd.wMainCmd=ALL_MAIN_CMD;
	cmd.wSubCmd=TSVR_USER_VIDEO;
	cmd.dwSequenceID=PT_VIDEO;
	return m_ClientSocket.SendData(cmd,pVideoData,(DWORD)nDataSize);
#else
	return m_ClientSocket.SendData(ALL_MAIN_CMD,TSVR_USER_VIDEO,0,pVideoData,(DWORD)nDataSize);
#endif
	
}

// ”∆µSPS+PPSÕ∑ ˝æ›
bool CNetDataSyncSink::OnS264SpsPpsData(BYTE *pHeadData,int nDataSize)
{
	
	if(!m_bLogined) return false;
	if(m_ClientSocket.GetConnectState() == SocketState_NoConnect) return false;
#if !MACIOS
	CMD_Command cmd;
	cmd.wMainCmd=ALL_MAIN_CMD;
	cmd.wSubCmd=TSVR_S_H264_SPS;
	cmd.dwSequenceID=PT_SPSPPS;
	return m_ClientSocket.SendData(cmd,pHeadData,(DWORD)nDataSize);
#else
	return m_ClientSocket.SendData(ALL_MAIN_CMD,TSVR_S_H264_SPS,0,pHeadData,(DWORD)nDataSize);
#endif
}

//∑¢ÀÕŒƒ±æ ˝æ›
bool CNetDataSyncSink::SendTextData(const char *pTxtData)
{
	if(!m_bLogined) return false;
	
	WORD wLength=(WORD)strlen(pTxtData);
#if !MACIOS
	
	CMD_Command cmd;
	cmd.wMainCmd=ALL_MAIN_CMD;
	cmd.wSubCmd=TSVR_USER_TXTMSG;
	cmd.dwSequenceID=PT_TEXT;
	return m_ClientSocket.SendData(cmd,(void *)pTxtData,wLength);
#else
	
	return m_ClientSocket.SendData(ALL_MAIN_CMD,TSVR_USER_TXTMSG,0,(void *)pTxtData,(DWORD)wLength);
	
#endif
}


//«–ªª∏∫‘ÿIP
void CNetDataSyncSink::SwitchLoadServerIpPort()
{
	WORD wConnNum=0xFFFF;
	WORD wServerPort=0;
	TCHAR szServerIP[IPLENGHT]=TEXT("");
	
	if(g_GlobalData.wLbsCount<=1) return;
	
	WORD wIndex=0;
	for(WORD i=0;i<g_GlobalData.wLbsCount;i++)
	{
		WORD wMinuNum=g_GlobalData.mLbsList[i].wConnCount;
		if(wMinuNum<wConnNum)
		{
			wConnNum=wMinuNum;
			wIndex=i;
			lstrcpy(szServerIP,g_GlobalData.mLbsList[i].szLbsIp);
			wServerPort=g_GlobalData.mLbsList[i].wLbsPort;
		}
	}
	
	if(wServerPort!=0)
	{
		lstrcpy(g_GlobalData.mLbsInfo.szLbsIp,szServerIP);
		g_GlobalData.mLbsInfo.wLbsPort=wServerPort;
		g_GlobalData.mLbsList[wIndex].wConnCount++;
	}
}

bool CNetDataSyncSink::OnNotifyScreenCap(int nEventID)
{
	if(m_pINetDataSink!=NULL)
	{
		m_pINetDataSink->NetStatusNotify(nEventID);
	}
	return true;
}

