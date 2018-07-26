//
//  RegExTool.h
//  REducation
//
//  Created by changxm on 14-3-22.
//  Copyright (c) 2014年 changxm. All rights reserved.
//
#import <Foundation/Foundation.h>

@interface RegExTool : NSObject

// 手机号码验证
+ (BOOL) isMobileNumber:(NSString* )mobileNum;

// 邮箱验证
+ (BOOL) isValidateEmail:(NSString* )mailStr;

// 电话号码验证
+ (BOOL) isValidateTelNumber:(NSString* )number;

// 数字验证
+ (BOOL) isValidateNumber:(NSString* )number;

// 是否带连字符的11位数
+ (BOOL) isElevenNumber:(NSString *)number;

@end
