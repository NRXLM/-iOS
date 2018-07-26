//
//  ImageCacheButton.h
//  MessagesDemo
//
//  Created by tan on 13-7-31.
//  Copyright (c) 2013年 Hexed Bits. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AppShare.h"

@interface ImageCacheButton : UIButton <NSURLConnectionDelegate>
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
}

- (id)initWithFrame:(CGRect)frame defaultImage:(UIImage *)image showLoadingIndicator:(BOOL)showLoading resizeImage:(BOOL)resize refreshImage:(BOOL)refresh;

- (void)setWithDefaultImage:(UIImage *)image showLoadingIndicator:(BOOL)showLoading resizeImage:(BOOL)resize refreshImage:(BOOL)refresh;

- (void)loadWithURL:(NSString *)urlString;
- (void)cancelLoad;

@end
