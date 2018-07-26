//
//  UITool.h
//  REducation
//
//  Created by changxm on 14-3-22.
//  Copyright (c) 2014年 changxm. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface UITool : NSObject

// 消息提示框，用系统默认提示框
+ (void) showSampleMsg:(NSString *)title message:(NSString *) strMsg;

// 不需要委托alert
+ (void) alertNotifyError:(NSString* )title contentStr:(NSString* )content thread:(BOOL)isOnMainThread;

// 只要一个确定按钮
+ (void) alertNotify:(NSString* )title contentStr:(NSString* )content thread:(BOOL)isOnMainThread;

// 有确认，取消两个按钮的Alert
+ (void) alertOkAndCancel:(NSString* )title contentStr:(NSString* )content delegate:(id)target thread:(BOOL)isOnMainThread;

// 有确认，取消两个按钮的Alert 带tag
+ (void) alertOkAndCancel:(NSString* )title contentStr:(NSString* )content delegate:(id)target thread:(BOOL)isOnMainThread withTag:(int)tag;

// 不需要回调时target设为nil即可
+ (void) showMessage:(NSString* )msg titleStr:(NSString* )title delegate:(id)target;

// 设置导航条
+ (void)customizeNavBar:(UIViewController*)targetView withTitle:(NSString *)title
            haveLButton:(BOOL)bHaveL haveRButton:(BOOL)bHaveR
       normalImageNameL:(NSString*)nmlImgNameL highlightedImageNameL:(NSString*)hltImgNameL buttonNameL:(NSString*)btnNameL actionL:(SEL)actionL
       normanImageNameR:(NSString*)nmlImgNameR highlightedImageNameR:(NSString*)hltImgNameR buttonNameR:(NSString*)btnNameR actionR:(SEL)actionR;

// 设置导航条, 左边为凌形按钮，右边为长方形按钮
+ (void)customizeNavBar:(UIViewController*)targetView withTitle:(NSString *)title buttonNameL:(NSString*)btnNameL buttonNameR:(NSString*)btnNameR actionL:(SEL)actionL actionR:(SEL)actionR;

@end
