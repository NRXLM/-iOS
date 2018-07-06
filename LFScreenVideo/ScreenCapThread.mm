//////////////////////////////////////////////////////////////////////////////////////////
//		∆¡ƒªΩÿ∆¡œﬂ≥Ã¿‡Õ∑Œƒº˛ ScreenCapThread.cpp
//		Author:	bowen.hu
//		Time:	2016-06-09
///////////////////////////////////////////////////////////////////////////////////////////
//#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <WebKit/WebKit.h>
#include "ScreenCapThread.h"
#include <objc/message.h>

extern "C"
{

void     UIGraphicsEndImageContext(void);
void UIGraphicsBeginImageContext(CGSize size);
CGContextRef __nullable UIGraphicsGetCurrentContext(void);
	size_t CGBitmapContextGetWidth(CGContextRef);
	size_t CGBitmapContextGetHeight(CGContextRef context);
};
///////////////////////////////////////////////////////////////////////////////////////////
CScreenCapThread::CScreenCapThread()
{
    m_pFrameData=NULL;
    m_pTmpBuffer=NULL;
    m_pEncodeImg=NULL;
    m_pZoomBuff=NULL;
    m_nFrameSize=0;
    m_nImgSize=0;
    
    m_nImgWidth=V_WIDTH;
    m_nImgHeight=V_HEIGHT;
    m_nFrameCX=V_WIDTH;
    m_nFrameCY=V_HEIGHT;
    
    m_nOffsetX=0;
    m_nOffsetY=0;
    
    mainWindow = NULL;
    mainLayer    = NULL;
    
    dstPixbuffer=NULL;
	mHasWebView = false;
}

CScreenCapThread::~CScreenCapThread()
{
    if(m_pFrameData!=NULL)
    {
        free(m_pFrameData);
    }
    if(m_pZoomBuff!=NULL)
    {
        free(m_pZoomBuff);
    }
    if(m_pTmpBuffer!=NULL)
    {
        free(m_pTmpBuffer);
    }
    if(m_pEncodeImg!=NULL)
    {
        free(m_pEncodeImg);
    }
    
    if(dstPixbuffer!=NULL)
    {
        CVPixelBufferRelease(dstPixbuffer);
    }
}

//…Ë÷√ ”∆µ¥Û–°
void CScreenCapThread::SetVideoSize(int codecx,int codecy,int screencx,int screency)
{
    LOGI("----=========---SetVideoSize:code size:%dx%d,screen size:%dx%d",codecx,codecy,screencx,screency);
    
    if(screencx==0 || screency==0)
        return;
    if(codecx==0 || codecy==0)
        return;
    
    m_nFrameCX=screencx;
    m_nFrameCY=screency;
    if(codecx>V_WIDTH)
    {
        m_nImgWidth=V_WIDTH;
    }
    else
    {
        m_nImgWidth=codecx;
    }
    double dbScreen=(double)screencx/(double)screency;
    
    m_nImgWidth=((int)(m_nImgWidth/16))*16;
    m_nImgHeight=m_nImgWidth/dbScreen;
    m_nImgHeight=((int)m_nImgHeight/16)*16;
    
    double dbImage=(double)m_nImgWidth/(double)m_nImgHeight;
    //m_nZoomCX
    if(dbScreen>dbImage)
    {
        m_nZoomCY=m_nImgHeight;
        m_nZoomCX=(int)(m_nZoomCY*dbScreen);
    }
    else
    {
        m_nZoomCX=m_nImgWidth;
        m_nZoomCY=(int)(m_nZoomCX/dbScreen);
    }
    
    m_nOffsetX=(m_nImgWidth-m_nZoomCX)/2;
    m_nOffsetY=(m_nImgHeight-m_nZoomCY)/2;
    LOGI("----=========---SetVideoSize success:%dx%d",m_nImgWidth,m_nImgHeight);
}

//≥ı ºªØ
bool CScreenCapThread::InitEncodeThread(IMediaDataSink *pIMediaDataSink)
{
    m_H264HwEncoder.SetMediaDataSink(pIMediaDataSink);
	
	Class UIWindow = objc_getClass(UIWindowClass);
	Class UIApplication = objc_getClass(UIApplicationClass);
	
//	id    applicationO  = objc_msgSend((id)UIApplication,sel_registerName("sharedApplication"));
//
//	id   delegate      = objc_msgSend(applicationO,sel_registerName("delegate"));
//	mainWindow         = objc_msgSend(delegate, sel_registerName("window"));
//	mainLayer          = objc_msgSend(mainWindow, sel_registerName("layer"));
	mainWindow = [[[UIApplication sharedApplication] delegate] window];
	
    mainLayer    = [(UIView*)mainWindow layer];
//
	
    return (mainLayer!=NULL);
}

