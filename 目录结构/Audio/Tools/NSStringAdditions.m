//
//  NSStringAdditions.m
//  RealTimeTransport
//
//  Created by tan on 13-5-20.
//  Copyright (c) 2013年 常 贤明. All rights reserved.
//

#import "NSStringAdditions.h"

@implementation NSString (NSStringAdditions)

//example: NSLog(@"%@",[@"abcd432e<a>fghi</a>jklmnop4324qabr5654<a />6stu<b />vwxyz" traverseUsingRegularExp:@"<(.*)>.*<\\/\\1>|<[^/]*/>"]);
- (NSArray *)traverseUsingRegularExp:(NSString *)reg
{
	NSMutableArray *result=[NSMutableArray array];
	NSRange range=[self rangeOfString:reg options:NSRegularExpressionSearch range:NSMakeRange(0, [self length])];
    while (range.length>0)
	{
		[result addObject:[NSValue valueWithRange:range]];
		range=NSMakeRange(range.location+range.length, [self length]-(range.location+range.length));
		range=[self rangeOfString:reg options:NSRegularExpressionSearch range:range];
	}
	return result;
}

- (BOOL)matchRegularExp:(NSString *)reg
{
    return [self rangeOfString:reg options:NSRegularExpressionSearch range:NSMakeRange(0, [self length])].length>0;
}

- (NSString *)stringFromMD5{
    
    if(self == nil || [self length] == 0)
        return nil;
    
    const char *value = [self UTF8String];
    
    unsigned char outputBuffer[CC_MD5_DIGEST_LENGTH];
    CC_MD5(value, strlen(value), outputBuffer);
    
    NSMutableString *outputString = [[NSMutableString alloc] initWithCapacity:CC_MD5_DIGEST_LENGTH * 2];
    for(NSInteger count = 0; count < CC_MD5_DIGEST_LENGTH; count++){
        [outputString appendFormat:@"%02x",outputBuffer[count]];
    }
    
    return outputString ;
}

- (NSString *)encodeForURL
{
    const CFStringRef legalURLCharactersToBeEscaped = CFSTR("!*'();:@&=+$,/?#[]<>\"{}|\\`^% ");
    
    return NSMakeCollectable(CFURLCreateStringByAddingPercentEscapes(kCFAllocatorDefault, (CFStringRef)self, NULL, legalURLCharactersToBeEscaped, kCFStringEncodingUTF8)) ;
}

- (NSString *)encodeForURLReplacingSpacesWithPlus;
{
    const CFStringRef legalURLCharactersToBeEscaped = CFSTR("!*'();:@&=$,/?#[]<>\"{}|\\`^% ");
    
    NSString *replaced = [self stringByReplacingOccurrencesOfString:@" " withString:@"+"];
    return [NSMakeCollectable(CFURLCreateStringByAddingPercentEscapes(kCFAllocatorDefault, (CFStringRef)replaced, NULL, legalURLCharactersToBeEscaped, kCFStringEncodingUTF8)) autorelease];
}

- (NSString *)decodeFromURL
{
    NSString *decoded = [NSMakeCollectable(CFURLCreateStringByReplacingPercentEscapesUsingEncoding(kCFAllocatorDefault, (CFStringRef)self, CFSTR(""), kCFStringEncodingUTF8)) autorelease];
    return [decoded stringByReplacingOccurrencesOfString:@"+" withString:@" "];
}

- (NSDate *)timeStampDate
{
    NSDateFormatter *dateFormatter=[[[NSDateFormatter alloc] init] autorelease];
    dateFormatter.dateFormat=@"yyyy-MM-dd HH:mm:ss.SSS";
    return [dateFormatter dateFromString:self];
}

@end
