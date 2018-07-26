////////////////////////////////////////////////////////////////////////////////////
//			媒体数据类实现文件 MediaDataManage.cpp
//			2016-08-13 Bowen.hu
/////////////////////////////////////////////////////////////////////////////////////
#include "MediaDataManage.h"
#include "SockClientSink.h"

/////////////////////////////////////////////////////////////////////////////////////
CMediaDataManage::CMediaDataManage(void)
{
	m_pIMediaDataSink=NULL;
	m_pIKernelDataSink=NULL;
	m_dwAudioSeqID=0;
	#ifdef ANDROID_OS
	m_pIAudioManager=NULL;
	m_VideoData.wDataSize=0;
	m_VideoData.wBufferSize=65535;
	m_VideoData.pDataBuffer=(BYTE*)::malloc(m_VideoData.wBufferSize);
	#endif
}

CMediaDataManage::~CMediaDataManage(void)
{
	#ifdef ANDROID_OS
	sem_destroy(&m_semt);
	if(m_VideoData.pDataBuffer!=NULL)
	{
		free(m_VideoData.pDataBuffer);
		m_VideoData.pDataBuffer=NULL;
		m_VideoData.wBufferSize=0;
	}
	#endif
}

//初始化媒体管理
bool CMediaDataManage::InitMediaManage()
{
	#ifdef ANDROID_OS
	IMediaDataManage *pIMediaDataManage=(IMediaDataManage *)this;
	m_AndroidVideoData.InitAndroidVideoData(pIMediaDataManage);
	#endif

	return true;
}

//设置聊天接口
bool CMediaDataManage::SetKernelDataSink(IKernelDataSink *pIKernelDataSink)
{
	m_pIKernelDataSink = pIKernelDataSink;
	return true;
}

//开始媒体管理服务
bool CMediaDataManage::StartMediaManage()
{
	bool bSuccess=true;
	#ifdef ANDROID_OS
	int nRet=sem_init(&m_semt,0,0);
    if(nRet!=0) return false;
	bSuccess=StartThread();
	#endif

	return bSuccess;
}
//停止媒体管理服务
bool CMediaDataManage::StopMediaManage()
{
	#ifdef ANDROID_OS
	m_bRun=false;
	sem_post(&m_semt);
	StopThread(1);
	#endif

	return true;
}

//设置黑板数据回调接口
bool CMediaDataManage::SetMediaDataSink(IMediaDataSink *pIMediaDataSink)
{
	m_pIMediaDataSink=pIMediaDataSink;
	return (m_pIMediaDataSink!=NULL);
}

#ifdef ANDROID_OS
//设置音频管理接口
bool CMediaDataManage::SetAudioManage(IAudioManager *pIAudioManager)
{
	m_pIAudioManager=pIAudioManager;
	return (m_pIAudioManager!=NULL);
}

//设置视频编码大小(nType=0 为RGB数据,nType=1为YUV数据)
bool CMediaDataManage::SetVideoEncodeSize(int cx,int cy,int nType)
{
	return m_AndroidVideoData.SetEncodeVideoSize(cx,cy,nType);
}

//开始视频编码
bool CMediaDataManage::StartVideoEncode()
{
	return m_AndroidVideoData.StartEncodeThread();
}

//停止视频编码
bool CMediaDataManage::StopVideoEncode()
{
	return m_AndroidVideoData.StopEncodeThread();
}

//添加视步视频数据
int CMediaDataManage::AddVideoData(const void *pVideoData,int nDataSize)
{
	return m_AndroidVideoData.AddVideoRgbData(pVideoData,nDataSize);
}

#endif
//音频数据
bool __cdecl CMediaDataManage::OnAudioRTData(const void *pAudioData,DWORD dwDataSize,DWORD dwTimeStamp)
{
	#ifdef ANDROID_OS
	if(m_pIAudioManager!=NULL)
		m_pIAudioManager->PlayAacAudioData(pAudioData,(int)dwDataSize);
	#else
	if(m_dwAudioSeqID>65535)
		m_dwAudioSeqID=0;

	if(m_pIMediaDataSink!=NULL)
		m_pIMediaDataSink->OnAudioRTData(pAudioData,dwDataSize,m_dwAudioSeqID);

	m_dwAudioSeqID++;
	#endif
	return true;
}

