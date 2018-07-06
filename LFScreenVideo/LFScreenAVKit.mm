//
//  LFScreenAVKit.c
//  LFScreenVideo
//
//  Created by friday on 2018/1/17.
//  Copyright © 2018年 friday. All rights reserved.
//

#import "LFScreenAVKit.h"
#import "LFServiceSession.h"

static id<LFScreenAVKit> mLFLiveSession= nil;


@implementation LFAVKit
+(id<LFScreenAVKit>)CreateLFLiveScreenKit:(int)lRegisterType
{
	if (mLFLiveSession == nil) {
		mLFLiveSession = [[LFServiceSession alloc]init];
    
		if (lRegisterType & RegisterTypeAudio) {
			[(LFServiceSession*)mLFLiveSession CreateAudioRecoder];
		}
		if (lRegisterType & RegisterTypeText) {
			[(LFServiceSession*)mLFLiveSession CreateTextCopyer];
		}
	}
	return mLFLiveSession;
}
@end

id<LFScreenAVKit> CreateLFLiveScreenKit(RegisterType lRegisterType)
{
	if (mLFLiveSession == nil) {
		mLFLiveSession = [[LFServiceSession alloc]init];
		if (lRegisterType & RegisterTypeText) {

		}
		if (lRegisterType & RegisterTypeAudio) {
			[(LFServiceSession*)mLFLiveSession CreateAudioRecoder];
		}
		if (lRegisterType & RegisterTypeText) {
			[(LFServiceSession*)mLFLiveSession CreateTextCopyer];
		}
	}
	return mLFLiveSession;
}
