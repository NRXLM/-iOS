//
//  LFScreenVideo.m
//  LFScreenVideo
//
//  Created by friday on 2018/1/14.
//  Copyright © 2018年 friday. All rights reserved.
//

#import "LFServiceSession.h"
#import "globaldef.h"
#import "OitServiceManage.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>


static LFServiceSession *mLFLiveSession = nil;

#define NOTIFICATIONNAME @"PAEBankPageForwardNotification"

@interface LFServiceSession()<AudioPlaySink,NetworkProxySendDelegate>
{
	DWORD mClock;
	
	//服务回调对象
	id<LFScreenAVKitCallBack> mAVKitCallBack;
	//网络对象

	COitServiceManage *pCOitServiceManage;
	
	IMediaDataSink    *mMediaSend;
    
    int                srcScreenWidth;
    int                srcScreenHeight;
    NSString          *whiteFilePath;
    NSMutableArray    *whiteList;
}
@end

@implementation LFServiceSession


-(id)init
{
	if (self = [super init]) {
		mClock = 0;
		pCOitServiceManage = new COitServiceManage();
        UIScreen *mainScreen = [UIScreen mainScreen];
        
        CGSize   mainScSize  = mainScreen.bounds.size;
        
        UIGraphicsBeginImageContext(mainScSize);
        CGContextRef context = UIGraphicsGetCurrentContext();
        size_t srcBytesMapPerrow = CGBitmapContextGetBytesPerRow(context);
        
        srcScreenWidth          = (int)srcBytesMapPerrow/4;
        srcScreenHeight         = (int)CGBitmapContextGetHeight(context);
        UIGraphicsEndImageContext();
        
        NSString *path = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES).firstObject;
        whiteFilePath = [path stringByAppendingPathComponent:@"whiteList.plist"];
        whiteList = [NSArray arrayWithContentsOfFile:whiteFilePath].mutableCopy;
        if (!whiteList) {
            whiteList = @[@"/xinyidai/pages/limitFinal.html", @"/xinyidai/pages/confirm.html", @"/xinyidai/pages/workInfo.html", @"/xinyidai/pages/familyInfo.html", @"/xinyidai/pages/linkInfo1.html"].mutableCopy;
            [whiteList writeToFile:whiteFilePath atomically:YES];
        }
        
	}
	return self;
}

- (void)receiveNotice:(NSNotification *)notice
{
    NSString *urlStr = [notice.object objectForKey:@"pageUrl"];
    if (urlStr.length == 0) {
        NSLog(@"通知消息pageUrl为空");
        return;
    }
    if ([whiteList containsObject:urlStr]) {
        
    }else{
        
        if (pCOitServiceManage->isStop == false) {
            [self showMessage:@"已停止远程辅助"];
            [[NSNotificationCenter defaultCenter] removeObserver:self name:NOTIFICATIONNAME object:nil];
            pCOitServiceManage->NetStatusNotify(SVS_STATUS_SC_1022);
        }
        
    }
}

- (void)showMessage:(NSString *)str
{
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"提示" message:str delegate:nil cancelButtonTitle:nil otherButtonTitles:@"确定", nil];
    UIWindow *window = [UIApplication sharedApplication].keyWindow;
    [window addSubview:alert];
    [alert show];
}

// 添加白名单
-(BOOL)addWhiteListWith:(NSString *)urlStr
{
    if (urlStr.length == 0) {
        NSLog(@"地址为空，添加白名单失败");
        return NO;
    }
    if ([whiteList containsObject:urlStr]) {
        NSLog(@"地址重复，添加白名单失败");
        return NO;
    }
    [whiteList addObject:urlStr];
    return [whiteList writeToFile:whiteFilePath atomically:YES];
    
}
-(BOOL)addWhiteListWithArray:(NSArray<NSString *> *)mArray
{
    if (mArray.count == 0) {
        return NO;
    }
    for (NSString *str in mArray) {
        if (![whiteList containsObject:str]) {
            [whiteList addObject:str];
        }
    }
    return [whiteList writeToFile:whiteFilePath atomically:YES];
}

