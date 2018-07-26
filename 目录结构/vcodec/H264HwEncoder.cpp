//
//  H264HwDecoderImpl.h
//  ShiPinHuiYi
//
//  Created by 徐杨 on 16/3/31.
//  Copyright (c) 2016年 feiyuxing. All rights reserved.
//

#include "H264HwEncoder.h"


/////////////////////////////////////////////////////////
CH264HwEncoder::CH264HwEncoder()
{
    frameCount = 0;
    mpIMediaDataSink=NULL;
	bResetEncoding = false;
	
	
	
}

CH264HwEncoder::~CH264HwEncoder()
{
	
}

//设置spspps数据
bool CH264HwEncoder::SetSpsPpsData(const uint8_t * pSps,int nSpsSize,const uint8_t *pPps,int nPpsSize)
{
    bool bSetSps=spsData.SetData((uint8_t *)pSps, nSpsSize);
    bool bSetPps=ppsData.SetData((uint8_t *)pPps, nPpsSize);
    if(bSetSps || bSetPps)
    {
        //发送sps pps数据
        const int DATASIZE=1024;
        uint8_t cbTmpData[DATASIZE];
        uint8_t *pData=cbTmpData;
        *pData++=(uint8_t)nSpsSize;
        memcpy(pData,pSps,nSpsSize);
        pData+=nSpsSize;
        *pData++=(uint8_t)nPpsSize;
        memcpy(pData,pPps,nSpsSize);
        int nSendSize=nSpsSize+nPpsSize+2;
        if(mpIMediaDataSink!=NULL)
            mpIMediaDataSink->OnS264SpsPpsData((BYTE *)cbTmpData, nSendSize);
    }
    
    return true;
}

//编码后h264数据
bool CH264HwEncoder::OnH264VideoData(const uint8_t * pVideoData,int nDataSize)
{
    if(nDataSize>0)
    {
        //发送h264数据
        if(mpIMediaDataSink!=NULL)
		{
			if (!mpIMediaDataSink->On264VideoData((BYTE *)pVideoData, nDataSize)) {
				return false;
			}
			
		}

    }
    return true;
}

void CH264HwEncoder::didCompressH264(void *outputCallbackRefCon, void *sourceFrameRefCon, OSStatus status, VTEncodeInfoFlags infoFlags,
                     CMSampleBufferRef sampleBuffer )
{
    if (status != 0) return;
    
    if (!CMSampleBufferDataIsReady(sampleBuffer))
    {
        printf("didCompressH264 data is not ready ");
        return;
    }
    CH264HwEncoder* encoder = (CH264HwEncoder*)outputCallbackRefCon;
    
    //Boolean CFDictionaryContainsKey(CFDictionaryRef theDict, const void *key);
    //const void *CFArrayGetValueAtIndex(CFArrayRef theArray, CFIndex idx);
    bool keyframe = !CFDictionaryContainsKey((CFDictionaryRef)(CFArrayGetValueAtIndex(CMSampleBufferGetSampleAttachmentsArray(sampleBuffer, true), 0)), kCMSampleAttachmentKey_NotSync);
    
    if (keyframe)
    {
        CMFormatDescriptionRef format = CMSampleBufferGetFormatDescription(sampleBuffer);
        size_t sparameterSetSize, sparameterSetCount;
        const uint8_t *sparameterSet;
        OSStatus statusCode = CMVideoFormatDescriptionGetH264ParameterSetAtIndex(format, 0, &sparameterSet, &sparameterSetSize, &sparameterSetCount, 0 );
        if (statusCode == noErr)
        {
            size_t pparameterSetSize, pparameterSetCount;
            const uint8_t *pparameterSet;
            OSStatus statusCode = CMVideoFormatDescriptionGetH264ParameterSetAtIndex(format, 1, &pparameterSet, &pparameterSetSize, &pparameterSetCount, 0 );
            if (statusCode == noErr)
            {
                encoder->SetSpsPpsData(sparameterSet,sparameterSetSize,pparameterSet,pparameterSetSize);
            }
        }
    }
    
    CMBlockBufferRef dataBuffer = CMSampleBufferGetDataBuffer(sampleBuffer);
    size_t length, totalLength;
    char *dataPointer;
    OSStatus statusCodeRet = CMBlockBufferGetDataPointer(dataBuffer, 0, &length, &totalLength, &dataPointer);
    if (statusCodeRet == noErr) {
        
        size_t bufferOffset = 0;
        static const int AVCCHeaderLength = 4;
        while (bufferOffset < totalLength - AVCCHeaderLength)
        {
            uint32_t NALUnitLength = 0;
            memcpy(&NALUnitLength, dataPointer + bufferOffset, AVCCHeaderLength);
            NALUnitLength = CFSwapInt32BigToHost(NALUnitLength);
            encoder->OnH264VideoData((uint8_t *)dataPointer + bufferOffset + AVCCHeaderLength, NALUnitLength);
            bufferOffset += AVCCHeaderLength + NALUnitLength;
        }
        
    }
    
}

