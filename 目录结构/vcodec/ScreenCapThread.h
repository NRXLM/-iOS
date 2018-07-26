//////////////////////////////////////////////////////////////////////////////////////////
//		∆¡ƒªΩÿ∆¡œﬂ≥Ã¿‡Õ∑Œƒº˛ ScreenCapThread.h
//		Author:	bowen.hu
//		Time:	2016-06-09
///////////////////////////////////////////////////////////////////////////////////////////
#ifndef __BOWEN_HU_SCREEN_CAP_THREAD_HEAD_FILE__
#define __BOWEN_HU_SCREEN_CAP_THREAD_HEAD_FILE__

#include "../NetModule/localdef.h"
#include "H264HwEncoder.h"
#include <objc/runtime.h>

#define UIWindowClass "UIWindow"
#define UIApplicationClass "UIApplication"
///////////////////////////////////////////////////////////////////////////////////////////
class CScreenCapThread:public CServiceThread,public ICaptureSink
{
private:
    BYTE				*m_pFrameData;				// ”∆µ÷°ª∫≥Â
    BYTE				*m_pTmpBuffer;				//¡Ÿ ±ª∫≥Â
    BYTE				*m_pEncodeImg;				//±‡¬ÎÕº∆¨ ˝æ›
    BYTE				*m_pZoomBuff;				//Àı∑≈ª∫≥Â
    CThreadLock			m_EncodeLock;				//±‡¬ÎÀ¯
    CH264HwEncoder      m_H264HwEncoder;
    
private:
    int					m_nImgWidth;				//Õº∆¨øÌ
    int					m_nImgHeight;				//Õº∆¨∏ﬂ
    int					m_nImgSize;					//Õº∆¨¥Û–°
    int					m_nFrameCX;					//≤…ºØ÷°øÌ
    int					m_nFrameCY;					//≤…ºØ÷°∏ﬂ
    int					m_nFrameSize;				//÷°¥Û–°
    int					m_nZoomCX;					//Àı∑≈øÌ
    int					m_nZoomCY;					//Àı∑≈∏ﬂ
    int					m_nOffsetX;					//X∆´“∆¡ø
    int					m_nOffsetY;					//Y∆´“∆¡ø
    //标记为
	bool                mHasWebView;
private:
    id            		mainWindow;
    id	             	mainLayer;
    CVPixelBufferRef    dstPixbuffer;                  //要返回的图片
    
public:
	CScreenCapThread();
	~CScreenCapThread();
	
	
	
public:
    //…Ë÷√ ”∆µ¥Û–°
    void SetVideoSize(int codecx,int codecy,int screencx,int screency);
    //≥ı ºªØ
    bool InitEncodeThread(IMediaDataSink *pIMediaDataSink);
    //ø™ º±‡¬Îœﬂ≥Ã
    bool StartVideoThread();
    //Õ£÷π±‡¬Îœﬂ≥Ã
    bool StopVideoThread();
	
	void SetHasWKWebView(bool bHasWebView);
	id   GetCurrentWKWebView(id subViews);
	
	void ResetWindow();
	
protected:
    //ø™ ºÕ®÷™∫Ø ˝
    virtual bool OnThreadStartEvent();
    //œﬂ≥ÃÕ£÷πÕ®÷™∫Ø ˝
    virtual bool OnThreadStopEvent();
    
	
public:
	virtual bool StopCaptureThread();
protected:
//     线程毁掉函数
     virtual bool RepetitionRun();
    
private:
    //采集视频
    void CaptureVideo();
};

//双线性插值图片缩放
void ResizeLinearImgZoom(Byte* pSrc,int sw,int sh,Byte* pDst,int dw,int dh,int xOffset,int yOffset);

#endif //__BOWEN_HU_SCREEN_CAP_THREAD_HEAD_FILE__