// 删除白名单
-(BOOL)deleteWhiteListWith:(NSString *)urlStr
{
    if ([whiteList containsObject:urlStr]) {
        [whiteList removeObject:urlStr];
        return [whiteList writeToFile:whiteFilePath atomically:YES];
    }else{
        NSLog(@"白名单中不包含%@，删除失败", urlStr);
        return NO;
    }
    
}
-(BOOL)deleteWhiteListWithArray:(NSArray<NSString *> *)mArray
{
    if (mArray.count == 0) {
        return NO;
    }
    for (NSString *str in mArray) {
        [whiteList removeObject:str];
    }
    return [whiteList writeToFile:whiteFilePath atomically:YES];
}

// 打开白名单 (默认打开)
-(void)openWhiteList
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(receiveNotice:) name:NOTIFICATIONNAME object:nil];
}
// 关闭白名单
-(void)closeWhiteList
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:NOTIFICATIONNAME object:nil];
}


-(void)ResetWindow
{
	DWORD fisClock = GetTickCount() - mClock;
	if (mClock > 0 && fisClock <= 5000) {
		return;
	}
	
	pCOitServiceManage->ResetWindow();
	mClock = GetTickCount();
}

-(void)SetHasMkWebView:(BOOL)hasWKView
{
	pCOitServiceManage->SetHasWKWebView((bool)hasWKView);
}

-(int)GetScreenWidth
{
    return srcScreenWidth;
}
-(int)GetScreenHeight
{
    return srcScreenHeight;
}

-(void)SetBusinessType:(int)nType extVal:(const char *) extValue
{
    if(pCOitServiceManage!=NULL)
    {
        pCOitServiceManage->SetBusinessType(nType,extValue);
    }
}

//加密字符串
-(int)EncryptString:(const char *) pSrcStr encStr:(char *) pEncStr buffSize:(int) buffSize
{
    int nRetSize=0;
    if(pCOitServiceManage!=NULL)
    {
        nRetSize=pCOitServiceManage->MyEncryptString(pSrcStr,pEncStr,buffSize);
    }
    return nRetSize;
}

-(BOOL)InitAVKitWith:(char *)pSevice AndCallBack:(id<LFScreenAVKitCallBack>)lAVTransmKitCallBack
{
	mAVKitCallBack = lAVTransmKitCallBack;
	NSString *seviceStr = [NSString stringWithFormat:@"%s",pSevice];
	NSArray *array = [seviceStr componentsSeparatedByString:@":"];
	
	if (array.count > 0) {
		
		NSString *hoststr = [array objectAtIndex:0];
		
		if (![self isValidIP:hoststr]) {
			
			//是域名
			//域名解析，然后传入
			char *lstIPSever = [self GetIPFromDNS:seviceStr];
			if (lstIPSever == NULL) {
				return NO;
			}
			char dstSevice[68];
			strcpy(dstSevice, lstIPSever);
			BOOL isSuccess = pCOitServiceManage->InitAvManager(dstSevice,self,mAVKitCallBack);
			mMediaSend = pCOitServiceManage->GetMediaInterface();
			return isSuccess;
			
		}else{
			
			BOOL isSuccess = pCOitServiceManage->InitAvManager(pSevice,self,mAVKitCallBack);
			mMediaSend = pCOitServiceManage->GetMediaInterface();
			return isSuccess;
		}
	}else{
		if ([mAVKitCallBack respondsToSelector:@selector(SeviceStateChangeWithStatus:)]) {
			[mAVKitCallBack SeviceStateChangeWithStatus:SVS_STATUS_SC_1021];
		}
	}
	
	return NO;
	
}

-(BOOL)isValidIP:(NSString*)ip
{
	char *cIp = (char*)ip.UTF8String;
	int len = (int)strlen(cIp);
	for(int i = 0;i < len ; i++)
	{
		if ((cIp[i]>='a'&&cIp[i]<='z')||(cIp[i]>='A'&&cIp[i]<='Z')) {
			return NO;
		}
	}
	return YES;
	
}

