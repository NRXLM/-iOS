//
//  OitServiceManage.h
//  vh264enc
//
//  Created by YangXu on 2018/1/17.
//  Copyright © 2018年 OIT. All rights reserved.
//

#ifndef AVManager_h
#define AVManager_h
#include "NetDataSyncSink.h"
#include "TimerEngine.h"
#include "OBNetCallBackSink.h"
#include "LFScreenAVKit.h"
#include "ScreenCapThread.h"
#include "EncOrDec.h"

//////////////////////////////////////////////////////////////
class COitServiceManage: public INetNotifySink,public ITimerEngineSink,public INetDataSink
{
private:
	id<AudioPlaySink>			    midAudio;                          //音频接口对象
	id<LFScreenAVKitCallBack>       midAppSink;                        //app层状态回调id
private:
	CNetDataSyncSink                mNetDataSyncSink;                 //网络对象
    CScreenCapThread                mScreenCapThread;                   //视频对象
	
private:
	CTimerEngine                    mTimerEngine;                     //定时器对象
	
public:
	//构造函数
	COitServiceManage();
	~COitServiceManage();
	
public:
	//初始化对象
	bool InitAvManager(LPCTSTR lpszLbsList,id<AudioPlaySink> audioSink,id<LFScreenAVKitCallBack> appSink);
	bool InitAvManager(char* lps);
    //设置业务线类型
    void SetBusinessType(int nType,const char *extValue);
	//开始管理服务
	bool StartService(DWORD dwServiceCode,DWORD dwClientCode,int vcx,int vcy,int scx,int scy);
	//开启服务管理
	bool StartService(CHAR *jobNum,CHAR *mobile,int vcx,int vcy,int scx,int scy);
	
	//停止管理服务
	bool StopService();
	//获取媒体接口
	IMediaDataSink *GetMediaInterface();
	//设置页面是否包含WKWebView
	void SetHasWKWebView(bool hasWKView);
	void ResetWindow();
    //加密字符串
    int MyEncryptString(const CHAR *pSrcStr,CHAR *pEncStr,int nBufferSize);
    
public:
	//ø™ º ”∆µ≤…ºØ
	virtual bool StartAVCapThread();
	//ø™ º∆Ù∂Ø“Ù∆µ◊Èº˛
	virtual bool StartAudioService();
	
	virtual bool StopAVCapThread();
	virtual bool StopAudioSevice();
	
	//定时器回调函数
	virtual bool OnEventTimer(WORD wTimerID,WPARAM wBindParam,void * pTimerSink);
	//音频数据通知接口
	virtual bool AudioDataNotify(const BYTE *pAudioData,DWORD dwDataSize);
	//网络状态通知
	virtual bool NetStatusNotify(int nEventID);
    
public:
    bool                             isStop; //是否停止
};

/////////////////////////////////////////////////////
void ParseLbsIpList(LPCTSTR lszTmpIP);

#endif /* AVManager_h */

