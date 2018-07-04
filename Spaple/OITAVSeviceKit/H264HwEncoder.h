//
//  H264HwEncoderImpl.h
//  h264v1
//
//  Created by Ganvir, Manish on 3/31/15.
//  Copyright (c) 2015 Ganvir, Manish. All rights reserved.
//


#include <VideoToolbox/VideoToolbox.h>
#include "config.h"
#include "../NetModule/localdef.h"

struct ICaptureSink
{
	virtual bool StopCaptureThread()=0;
};

//////////////////////////////////////////////////////////////////////////
class CH264HwEncoder
{
private:
    typedef struct tagDataBuff
    {
        int         nDataSize;
        uint8_t     *pData;
        
        tagDataBuff()
        {
            pData=NULL;
            nDataSize=0;
        }
        ~tagDataBuff()
        {
            free(pData);
        }
        //设置数据
        bool SetData(uint8_t *pDataBuffer,int nSize)
        {
            if(nDataSize!=nSize)
            {
                if(pData!=NULL)
                {
                    free(pData);
                    pData=NULL;
                }
                pData=(uint8_t *)::malloc(nSize);
                if(pData==NULL) return false;
                memcpy(pData,pDataBuffer,nSize);
                return true;
            }
            else
            {
                uint8_t *pSrcData=pData;
                uint8_t *pDstData=pDataBuffer;
                bool bSame=true;
                for(int i=0;i<nSize;i++)
                {
                    if(*pSrcData!=*pDstData)
                    {
                        bSame=false;
                        break;
                    }
                    pSrcData++;
                    pDstData++;
                }
                if(!bSame)
                {
                    memcpy(pData,pDataBuffer,nSize);
                    return true;
                }
            }
            return false;
        }
        
    }DATABUFF;
    
private:
    VTCompressionSessionRef EncodingSession;
    CMFormatDescriptionRef  format;
    CMSampleTimingInfo      *timingInfo;
    int                     frameCount;
    tagDataBuff              spsData;
    tagDataBuff              ppsData;
    
    IMediaDataSink          *mpIMediaDataSink;                  //媒体接口
	ICaptureSink            *mpCaptureSink;
	
	bool                     bResetEncoding;
	
	int 					 mEncWidth;
	int                      mEncHeight;
	
public:
    CH264HwEncoder();
    virtual ~CH264HwEncoder();
    
public:
    //初化始化编辑器
    bool InitEncode(int nWidth,int nHeight);
    //释放编码器
    void ReleaseEncode();
    //编码
    int encode(void *pSampleBuffer);
    //设置媒体回调接口
    bool SetMediaDataSink(IMediaDataSink *pIMediaDataSink);
	bool SetCaptureSink(ICaptureSink *pCaptureSink);
	void ResetEncoding(bool isReset);
private:
    //设置spspps数据
    bool SetSpsPpsData(const uint8_t * pSps,int nSpsSize,const uint8_t *pPps,int nPpsSize);
    //编码后h264数据
    bool OnH264VideoData(const uint8_t * pVideoData,int nDataSize);
    
private:
    //编码回调函数
    static void didCompressH264(void *outputCallbackRefCon, void *sourceFrameRefCon, OSStatus status, VTEncodeInfoFlags infoFlags,
                                CMSampleBufferRef sampleBuffer);
    


};

//////////////////////////////////////////////////////////////////////////