//设置媒体回调接口
bool CH264HwEncoder::SetMediaDataSink(IMediaDataSink *pIMediaDataSink)
{
    mpIMediaDataSink=pIMediaDataSink;
    return (mpIMediaDataSink!=NULL);
}
bool CH264HwEncoder::SetCaptureSink(ICaptureSink *pCaptureSink)
{
	mpCaptureSink = pCaptureSink;
	return (mpCaptureSink != NULL);
}

//初化始化编辑器
bool CH264HwEncoder::InitEncode(int nWidth,int nHeight)
{
	mEncWidth = nWidth;
	mEncHeight = nHeight;
    OSStatus status = VTCompressionSessionCreate(NULL, nWidth, nHeight, kCMVideoCodecType_H264, NULL, NULL, NULL, didCompressH264, (void *)this, &EncodingSession);
    if (status != 0)
    {
        printf("Error by VTCompressionSessionCreate  ");
        return false;
    }
    
    VTSessionSetProperty(EncodingSession, kVTCompressionPropertyKey_RealTime, kCFBooleanTrue);
    VTSessionSetProperty(EncodingSession, kVTCompressionPropertyKey_ProfileLevel, kVTProfileLevel_H264_Baseline_4_1);
    
    SInt32 bitRate = nWidth*nHeight*50;  //越高效果越屌  帧数据越大
    CFNumberRef ref = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &bitRate);
    VTSessionSetProperty(EncodingSession, kVTCompressionPropertyKey_AverageBitRate, ref);
    CFRelease(ref);
    
    int frameInterval = 25; //关键帧间隔 越低效果越屌 帧数据越大
    CFNumberRef  frameIntervalRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &frameInterval);
    VTSessionSetProperty(EncodingSession, kVTCompressionPropertyKey_MaxKeyFrameInterval,frameIntervalRef);
    CFRelease(frameIntervalRef);
    status=VTCompressionSessionPrepareToEncodeFrames(EncodingSession);
    return (status==0);
}

void CH264HwEncoder::ResetEncoding(bool isReset)
{
	bResetEncoding = isReset;
}

//释放编码器
void CH264HwEncoder::ReleaseEncode()
{
    VTCompressionSessionInvalidate(EncodingSession);
}

//编码
int CH264HwEncoder::encode(void *pSampleBuffer)
{
    CVPixelBufferRef pixBuffer=(CVPixelBufferRef)pSampleBuffer;
    
    if (EncodingSession==nil||EncodingSession==NULL) {
		
		if (bResetEncoding == true) {
			InitEncode(mEncWidth, mEncHeight);
			bResetEncoding = false;
			return 0;
		}
		
        return -1;
    }
    
    frameCount++;
    CVImageBufferRef imageBuffer = (CVImageBufferRef)CVPixelBufferRetain(pixBuffer);
    CMTime presentationTimeStamp = CMTimeMake(frameCount, 1000);
    VTEncodeInfoFlags flags;
    OSStatus statusCode = VTCompressionSessionEncodeFrame(EncodingSession,
                                                          imageBuffer,
                                                          presentationTimeStamp,
                                                          kCMTimeInvalid,
                                                          NULL, NULL, &flags);
    if (statusCode != noErr)
    {
        if (EncodingSession!=nil||EncodingSession!=NULL)
        {
			bResetEncoding = false;
            VTCompressionSessionInvalidate(EncodingSession);
            CFRelease(EncodingSession);
            EncodingSession = NULL;
            return -2;
        }
    }
    
    return 0;
}

