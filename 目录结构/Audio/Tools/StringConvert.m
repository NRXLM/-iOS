//
//  StringConvert.m
//  REducation
//
//  Created by changxm on 14-3-22.
//  Copyright (c) 2014年 changxm. All rights reserved.
//

#import "StringConvert.h"
#import "CommonCrypto/CommonDigest.h"

@implementation StringConvert

// 日期转为格式化字符串
+ (NSString* ) dateToLocalString:(NSDate* )nDate
{
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy-MM-dd"];
    return [dateFormatter stringFromDate:nDate];
}

// 时间戳转为字符串
+ (NSString* ) dateTimeToLocalString:(UNDWORD) udwTime
{
    NSDate *nDate = [NSDate dateWithTimeIntervalSince1970:udwTime];
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init] ;
    //[dateFormatter setDateFormat:@"HH:mm"];
    [dateFormatter setDateFormat:@"yyyy-MM-dd"];
    return [dateFormatter stringFromDate:nDate];
}

// utf8转为字符串
+ (NSString* ) convertUTF8toString:(BYTE* )pStrUTF8 strSize:(WORD)pLen
{
    if (pLen==0)
    {
        return [[NSString alloc] init] ;
    }
    NSString *str=[[NSString alloc] initWithBytes:pStrUTF8 length:pLen encoding:NSUTF8StringEncoding] ;
    return str;
}

// gbk转为字符串
+ (NSString* ) convertGBKtoString:(BYTE* )pStrGBK strSize:(WORD)pLen
{
    if (pLen == 0)
    {
        return nil;
    }
    
    NSStringEncoding enc = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingGB_18030_2000);
    NSString *str=[[NSString alloc] initWithBytes:pStrGBK length:pLen encoding:enc] ;
    return str;
}

// 字符串转为gbk
+ (const char* ) convertStringtoGBK:(NSString* )str
{
    if (str.length!=0)
    {
        NSStringEncoding enc = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingGB_18030_2000);
        const char *c = [str cStringUsingEncoding:enc];
        return c;
    }
    return nil;
}

// NSdate转换为string
+ (NSString* ) stringFromDate:(NSDate* )date
{
    NSDateFormatter* dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy－MM－dd"];
    NSString* strDate = [dateFormatter stringFromDate:date];
	
    
    return strDate;
}

// NSdate转换为string
+ (NSString* ) stringFromDate:(NSDate* )date withMode:(NSString* )mode
{
    if (date == nil)
    {
        return nil;
    }
    NSDateFormatter* dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:mode];
    NSString* strDate = [dateFormatter stringFromDate:date];
    
    return strDate;
}

+ (NSString* ) stringFromInterval:(long long)sec
{
    NSDate* date = [NSDate dateWithTimeIntervalSince1970:sec];
    NSDateFormatter* dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy-MM-dd"];
    NSString* strDate = [dateFormatter stringFromDate:date];
	
    
    return strDate;
}

+ (NSString* ) stringFromInterval:(long long)sec withMode:(NSString* )mode
{
    NSDate* date = [NSDate dateWithTimeIntervalSince1970:sec];
    NSDateFormatter* dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:mode];
    NSString* strDate = [dateFormatter stringFromDate:date];
	
    
    return strDate;
}

// 得到生日格式的字符串
+ (NSString* ) brithStringFromInterval:(long long)sec
{
    NSDate* date = [NSDate dateWithTimeIntervalSince1970:sec];
    NSDateFormatter* dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy-MM-dd"];
    NSString* strDate = [dateFormatter stringFromDate:date];
    
    NSString *strShort = [strDate substringFromIndex:5];
    return strShort;
}

+ (unsigned char* )MD5Chars:(NSString* )str
{
    const char *cStr = [str UTF8String];
    unsigned char result[CC_MD5_DIGEST_LENGTH];
    CC_MD5(cStr, strlen(cStr), result);
    
    unsigned char* pBuff = (unsigned char* )result;
    
    return pBuff;
}

+ (NSString* )MD5String:(NSString* )str
{
    const char *cStr = [str UTF8String];
    unsigned char result[16];
    CC_MD5(cStr, strlen(cStr), result); // This is the md5 call
    return [NSString stringWithFormat:
            @"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
            result[0], result[1], result[2], result[3],
            result[4], result[5], result[6], result[7],
            result[8], result[9], result[10], result[11],
            result[12], result[13], result[14], result[15]
            ];
}

+ (void)separateURLString:(NSString *)urlString ToPreString:(NSString **)preString pathString:(NSString **)pathString
{
    NSURL *url = [NSURL URLWithString:urlString];
    if ([urlString hasPrefix:@"http://192.168"]) {
        *pathString = [url path];
        NSRange range = [urlString rangeOfString:*pathString];
        *preString = [urlString substringToIndex:range.location];
    }
}

// 获取扩展名
+ (NSString* )extendName:(NSString* )path
{
    NSArray* array = [path componentsSeparatedByString:@"."];
    NSString* lastStr = nil;
    lastStr = [array lastObject];
    return lastStr;
}

// 获取距离现在的时长表达字符串
+ (NSString*) timeSpaceFromeNow:(long long) sec
{
    const NSTimeInterval secsOneDay = 24 * 60 * 60;
    const double zeroDoubleExp = 0.00000001;
    NSDate* dateNow = [NSDate date];
    NSTimeInterval secNow = [dateNow timeIntervalSince1970];
    NSTimeInterval secSpace = secNow - sec;                                         // 指定日期距离现在的时长
    NSTimeInterval timeSec = (NSTimeInterval)((int) secNow % (int)secsOneDay);      // 距离当天零点的时长
    
    if ((secSpace - timeSec) < zeroDoubleExp) // 判断是不是当天发布的
    {
        NSCalendar *cal = [NSCalendar currentCalendar];
        NSDateComponents* dcTheDate = [cal components:(NSHourCalendarUnit | NSMinuteCalendarUnit) fromDate:[NSDate dateWithTimeIntervalSince1970:sec]];
        
        int theHour = [dcTheDate hour];
        int theMinute = [dcTheDate minute];
        
        if (theMinute < 10)
        {
            return [NSString stringWithFormat:@"%d:0%d", theHour, theMinute];
        }
        else
        {
            return [NSString stringWithFormat:@"%d:%d", theHour, theMinute];
        }
    }
    else if ((secSpace - secsOneDay - timeSec) < zeroDoubleExp) // 判断是不是昨天发布的
    {
        return @"昨天";
    }
    else if ((secSpace - secsOneDay * 2 - timeSec) < zeroDoubleExp) // 判断是不是前天发布的
    {
        return @"前天";
    }
    else // 三天及以前的时间发布的
    {
        NSCalendar *cal = [NSCalendar currentCalendar];
        NSDateComponents* dcNow = [cal components:NSYearCalendarUnit fromDate:dateNow];
        NSDateComponents* dcTheDate = [cal components:NSDayCalendarUnit | NSMonthCalendarUnit | NSYearCalendarUnit fromDate:[NSDate dateWithTimeIntervalSince1970:sec]];
        
        int yearNow = [dcNow year];
        int theYear = [dcTheDate year];
        
        if (theYear == yearNow) // 判断是不是当年发布的
        {
            int theMonth = [dcTheDate month];
            int theDay = [dcTheDate day];
            return [NSString stringWithFormat:@"%d月%d日", theMonth, theDay];
        }
        else // 去年或去年以前发布的
        {
            return [NSString stringWithFormat:@"%d年", theYear];
        }
    }
    
    return nil;
}

@end
