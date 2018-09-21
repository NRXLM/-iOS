////////////////////////////////////////////////////////////////////
//      AV音频视管理类实现文件  AVManager.cpp
//      Bowen 2018-01-16
//////////////////////////////////////////////////////////////////////
#include "OitServiceManage.h"
CONFIGDATA	g_GlobalData;

///////////////////////////////////////////////////////////////////////
//构造函数
COitServiceManage::COitServiceManage()
{
    isStop = true;
}

COitServiceManage::~COitServiceManage()
{
}

//初始化对象
bool COitServiceManage::InitAvManager(LPCTSTR lpszLbsList,id<AudioPlaySink> audioSink,id<LFScreenAVKitCallBack> appSink)
{
	midAudio=audioSink;
	midAppSink=appSink;
	g_GlobalData.vcx=DST_V_CX;
	g_GlobalData.vcy=DST_V_CY;
	
	ITimerEngineSink *pITimerEngineSink=(ITimerEngineSink *)this;
	mTimerEngine.SetTimerEngineSink(pITimerEngineSink);
	
	ParseLbsIpList(lpszLbsList);
	
	INetNotifySink *pINetNotifySink=(INetNotifySink *)this;
	mNetDataSyncSink.InitDataSyncThread(pINetNotifySink, NULL);
	mNetDataSyncSink.SetNetDataSink((INetDataSink *)this);
	return true;
}

//设置业务线类型
void COitServiceManage::SetBusinessType(int nType,const char *extValue)
{
    g_GlobalData.nBusinessType=nType;
    int nExtValLen=lstrlen(extValue);
    if(nExtValLen<WNAME_LEN)
    {
        lstrcpy(g_GlobalData.szExtVal,extValue);
    }
}

//开始管理服务
bool COitServiceManage::StartService(DWORD dwServiceCode,DWORD dwClientCode,int vcx,int vcy,int scx,int scy)
{
	g_GlobalData.vcx=vcx;
	g_GlobalData.vcy=vcy;
	g_GlobalData.dwServiceCode=dwServiceCode;
	g_GlobalData.dwClientCode=dwClientCode;
	
	mTimerEngine.StartService();
    
    IMediaDataSink *pIMediaDataSink=(IMediaDataSink *)&mNetDataSyncSink;
    mScreenCapThread.InitEncodeThread(pIMediaDataSink);
    mScreenCapThread.SetVideoSize(vcx, vcy, scx, scy);
    
	mNetDataSyncSink.StartNetThread();
	
	return mNetDataSyncSink.ConnectToServer(CST_LBSVR);
}

//开始管理服务
bool COitServiceManage::StartService(CHAR *jobNum, CHAR *mobile, int vcx, int vcy, int scx, int scy)
{
    if (isStop) {
        isStop = false;
    }else{
        return false;
    }
    
    int nJobjNumLen=lstrlen(jobNum);
    int nMobileLen=lstrlen(mobile);
    if(nJobjNumLen>=MAX_NAME*2)
    {
        mNetDataSyncSink.OnNotifyScreenCap(JNI_SC_1015);
        return false;
    }
    if(nMobileLen>=WNAME_LEN)
    {
        mNetDataSyncSink.OnNotifyScreenCap(JNI_SC_1016);
        return false;
    }
    
    g_GlobalData.wReqJobFaildCount = 0;
    
	lstrcpy(g_GlobalData.szJobNum, jobNum);
	lstrcpy(g_GlobalData.szMobile, mobile);
	g_GlobalData.vcx = vcx;
	g_GlobalData.vcy = vcy;
	//
	mTimerEngine.StartService();
	IMediaDataSink *pIMediaDataSink = (IMediaDataSink*)&mNetDataSyncSink;
	mScreenCapThread.InitEncodeThread(pIMediaDataSink);
	mScreenCapThread.SetVideoSize(vcx, vcy, scx, scy);
	mNetDataSyncSink.StartNetThread();
	
	return mNetDataSyncSink.ConnectToServer(CST_LBSVR);
}

//停止管理服务
bool COitServiceManage::StopService()
{
	mScreenCapThread.StopVideoThread();
	[midAudio StopAudioService];
	mTimerEngine.StopService();
	mNetDataSyncSink.CloseConnect();

    isStop = true;
    
	return true;
}

//获取媒体接口
IMediaDataSink * COitServiceManage::GetMediaInterface()
{
	IMediaDataSink *pIMideaDataSink=(IMediaDataSink *)&mNetDataSyncSink;
	return pIMideaDataSink;
}

