////////////////////////////////////////////////////////////////////////////////////
//			Android 视频数据类实现文件 AndroidVideoData.cpp
//			2016-08-13 Bowen.hu
/////////////////////////////////////////////////////////////////////////////////////
#include "AndroidVideoData.h"
#include "SockClientSink.h"
#include "custom_log.h"

#define FRAME_SPACE		100				//每秒10帧
#define MAX_SPACE		2000			//最大间隔2000ms(每2秒1帧)
/////////////////////////////////////////////////////////////////////////////////////
CAndroidVideoData::CAndroidVideoData(void)
{
	#ifdef ANDROID_OS
	m_nYuvSize=MAX_WCX*MAX_WCY*3/2;
	m_pYuvData=NULL;//(BYTE*)::malloc(m_nYuvSize);
	m_bHaveKeyFrame=FALSE;
	m_pRgbBuffer=NULL;
	m_nRgbBuffSize=0;
	m_pH264Data=NULL;
	m_nVideoCX=MAX_WCX;
	m_nVideoCY=MAX_WCY;
	m_bHaveRgbData=false;
	m_dwLastAddTime=0;
	m_dwWaitTime=FRAME_SPACE;		//每秒10帧
	m_pIMediaDataManage=NULL;
	#endif
}

CAndroidVideoData::~CAndroidVideoData(void)
{
	#ifdef ANDROID_OS
	//m_H264Decoder.H264DecoderClose();
	//m_H264Encoder.X264EncoderClose();

	if(m_pRgbBuffer!=NULL)
	{
		free(m_pRgbBuffer);
		m_pRgbBuffer=NULL;
	}
	 
	if(m_pH264Data!=NULL)
	{
		free(m_pH264Data);
		m_pH264Data=NULL;
	}
	#endif
}

//音频数据
bool CAndroidVideoData::OnAudioRTData(const void *pAudioData,DWORD dwDataSize)
{
	return true;
}

//H264视频编码数据
bool CAndroidVideoData::OnVideoDecoderData(WORD wChannelID,const void *pH264Data,DWORD dwDataSize)
{
	//if(dwDataSize==0) return false;
	//#ifdef ANDROID_OS
	//BYTE *pYuvData=NULL;
	//int nWidth=0;
	//int nHeight=0;
	//BYTE *pData=(BYTE *)pH264Data;
	//BYTE cbKey=pData[0];
	////NetMsgAutoParam(OT_NOOPTION,TEXT("------Video----OnVideoH264CodeData Key:%d"),cbKey);
	//if(cbKey==0x67 || cbKey==0x68) 
	//{
	//	m_bHaveKeyFrame=FALSE;
	//	m_H264Decoder.H264DecoderDecode((unsigned char *)pH264Data,dwDataSize,&nWidth,&nHeight,&pYuvData);
	//}
	//else if(cbKey==0x65)
	//{
	//	m_bHaveKeyFrame=TRUE;
	//}
	//if(!m_bHaveKeyFrame) return true;
	//
	////NetMsgAutoParam(OT_NOOPTION,TEXT("------Video----OnVideoH264CodeData to decoder,DataSize:%d"),dwDataSize);
	//int nRet=m_H264Decoder.H264DecoderDecode((unsigned char *)pH264Data,dwDataSize,&nWidth,&nHeight,&pYuvData);
	////NetMsgAutoParam(OT_NOOPTION,TEXT("------Video----decoder return ,ret:%d,w:%d,h:%d"),nRet,nWidth,nHeight);
	////发送给Java层
	//if(nRet>0 && nWidth>0)
	//{
	//	OnJavaVideoData(pYuvData,nRet,nWidth,nHeight);
	//}
	//if(pYuvData!=NULL)
	//{
	//	free(pYuvData);
	//	pYuvData=NULL;
	//}
	//#endif
	return true;
}

#ifdef ANDROID_OS
//初始化媒体管理
bool CAndroidVideoData::InitAndroidVideoData(IMediaDataManage *pIMediaDataManage)
{	
	m_pIMediaDataManage=pIMediaDataManage;
	
	/*int nRet=m_H264Decoder.H264DecoderOpen();
	if(nRet!=0)
	{
		NetMsgAutoParam(OI_EXCEPTION,"-------*******----Open h264 decoder falid:%d",nRet);
		return false;
	}*/

	return (m_pIMediaDataManage!=NULL);
}

