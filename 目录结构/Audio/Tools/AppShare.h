//
//  AppShare.h
//  RealTimeTransport
//
//  Created by tan on 13-5-20.
//  Copyright (c) 2013年 常 贤明. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>
#import "UIImageAdditions.h"

#define IMAGE_DATA                  @"image_data"
#define IMAGE_NAME                  @"image_name"
#define ETAG                        @"ETag"
#define LAST_MODIFIED               @"Last-Modified"

#define ETAG_REQUEST                @"If-None-Match"
#define LAST_MODIFIED_REQUEST       @"If-Modified-Since"
#define PUB_TMP_DIRECT              @"publish_direct"   //发布 相关文件缓存
#define CHAT_TMP_DIRECT             @"chat_tmp_direct"  //聊天 相关文件缓存
        
@interface AppShare : NSObject

+ (NSString *)documentsFolderPath;

+ (NSString *)cacheFolderPath;

+ (NSString *)imageCacheFolderPath;
+ (NSString *)imageCachePathWithName:(NSString *)imageURL;
+ (void)saveImageToCache:(NSDictionary *)imageInfo;
+ (UIImage *)cacheImage:(NSString *)imageURL;
+ (NSMutableArray *)refreshedImageArray;

+ (NSString *)audioCacheFolderPath;
+ (NSString *)audioCacheFolderPathForCustomer:(NSString *)fileName userId:(NSString *)uId;

+ (NSString* )recordLocalAudioFolderPath;

+ (NSString* )myPicCacheFilePath:(NSString* )pic;
+ (NSString* )myAudioCacheFilePath:(NSString* )audioURL;
+ (NSString* )myVideoCacheFilePath:(NSString* )videoURL;

+ (BOOL)fileExist:(NSString* )fileName;
+ (BOOL)fileExist:(NSString *)fileName andUserId:(NSString *)uId andDataName:(NSString *)dataName;
+ (NSString *)createFileName:(NSString *)fileName andUserId:(NSString *)uId andDataName:(NSString *)dataName;

+ (NSString *)dataBasePath;
+ (BOOL)createDataBaseWithPath:(NSString *)path;

+ (BOOL)addExtendedAttributeWithPath:(NSString *)path key:(NSString *)key value:(NSData *)value;
+ (NSString *)readExtendedAttributeWithPath:(NSString *)path key:(NSString *)key;
+ (NSMutableDictionary *)readAllExtendedAttributesWithPath:(NSString *)path;

+ (NSString *)macAddress;
+ (BOOL)isJailBreak;
+ (NSString *)currentAppVersion;

+ (NSString *)transformString:(NSString *)string;

+ (UIView *)horizontalSolidLineWithRect:(CGRect)frame;

+ (NSString *)pathByWriteData:(NSData *)data toFileName:(NSString *)fileName;

// 图片临时缓存
+ (NSString* )tmpCacheFilePath:(NSString* )name withDirect:(NSString* )dir;

@end