//ø™ º ”∆µ≤…ºØ
bool COitServiceManage::StartAVCapThread()
{
    mScreenCapThread.StartVideoThread();
    
	return true;
}
//ø™ º∆Ù∂Ø“Ù∆µ◊Èº˛
bool COitServiceManage::StartAudioService()
{
	if ([midAudio respondsToSelector:@selector(StartAudioService)]) {
		return (bool)[midAudio StartAudioService];
	}
	return true;
}

bool COitServiceManage::StopAudioSevice()
{
	if ([midAudio respondsToSelector:@selector(StopAudioService)]) {
		return (bool)[midAudio StopAudioService];
	}
	return true;
}

bool COitServiceManage::StopAVCapThread()
{
	mScreenCapThread.StopVideoThread();
	return true;
}

//定时器回调函数
bool COitServiceManage::OnEventTimer(WORD wTimerID,WPARAM wBindParam,void * pTimerSink)
{
	if(pTimerSink!=NULL)
	{
		((ITimerSink *)pTimerSink)->OnEventTimer(wTimerID, wBindParam);
	}
	return true;
}

//音频数据通知接口
bool COitServiceManage::AudioDataNotify(const BYTE *pAudioData,DWORD dwDataSize)
{
	if(midAudio==nil) return true;
	
#if HAS_AUDIO
	id<AudioPlaySink> audioSink=(id<AudioPlaySink>)midAudio;
	[audioSink OnAudioDataPlay:(const void *)pAudioData dataSize:dwDataSize];
#endif
	return true;
}

//网络状态通知
bool COitServiceManage::NetStatusNotify(int nEventID)
{
	if(midAppSink==nil) return true;
	//通知 midAppSink
	if ([midAppSink respondsToSelector:@selector(SeviceStateChangeWithStatus:)]) {
		NSObject<LFScreenAVKitCallBack>* mObj = midAppSink;
		dispatch_async(dispatch_get_main_queue(), ^{
		SeviceStatus aSta = (SeviceStatus)nEventID;
			[mObj SeviceStateChangeWithStatus:aSta];
		});
	}
	
	return true;
}

void COitServiceManage::SetHasWKWebView(bool hasWKView)
{
	mScreenCapThread.SetHasWKWebView(hasWKView);
}

void COitServiceManage::ResetWindow()
{
	mScreenCapThread.ResetWindow();
}
//加密字符串
int COitServiceManage::MyEncryptString(const CHAR *pSrcStr,CHAR *pEncStr,int nBufferSize)
{
    return EncCheckCode(pSrcStr,pEncStr,nBufferSize);
    
}

//////////////////////////////////////////////////////////////////////////////////
//ªÒ»°“‘÷∏∂®◊÷∑˚∑÷∏Ùµƒ◊÷∑˚¥Æ
int ParseSubString(LPCTSTR lpszList,const TCHAR *split,CStringVec & slist)
{
	if(lpszList==NULL) return 0;
	
	int nCount=0;
	TCHAR *pSplit = strtok((TCHAR *)lpszList,split);
	while(pSplit!=NULL)
	{
		string strSub=pSplit;
		slist.push_back(strSub);
		pSplit= strtok(NULL,split);
		nCount++;
	}
	
	return nCount;
}

//Ω‚Œˆ∑˛ŒÒ∆˜IPµÿ÷∑
void ParseLbsIpList(LPCTSTR lszTmpIP)
{
	CStringVec slist;
	int nCount=ParseSubString(lszTmpIP,",",slist);
	if(nCount>0)
	{
		TCHAR szSubStr[MAX_PATH];
		for(int i=0;i<nCount && g_GlobalData.wLbsCount<LBSMAXNUM;i++)
		{
			CStringVec itemVec;
			string strItem=slist[i];
			lstrcpy(szSubStr,strItem.c_str());
			int nSubCount=ParseSubString(szSubStr,":",itemVec);
			if(nSubCount==2)
			{
				string strSvrIp=itemVec[0];
				string strPort=itemVec[1];
				WORD wPort=(WORD)atoi(strPort.c_str());
				strcpy(g_GlobalData.mLbsList[g_GlobalData.wLbsCount].szLbsIp,strSvrIp.c_str());
				g_GlobalData.mLbsList[g_GlobalData.wLbsCount++].wLbsPort=wPort;
			}
		}
	}
	if(g_GlobalData.wLbsCount>0)
	{
		strcpy(g_GlobalData.mLbsInfo.szLbsIp,g_GlobalData.mLbsList[0].szLbsIp);
		g_GlobalData.mLbsInfo.wLbsPort=g_GlobalData.mLbsList[0].wLbsPort;
	}
}



