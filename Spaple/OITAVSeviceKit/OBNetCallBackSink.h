//
//  OBNetCallBackSink.h
//  vh264enc
//
//  Created by friday on 2018/1/17.
//  Copyright © 2018年 OIT. All rights reserved.
//

#import <Foundation/Foundation.h>



@protocol AudioPlaySink <NSObject>

-(BOOL)StartAudioService;
-(BOOL)StopAudioService;
-(void) OnAudioDataPlay:(const void *) pAudioData dataSize:(unsigned int) nDataSize;

@end