//视频H264SpsPps数据
bool __cdecl CMediaDataManage::OnH264SpsPpsData(DWORD dwChannelID,const void * pSpsPps,DWORD dwDataSize)
{
	if(pSpsPps==NULL) return false;
	if(dwDataSize<2) return true;

	const BYTE *pDstData=(const BYTE *)pSpsPps;
	WORD wSpsSize=pDstData[0];
	BYTE *pSpsData=(BYTE *)pDstData+1;
	BYTE *pPpsData=pSpsData+wSpsSize;
	WORD wPpsSize=pPpsData[0];
	pPpsData+=1;
	if(wSpsSize==0 || wSpsSize>256) return false;
	if(wPpsSize==0 || wPpsSize>128) return false;

	#ifndef ANDROID_OS
	m_pIMediaDataSink->OnH264SpsPpsData((WORD)dwChannelID,pSpsData,wSpsSize);
	m_pIMediaDataSink->OnH264SpsPpsData((WORD)dwChannelID,pPpsData,wPpsSize);
	return true;
	#else
	CThreadLockHandle LockHandle(&m_DataLock);
	m_DataStorage.AddData(AVT_AUDIO,dwChannelID,0,pSpsData,wSpsSize);
	sem_post(&m_semt);
	m_DataStorage.AddData(AVT_AUDIO,dwChannelID,0,pPpsData,wPpsSize);
	sem_post(&m_semt);
	#endif
	
	NetMsgAutoParam(OT_NOOPTION,TEXT("------Add-----OnH264SpsPpsData,DataSize:%d"),dwDataSize);
	return true;
}

//H264视频编码数据
bool __cdecl CMediaDataManage::OnVideoH264CodeData(DWORD dwChannelID,const void *pH264Data,DWORD dwDataSize)
{
	//NetMsgAutoParam(OT_NOOPTION,TEXT("------Add-----OnVideoH264CodeData,DataSize:%d"),dwDataSize);
	//return true;
	#ifdef ANDROID_OS
	CThreadLockHandle LockHandle(&m_DataLock);
	m_DataStorage.AddData(AVT_AUDIO,dwChannelID,0,(void *const)pH264Data,(WORD)dwDataSize);
	sem_post(&m_semt);
	#else
	if(m_pIMediaDataSink!=NULL)
		m_pIMediaDataSink->OnVideoH264CodeData((WORD)dwChannelID,pH264Data,dwDataSize);
	#endif

	return true;
}

//错误提示信息
bool __cdecl CMediaDataManage::OnErrMsgNotify(int nErrCode,LPCTSTR lpszMsgTxt)
{
	#ifdef ANDROID_OS
	TCHAR szMsg[MAX_PATH]=TEXT("");
	sprintf(szMsg,TEXT("{\"errcode\":%d,\"msg\":\"%s\"}"),nErrCode,lpszMsgTxt);
	OnJavaErrMsg(szMsg);
	#else
	if(m_pIKernelDataSink!=NULL)
        m_pIKernelDataSink->OnLoginRespNotify(0);
//		m_pIKernelDataSink->OnErrMsgNotify(nErrCode,lpszMsgTxt);
	#endif
	return true;
}

//发送H264编码数据
bool __cdecl CMediaDataManage::SendH264CodeData(const void *pH264Data,DWORD dwDataSize)
{
	return true;
}

#ifdef ANDROID_OS
//开始通知函数
bool CMediaDataManage::OnThreadStartEvent()
{
	return (m_VideoData.pDataBuffer!=NULL);
}

//线程停止通知函数
bool CMediaDataManage::OnThreadStopEvent()
{
	return true;
}

//线程体函数
bool CMediaDataManage::RepetitionRun()
{
	sem_wait(&m_semt); 
	if(!m_bRun) return false;
	CThreadLockHandle LockHandle(&m_DataLock);
	tagDataHead dHead={0};
	//NetMsgAutoParam(OT_NOOPTION,TEXT("------Video----Start GetData,buffer size:%d"),m_VideoData.wBufferSize);
	m_DataStorage.GetData(dHead,m_VideoData.pDataBuffer,m_VideoData.wBufferSize);
	if(dHead.wDataSize==0) return true;

	m_AndroidVideoData.OnVideoDecoderData(dHead.wChannelID,m_VideoData.pDataBuffer,dHead.wDataSize);

	return m_bRun;
}

#endif