//ø™ º±‡¬Îœﬂ≥Ã
bool CScreenCapThread::StartVideoThread()
{
    LOGI("----=========---StartVideoThread");
    
    if(!m_H264HwEncoder.InitEncode(m_nImgWidth, m_nImgHeight))
        return false;
	m_H264HwEncoder.SetCaptureSink((ICaptureSink *)this);
    m_nImgSize=m_nImgWidth*m_nImgHeight*4;
    m_nFrameSize=m_nFrameCX*m_nFrameCY*4;
    if(m_pFrameData!=NULL)
    {
        free(m_pFrameData);
        m_pFrameData=NULL;
    }
    m_pFrameData=(BYTE *)malloc(m_nFrameSize);
    if(m_pFrameData==NULL)
    {
        LOGE("----=========---m_pFrameData malloc faild");
        return false;
    }
    
    if(m_pZoomBuff!=NULL)
    {
        free(m_pZoomBuff);
        m_pZoomBuff=NULL;
    }
    m_pZoomBuff=(BYTE *)malloc(m_nFrameSize);
    if(m_pZoomBuff==NULL)
    {
        LOGE("----=========---m_pZoomBuff malloc faild");
        return false;
    }
    
    if(m_pTmpBuffer!=NULL)
    {
        free(m_pTmpBuffer);
    }
    m_pTmpBuffer=(BYTE *)malloc(m_nImgSize);
    if(m_pTmpBuffer==NULL)
    {
        LOGE("----=========---m_pTmpBuffer malloc faild");
        return false;
    }
    
    if(m_pEncodeImg!=NULL)
    {
        free(m_pEncodeImg);
        m_pEncodeImg=NULL;
    }
    
    m_pEncodeImg=(BYTE *)malloc(m_nImgSize);
    if(m_pEncodeImg==NULL)
    {
        LOGE("----=========---m_pEncodeImg malloc faild");
        return false;
    }
    
    if(dstPixbuffer!=NULL)
    {
        CVPixelBufferRelease(dstPixbuffer);
    }
    //实例化目标PixImg
    CVPixelBufferCreateWithBytes(kCFAllocatorDefault, m_nImgWidth, m_nImgHeight, kCVPixelFormatType_32BGRA, m_pEncodeImg, m_nImgWidth*4, NULL, NULL, NULL, &dstPixbuffer);
    
    
    bool bSuccess=StartThread();
    if(!bSuccess)
    {
        LOGE("----=========---Video Encode Thread faild");
    }
    else
    {
        LOGI("----=========---StartVideoThread success");
    }
    return bSuccess;
}

//Õ£÷π±‡¬Îœﬂ≥Ã
bool CScreenCapThread::StopVideoThread()
{
    m_bRun=false;
    
    bool bSuccess=StopThread(1);
    
    m_H264HwEncoder.ReleaseEncode();
    
    return bSuccess;
}

bool CScreenCapThread::StopCaptureThread()
{
	return StopVideoThread();
}

//ø™ ºÕ®÷™∫Ø ˝
bool CScreenCapThread::OnThreadStartEvent()
{
    return true;
}

//œﬂ≥ÃÕ£÷πÕ®÷™∫Ø ˝
bool CScreenCapThread::OnThreadStopEvent()
{
    return true;
}

//œﬂ≥ÃÃÂ∫Ø ˝
bool CScreenCapThread::RepetitionRun()
{
	MyThreadWait(200);
	if(m_bRun)
	{
        CaptureVideo();
	}

	return m_bRun;
}
void CScreenCapThread::SetHasWKWebView(bool bHasWebView)
{
	mHasWebView = bHasWebView;
}

void CScreenCapThread::ResetWindow()
{
	m_H264HwEncoder.ResetEncoding(true);
}

id CScreenCapThread::GetCurrentWKWebView(id subViews)
{
	NSArray *curSubViews = (NSArray*)subViews;
	for (UIView *view in curSubViews) {
		if ([view isKindOfClass:[WKWebView class]]) {
			return view;
		}else{
			UIView *wkView = GetCurrentWKWebView(view.subviews);
			if (wkView != nil) {
				return wkView;
			}
		}
	}
	return nil;
}

