//
//  AVManager.h
//  AVManager
//
//  Created by changxm on 13-11-6.
//  Copyright (c) 2013年 changxm. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IMDelegate.h"



typedef enum
{
    AVStatus_None,
    AVStatus_Run,
    AVStatus_Pause,
    AVStatus_Stop
}AVStatus;

@interface AVManager : NSObject<AVDelegate,AudioDataDelegate,AVManagerDelegate>

@property (nonatomic,assign,readonly) AVStatus avStatus;

- (id)initWithSendAudioDelegate:(id<NetworkProxySendDelegate>)audioDelegate;

// 录音状态
- (void) localRecorderAudioStatus:(BOOL)status;

// 对方接听状态
- (void) setRemoteListenStatus:(int)status;

@end