//设置编码视频大小
bool CAndroidVideoData::SetEncodeVideoSize(int cx,int cy,int nType)
{
	m_nDataType=nType;
	m_nVideoCX=cx;
	m_nVideoCY=cy;
	
	CThreadLockHandle LockHandle(&m_RgbDataLock);
	int nBufferSize=m_nVideoCX*m_nVideoCY*3;
	if(VT_YUV==m_nDataType)
	{
		nBufferSize/=2;
	}
	if(m_pRgbBuffer!=NULL)
	{
		free(m_pRgbBuffer);
		m_pRgbBuffer=NULL;
	}
	BYTE *pBufffer=(BYTE *)::malloc(nBufferSize);
	if(pBufffer==NULL) return false;
	m_pRgbBuffer=pBufffer;
	pBufffer=(BYTE *)::malloc(nBufferSize);
	if(pBufffer==NULL)
	{
		free(m_pRgbBuffer);
		return false;
	}
	m_nRgbBuffSize=nBufferSize;
	m_pH264Data=pBufffer;
	return true;
}

//获取SpsPps数据
int CAndroidVideoData::GetSpsPpsData(BYTE *pSpsPps,int nBufferSize)
{
	BYTE cbSps[256];
	BYTE cbPps[64];
	int nSpsLen=255;
	int nPpsLen=64;
	unsigned char *pSpsData=cbSps;
	unsigned char *pPpsData=cbPps;
	/*int nRet=m_H264Encoder.X264EncodePPSAndSPS(pSpsData,nSpsLen,pPpsData,nPpsLen);
	if(nRet<=0) return 0;*/
	int nDataSize=nSpsLen+nPpsLen+2;
	if(nBufferSize<nDataSize) return 0;
	pSpsPps[0]=(BYTE)nSpsLen;
	pSpsPps++;
	memcpy(pSpsPps,cbSps,nSpsLen);
	pSpsPps+=nSpsLen;
	pSpsPps[0]=(BYTE)nPpsLen;
	pSpsPps++;
	memcpy(pSpsPps,cbPps,nPpsLen);

	return nDataSize;
}

//启动编码线程
bool CAndroidVideoData::StartEncodeThread()
{
	return StartThread();
}

//停止编码线程
bool CAndroidVideoData::StopEncodeThread()
{
	m_bHaveRgbData=false;
	m_bRun=false;
	return StopThread();
}

//添加视频RGB数据
bool CAndroidVideoData::AddVideoRgbData(const void *pRgbData,int nDataSize)
{
	CThreadLockHandle LockHandle(&m_RgbDataLock);
	if(m_pRgbBuffer==0 || m_nRgbBuffSize!=nDataSize) return false;
	memcpy(m_pRgbBuffer,pRgbData,nDataSize);

	m_bHaveRgbData=true;
	DWORD dwTime=GetTickCount();
	DWORD dwSpaceTime=dwTime-m_dwLastAddTime;

	if(dwSpaceTime>FRAME_SPACE)
	{
		if(dwSpaceTime>MAX_SPACE) dwTime=MAX_SPACE;
		m_dwWaitTime=dwSpaceTime;
	}

	m_dwLastAddTime=dwTime;
	return true;
}

//开始通知函数
bool CAndroidVideoData::OnThreadStartEvent()
{
	/*m_bHaveRgbData=false;
	if(!m_H264Encoder.X264EncoderOpen(m_nVideoCX,m_nVideoCY,FRAME_SPACE))
	{
		LOGE(TEXT("-------------*****----打开X264编码器失败"));
		return false;
	}*/
	return true;
}

//线程停止通知函数
bool CAndroidVideoData::OnThreadStopEvent()
{
	return true;
}

//线程体函数
bool CAndroidVideoData::RepetitionRun()
{
	if(m_pRgbBuffer==NULL) return false;
	if(m_pH264Data==NULL) return false;

	//struct timeval delay;
	//delay.tv_sec = 0;
	//delay.tv_usec =m_dwWaitTime*1000;
	//select(0, NULL, NULL, NULL, &delay);
	//if(!m_bRun) return false;

	//CThreadLockHandle LockHandle(&m_RgbDataLock);

	////编码一帧
	//int nBufferSize=m_nRgbBuffSize;
	//int nRet=m_H264Encoder.X264Encode(m_pRgbBuffer,m_pH264Data,nBufferSize);
	//if(nRet>0 && m_pIMediaDataManage!=NULL)
	//{
	//	//发送到网络
	//	m_pIMediaDataManage->SendH264CodeData(m_pH264Data,nRet);
	//}

	return m_bRun;
}

#endif