//采集视频
void CScreenCapThread::CaptureVideo()
{
    CGSize mcSize;
    mcSize.width=m_nFrameCX;
    mcSize.height=m_nFrameCY;
	
	dispatch_async(dispatch_get_main_queue(), ^{
		
		@autoreleasepool{
		
			UIWindow *windox = (UIWindow*)mainWindow;
			
		    // 创建一个位图上下文
			UIGraphicsBeginImageContext(mcSize);
            // 获取当前上下文
			CGContextRef context = UIGraphicsGetCurrentContext();
			// 定义一个函数指针action
            void (*action)(id, SEL, CGContextRef) = (void (*)(id, SEL, CGContextRef))objc_msgSend;
            // 调用CAlayer的renderInContext方法
            action(mainLayer, sel_registerName("renderInContext:"), context);
			if (mHasWebView) {
				UIView *drawView = GetCurrentWKWebView(windox.subviews);
				CGRect frame = [drawView convertRect:drawView.bounds toView:windox];
				for(UIView *wkSubView in drawView.subviews)
				{
                    // WKWebView截图
					[wkSubView drawViewHierarchyInRect:frame afterScreenUpdates:YES];
				}
			}
			
			
			
			Byte *data = (Byte*)CGBitmapContextGetData(context);
			if(data==NULL) return ;
			memcpy(m_pFrameData, data, m_nFrameSize);
			
			UIGraphicsEndImageContext();
		}
	});

    CVPixelBufferUnlockBaseAddress(dstPixbuffer, 0);
    //缩放处理
    BYTE *pImgData=m_pFrameData;
    if(m_nFrameCX != m_nZoomCX || m_nFrameCY != m_nZoomCY)
    {
	ResizeLinearImgZoom(m_pFrameData,m_nFrameCX,m_nFrameCY,m_pZoomBuff,m_nZoomCX,m_nZoomCY,0,0);
        pImgData=m_pZoomBuff;
    }
    //居中偏移
    Byte* pSrcData = NULL;
    int nSrcBand = 0;
    int nDstBand = 0;
    Byte *pOffSrc;
    Byte *pOffDst;
    
    nSrcBand=m_nZoomCX*4;
    nDstBand=m_nImgWidth*4;
    
    if(m_nOffsetX>0)
    {
        
        pOffSrc=pImgData;
        pOffDst=m_pEncodeImg+nDstBand*m_nOffsetY;
        for(int y=0;y<m_nZoomCY;y++)
        {
            memcpy(pOffDst,pOffSrc,nSrcBand);
            pOffSrc+=nSrcBand;
            pOffDst+=nDstBand;
        }
        pSrcData=NULL;
    }
    else if(m_nOffsetY>0)
    {
        pOffSrc=pImgData;
        pOffDst=m_pEncodeImg+nDstBand*m_nOffsetY;
        memcpy(pOffDst,pOffSrc,nSrcBand*m_nZoomCY);
        pSrcData=NULL;
    }
    else
    {
        memcpy(m_pEncodeImg,pImgData,m_nImgSize);
    }
    //居中偏移结束
    CVPixelBufferLockBaseAddress(dstPixbuffer, 0);
    
    
    m_H264HwEncoder.encode((void *)dstPixbuffer);

}

//////////////////////////////////////////////////////////////////////////
//双线性插值图片缩放
void ResizeLinearImgZoom(Byte* pSrc,int sw,int sh,Byte* pDst,int dw,int dh,int xOffset,int yOffset)
{
    
#define BYTE Byte
    int w0 = sw;
    int h0 = sh;
    int pix0=4;
    int pitch0 = sw*pix0;
    
    int w1 = dw;
    int h1 = dh;
    int pix1=4;
    int pitch1 = dw*pix1;
    
    BYTE *p1 = pDst;
    
    float fw = float(w0-1) / (w1-1);
    float fh = float(h0-1) / (h1-1);
    
    float x0, y0;
    int y1, y2, x1, x2;
    float fx1, fx2, fy1, fy2;
    
    int* arr_x1 = new int[w1];
    int* arr_x2 = new int[w1];
    float* arr_fx1 = new float[w1];
    
    for(int x=0; x<w1; x++)
    {
        x0 = x*fw;
        arr_x1[x] = int(x0);
        arr_x2[x] = int(x0+0.5f);
        arr_fx1[x] = x0 - arr_x1[x];
    }
    
    for(int y=0; y<h1; y++)
    {
        y0 = y*fh;
        y1 = int(y0);
        y2 = int(y0+0.5f);
        fy1 = y0-y1;
        fy2 = 1.0f - fy1;
        for(int x=0; x<w1; x++)
        {
            x1 = arr_x1[x];
            x2 = arr_x2[x];
            fx1 = arr_fx1[x];
            fx2 = 1.0f-fx1;
            
            float s1 = fx2*fy2;
            float s2 = fx1*fy2;
            float s3 = fx1*fy1;
            float s4 = fx2*fy1;
            BYTE* p11 = pSrc + pitch0*y1 + pix0*x1;
            BYTE* p12 = pSrc + pitch0*y1 + pix0*x2;
            BYTE* p21 = pSrc + pitch0*y2 + pix0*x1;
            BYTE* p22 = pSrc + pitch0*y2 + pix0*x2;
            
            *p1 = BYTE((*p11)*s1 + (*p12)*s2 + (*p21)*s4 + (*p22)*s3);    p1++;    p11++; p12++; p21++; p22++;
            *p1 = BYTE((*p11)*s1 + (*p12)*s2 + (*p21)*s4 + (*p22)*s3);    p1++;    p11++; p12++; p21++; p22++;
            *p1 = BYTE((*p11)*s1 + (*p12)*s2 + (*p21)*s4 + (*p22)*s3);  p1++;
            *p1 =0;	p1++;
        }
        p1 = pDst + y*pitch1;
    }
    
    delete []arr_x1;
    delete []arr_x2;
    delete []arr_fx1;
}
