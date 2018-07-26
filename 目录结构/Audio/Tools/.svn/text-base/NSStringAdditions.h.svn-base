//
//  NSStringAdditions.h
//  RealTimeTransport
//
//  Created by tan on 13-5-20.
//  Copyright (c) 2013年 常 贤明. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CommonCrypto/CommonDigest.h>

@interface NSString (NSStringAdditions)

- (NSArray *)traverseUsingRegularExp:(NSString *)reg;
- (BOOL)matchRegularExp:(NSString *)reg;

- (NSString *)stringFromMD5;

- (NSString *)encodeForURL;
- (NSString *)encodeForURLReplacingSpacesWithPlus;
- (NSString *)decodeFromURL;

- (NSDate *)timeStampDate;

@end
