//
//  ImageCacheView.h
//  RealTimeTransport
//
//  Created by tan on 13-5-20.
//  Copyright (c) 2013年 常 贤明. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AppShare.h"
#import "UIImageAdditions.h"

@class ImageCacheView;

@protocol ImageCacheViewDelegate

@optional
- (void)imageCacheViewTouchDown:(ImageCacheView *)imageCacheView;
- (void)imageCacheViewTouchUpInside:(ImageCacheView *)imageCacheView;

@end

@interface ImageCacheView : UIImageView <NSURLConnectionDelegate>
{
    BOOL showLoadingIndicator;
    BOOL resizeImage;
    BOOL refreshImage;
    
    UIImage *defaultImage;
    
    __block UIActivityIndicatorView *loadingIndicator;
    
    __block NSURLConnection *connection;
    NSInteger statusCode;
    
    __block NSString *urlStr;
    __block NSMutableData *receiveData;
    
    __block NSMutableDictionary *extendedAttributes;
    
    NSObject <ImageCacheViewDelegate>* delegate;
}

@property(nonatomic, retain) UIImage *defaultImage;

- (id)initWithFrame:(CGRect)frame defaultImage:(UIImage *)image showLoadingIndicator:(BOOL)showLoading resizeImage:(BOOL)resize refreshImage:(BOOL)refresh delegate:(NSObject <ImageCacheViewDelegate>*)del;

- (void)setWithDefaultImage:(UIImage *)image showLoadingIndicator:(BOOL)showLoading resizeImage:(BOOL)resize refreshImage:(BOOL)refresh delegate:(NSObject <ImageCacheViewDelegate>*)del;
- (void)SetLastDefaultImage:(UIImage* )image;
- (void)loadWithURL:(NSString *)urlString;
- (void)cancelLoad;

@end
