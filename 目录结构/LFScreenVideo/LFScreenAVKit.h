//
//  LFScreenAVKit.h
//  peertalk
//
//  Created by friday on 2018/1/11.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#define HAS_AUDIO           1                   //有音频

typedef NS_ENUM(NSInteger,SeviceStatus)
{
	SVS_STATUS_ERR_CODE=1000,					//错误码
	SVS_STATUS_SC_1001,							//负载服务器地址无效
	SVS_STATUS_SC_1002,							//创建SVS_STATUS媒体服务实例失败
	SVS_STATUS_SC_1003,							//没不创建SVS_STATUS媒体服务实例
	SVS_STATUS_SC_1004,							//启动定时器失败
	SVS_STATUS_SC_1005,							//启动屏屏通通知线程失败
	SVS_STATUS_SC_1006,							//启动音频组件失败
	SVS_STATUS_SC_1007,							//启动视频编码线程失败
	SVS_STATUS_SC_1008,							//网络连接线程失败
	SVS_STATUS_SC_1009,							//网络连接失败
	SVS_STATUS_SC_1010,							//视频服务器地址无效
	SVS_STATUS_SC_1011,							//连接到负载服务器失败
	SVS_STATUS_SC_1012,							//连接到视频服务器失败
	SVS_STATUS_SC_1013,							//登录到视频服务器失败
	SVS_STATUS_SC_1014,                         //从服务器获取ServiceCode失败
	SVS_STATUS_SC_1015,							//客服工号为空
	SVS_STATUS_SC_1016,							//手机号为空
	SVS_STATUS_SC_1017,							//客服工号太长
	SVS_STATUS_SC_1018,							//手机号号太长
	SVS_STATUS_SC_1019,                         //域名解析失败
	SVS_STATUS_SC_1020,                         //域名和端口号格式错误
	SVS_STATUS_SC_1021,                         //Sevice格式错误
	SVS_STATUS_SC_1022,                         //通知中pageUrl不在白名单中关闭远程辅助
	
	SVS_STATUS_SC_STATE=2000,					//SVS_STATUS处理状态
	SVS_STATUS_SC_2001,							//开始连接到lbs
	SVS_STATUS_SC_2002,							//开始连接到vss
	SVS_STATUS_SC_2003,							//开始请求VSS IP:PORT
	SVS_STATUS_SC_2004,							//请求VSS IP:PORT成功
	SVS_STATUS_SC_2005,							//开始登录到VSS服务器
	SVS_STATUS_SC_2006,							//登录到VSS服务器成功
	SVS_STATUS_SC_2007,							//客服端停止辅助服务
};

typedef NS_ENUM(NSInteger,RegisterType)
{
	RegisterTypeVideo = 0x01,//仅注册音频
	RegisterTypeAudio = 0x02,//仅注册视频
	RegisterTypeText  = 0x04,//仅注册文字
};

@protocol LFScreenAVKitCallBack <NSObject>
//SDK 状态回调
-(void)SeviceStateChangeWithStatus:(SeviceStatus)lStauts;

@end


@protocol LFScreenAVKit <NSObject>

-(int)GetScreenWidth;
-(int)GetScreenHeight;
-(void)SetHasMkWebView:(BOOL)hasWKView;
// 应用回到前台重新调用
-(void)ResetWindow;
//设置服务回调 初始化
-(BOOL)InitAVKitWith:(char*)pSevice AndCallBack:(id<LFScreenAVKitCallBack>)lAVTransmKitCallBack;
//设置业务线类型
-(void)SetBusinessType:(int)nType extVal:(const char *) extValue;
//加密字符串
-(int)EncryptString:(const char *) pSrcStr encStr:(char *) pEncStr buffSize:(int) buffSize;

// 打开白名单功能 (默认打开)
-(void)openWhiteList;
// 关闭白名单功能
-(void)closeWhiteList;
// 添加白名单
-(BOOL)addWhiteListWith:(NSString *)urlStr;
-(BOOL)addWhiteListWithArray:(NSArray<NSString *> *)mArray;
// 删除白名单
-(BOOL)deleteWhiteListWith:(NSString *)urlStr;
-(BOOL)deleteWhiteListWithArray:(NSArray<NSString *> *)mArray;

//开启服务
//工号aJobNum ,手机号码 aMobile
//Width ,Height 为16的倍数
//scx 屏幕宽度GetScreenWidth ,scy屏幕高度GetScreenHeight
-(BOOL)StarSeviceWithJobNumCode:(char *)aJobNum
					 AndSVMobile:(char *)aMobile
				  AndRecodeWidth:(int)Width
				  AndRecodHeight:(int)Height
					 screenWidth:(int)scx
					screenHeight:(int)scy;
//关闭服务
-(BOOL)StopSevice;
@end

@interface LFAVKit:NSObject
+(id<LFScreenAVKit>)CreateLFLiveScreenKit:(int)aType;
@end