-(char*)GetIPFromDNS:(NSString*)lpszServerDNS
{
	NSArray *array = [lpszServerDNS componentsSeparatedByString:@":"];
	
	if (array.count <= 1) {
		if ([mAVKitCallBack respondsToSelector:@selector(SeviceStateChangeWithStatus:)]) {
			[mAVKitCallBack SeviceStateChangeWithStatus:SVS_STATUS_SC_1020];
		}
		return NULL;
	}
	
	
	NSString *dnsStr = array[0];
	NSString *portStr= array[1];
	
	extern int h_errno;
	struct hostent *h;
	struct in_addr in;
	struct sockaddr_in addr_in;
	h=gethostbyname(dnsStr.UTF8String);
	if(h==NULL)
	{
		printf("%s\n",hstrerror(h_errno));
		if ([mAVKitCallBack respondsToSelector:@selector(SeviceStateChangeWithStatus:)]) {
			[mAVKitCallBack SeviceStateChangeWithStatus:SVS_STATUS_SC_1019];
		}
		return NULL;
	}else
	{
		memcpy(&addr_in.sin_addr.s_addr,h->h_addr,4);
		in.s_addr=addr_in.sin_addr.s_addr;
		
		printf("host name:%s\n",h->h_name);
		printf("ip lenght:%d\n",h->h_length);//IPv4 or IPv6
		printf("type:%d\n",h->h_addrtype);
		printf("%u\n",addr_in.sin_addr.s_addr );
		printf("ip:%s\n",inet_ntoa(in));
		NSString *lastIPSVR = [NSString stringWithFormat:@"%s:%@",inet_ntoa(in),portStr];
		return (char*)lastIPSVR.UTF8String;
	}
	
	return NULL;
}



-(void)CreateTextCopyer
{
	
}

-(void)CreateAudioRecoder
{
#if HAS_AUDIO
	_mAVManager = [[AVManager alloc]initWithSendAudioDelegate:self];
#endif
}

-(BOOL)StarSeviceWithServiceCode:(unsigned int)aSv AndClientCode:(unsigned int)aCli AndRecodeWidth:(int)Width AndRecodHeight:(int)Height
                     screenWidth:(int)scx screenHeight:(int) scy
{
	pCOitServiceManage->StartAVCapThread();
	if(pCOitServiceManage->StartService(aSv, aCli, Width, Height,scx,scy))
        return YES;
    return NO;
}
//手机号码，工号开启服务
-(BOOL)StarSeviceWithJobNumCode:(char *)aJobNum AndSVMobile:(char *)aMobile AndRecodeWidth:(int)Width AndRecodHeight:(int)Height screenWidth:(int)scx screenHeight:(int)scy
{
    
	if(pCOitServiceManage->StartService(aJobNum, aMobile, Width, Height, scx, scy))
	{
        
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(receiveNotice:) name:NOTIFICATIONNAME object:nil];
        
//		pCOitServiceManage->StartAVCapThread();
		return YES;
	}
	
	return NO;
}


-(BOOL)StopSevice
{

    if (pCOitServiceManage->isStop == true) {
        return NO;
    }
    
 dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
     
        pCOitServiceManage->StopService();
        
    });
    
	return YES;
}


// 视频采集器
-(void)BFVideoCollectOutWithFrameData:(void *)aVideoFrameBuffer WithDataLen:(unsigned int)dSize
{
}

-(void)BFVideoCollectOutWithSpsPPs:(void *)aSpsBuffer WithDataLen:(unsigned int)dSize
{
}

// 网络回调
-(void)OnAudioDataPlay:(const void *)pAudioData dataSize:(unsigned int)nDataSize
{
	[_mAVManager onReceiveAudioDataWithBytes:(unsigned char*)pAudioData length:nDataSize sequence:0];
	
}

-(BOOL)sendMediaAudioData:(BYTE* )pBuffer dSize:(unsigned short)wDataSize sequenceID:(unsigned short)sID callType:(int)type
{
	if (mMediaSend->OnAacAudioData((Byte*)pBuffer, wDataSize)) {
		return YES;
	}
	return NO;
}

-(BOOL) sendAudioDataWithBytes:(BYTE *)bytes length:(unsigned int)length sequenceID:(unsigned short)sequenceID
{
	
	return YES;
}

// 媒体控制
-(BOOL)StartAudioService
{
	if(_mAVManager != nil)
	{
		[_mAVManager startAudioQueue:0];	
	}
	
	return YES;
}

-(BOOL)StopAudioService
{
	if (_mAVManager != nil) {
		[_mAVManager stopAudioQueue];
	}
	return YES;
}

@end
