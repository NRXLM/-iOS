//
//  LFLiveSession.h
//  LFLiveSession
//
//  Created by friday on 2018/1/14.
//  Copyright © 2018年 friday. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LFScreenAVKit.h"
#import "AVManager.h"



@interface LFServiceSession : NSObject<LFScreenAVKit>


@property(nonatomic,retain)AVManager            *mAVManager;


-(void)CreateAudioRecoder;
-(void)CreateTextCopyer;


@end
