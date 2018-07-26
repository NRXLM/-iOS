////////////////////////////////////////////////////////////////////////////////////
//			Android 视频数据类头文件 AndroidVideoData.h
//			2016-08-13 Bowen.hu
/////////////////////////////////////////////////////////////////////////////////////
#ifndef __BOWEN_HU_ANDROID_VIDEO_DATA_HEAD_FILE__
#define __BOWEN_HU_ANDROID_VIDEO_DATA_HEAD_FILE__

#include "ClientModule.h"
#include "localdef.h"
#ifdef ANDROID_OS
#include "../VideoCodec/libVideoDecoder/decode2.h"
#include "../VideoCodec/libVideoEncoder/x264_encoder.h"
#endif

#define MAX_WCX			640				//最大视频宽
#define MAX_WCY			480				//最大视频高
#define VT_RGB			0				//RGB数据
#define VT_YUV			1				//YUV数据
/////////////////////////////////////////////////////////////////////////////////////
class CAndroidVideoData
	#ifdef ANDROID_OS
	: public CServiceThread
	#endif
{
#ifdef ANDROID_OS
private:
	BYTE						*m_pYuvData;				//YUV数据
	int							m_nYuvSize;					//Yuv数据大小
	BOOL						m_bHaveKeyFrame;			//有关键帧
	//CH264Decoder				m_H264Decoder;				//H264解码器

private:
	int							m_nDataType;				//数据类型
	int							m_nVideoCX;					//视频宽
	int							m_nVideoCY;					//视频高
	BYTE						*m_pRgbBuffer;				//重采样RGB缓冲
	int							m_nRgbBuffSize;				//RGB缓冲大小
	BYTE						*m_pH264Data;				//H264数据
	//CX264Encoder				m_H264Encoder;				//H264编码器
	CThreadLock					m_RgbDataLock;				//视频数据锁
	bool						m_bHaveRgbData;				//有RGB数据标志
	DWORD						m_dwLastAddTime;			//最后一次添加RGB时间
	DWORD						m_dwWaitTime;				//帧间隔时间
	IMediaDataManage			*m_pIMediaDataManage;		//媒体管理接口
#endif

public:
	CAndroidVideoData(void);
	~CAndroidVideoData(void);

public:
	#ifdef ANDROID_OS
	//初始化媒体管理
	bool InitAndroidVideoData(IMediaDataManage *pIMediaDataManage);
	//设置编码视频大小
	bool SetEncodeVideoSize(int cx,int cy,int nType);
	//添加视频RGB数据
	bool AddVideoRgbData(const void *pRgbData,int nDataSize);
	//获取SpsPps数据
	int GetSpsPpsData(BYTE *pSpsPps,int nBufferSize);
	//启动编码线程
	bool StartEncodeThread();
	//停止编码线程
	bool StopEncodeThread();

protected:
	//开始通知函数
	virtual bool OnThreadStartEvent();
	//线程停止通知函数
	virtual bool OnThreadStopEvent();
	//线程体函数
	virtual bool RepetitionRun();
	#endif

public:
	//音频数据
	bool OnAudioRTData(const void *pAudioData,DWORD dwDataSize);
	//解码H264视频数据
	bool OnVideoDecoderData(WORD wChannelID,const void *pH264Data,DWORD dwDataSize);

};

#endif //__BOWEN_HU_ANDROID_VIDEO_DATA_HEAD_FILE__
