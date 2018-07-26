//
//  ImageCacheButton.m
//  MessagesDemo
//
//  Created by tan on 13-7-31.
//  Copyright (c) 2013年 Hexed Bits. All rights reserved.
//

#import "ImageCacheButton.h"

@implementation ImageCacheButton

- (id)initWithFrame:(CGRect)frame defaultImage:(UIImage *)image showLoadingIndicator:(BOOL)showLoading resizeImage:(BOOL)resize refreshImage:(BOOL)refresh
{
    self = [super initWithFrame:frame];
    if (self) {
        
        [self setWithDefaultImage:image showLoadingIndicator:showLoading resizeImage:resize refreshImage:refresh];
        
    }
    return self;
}

- (void)setWithDefaultImage:(UIImage *)image showLoadingIndicator:(BOOL)showLoading resizeImage:(BOOL)resize refreshImage:(BOOL)refresh
{
    defaultImage=[image retain];
    
    [self setBackgroundImage:image forState:UIControlStateNormal];
    
    showLoadingIndicator=showLoading;
    
    resizeImage=resize;
    
    refreshImage=refresh;
    
    receiveData=nil;
    
    urlStr=nil;
    
    extendedAttributes=nil;
    
    if (showLoadingIndicator) {
        loadingIndicator = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleGray];
        loadingIndicator.center = CGPointMake(self.bounds.size.width/2, self.bounds.size.height/2);
        [self addSubview:loadingIndicator];
        [loadingIndicator release];
    }
}

- (void)startLoading
{
    [self bringSubviewToFront:loadingIndicator];
    [loadingIndicator startAnimating];
}

- (void)stopLoading
{
    [self sendSubviewToBack:loadingIndicator];
    [loadingIndicator stopAnimating];
}

-(void)loadWithURL:(NSString *)urlString
{
   // NSString* strHeadUrl = [[NSBundle mainBundle] pathForResource:@"default_head" ofType:@"png"];
    if (!urlString || ![urlString isKindOfClass:[NSString class]] || [urlString isEqualToString:@""] )
    {
        [self setBackgroundImage:defaultImage forState:UIControlStateNormal];
         return ;
    }
    
    UIImage *image = nil;
    // 如果不是网络路径，从本地取图片
    if (![urlString hasPrefix:@"http://"])
    {
        image = [UIImage imageWithContentsOfFile:urlString];
        if (!image)
        {
            return;
        }
        if (resizeImage)
            image = [image resize:self.frame.size];
        [self setBackgroundImage:image forState:UIControlStateNormal];
        return;
    }
    
    image = [AppShare cacheImage:urlString];
    if (image) {
        if (resizeImage)
            image = [image resize:self.frame.size];
        [self setBackgroundImage:image forState:UIControlStateNormal];
        if (refreshImage) {
            if ([[AppShare refreshedImageArray] containsObject:urlString])
                return ;
            
            SAFE_RELEASE(extendedAttributes);
            extendedAttributes=[[AppShare readAllExtendedAttributesWithPath:[AppShare imageCachePathWithName:urlString]] retain];
        }
        else
            return ;
    }
    else
    {
        if (showLoadingIndicator) {
            [self startLoading];
        }
    }
    
    [self cancelLoad];
    
    SAFE_RELEASE(receiveData);
    receiveData = [[NSMutableData alloc] init];
    
    SAFE_RELEASE(urlStr);
    urlStr=[urlString retain];
    
    statusCode = 0;
    
    __block ImageCacheButton *imageCacheButton=self;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        NSMutableURLRequest *request = [NSMutableURLRequest.alloc initWithURL:[NSURL URLWithString:urlStr] cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:10];
        request.HTTPShouldHandleCookies = NO;
        request.HTTPShouldUsePipelining = YES;
        
        NSString *etagValue=[extendedAttributes objectForKey:ETAG];
        NSString *lastModifiedValue=[extendedAttributes objectForKey:LAST_MODIFIED];
        
        if (etagValue) {
            [request addValue:[extendedAttributes objectForKey:ETAG] forHTTPHeaderField:ETAG_REQUEST];
        }
        
        if (lastModifiedValue) {
            [request addValue:[extendedAttributes objectForKey:LAST_MODIFIED] forHTTPHeaderField:LAST_MODIFIED_REQUEST];
        }
        
        connection=[[NSURLConnection alloc] initWithRequest:request delegate:imageCacheButton];
        [connection start];
        
        [request release];
        
        CFRunLoopRun();
    });
}

- (void)cancelLoad
{
    if (connection) {
        [connection cancel];
        SAFE_RELEASE(connection);
    }
}

#pragma mark - NSURLConnectionDelegate
- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
    statusCode=[(NSHTTPURLResponse *)response statusCode];
    
    if (!extendedAttributes) {
        extendedAttributes=[NSMutableDictionary new];
    }
    else
    {
        [extendedAttributes removeAllObjects];
    }
    
    [extendedAttributes setValue:[[(NSHTTPURLResponse *)response allHeaderFields] objectForKey:ETAG] forKey:ETAG];
    [extendedAttributes setValue:[[(NSHTTPURLResponse *)response allHeaderFields] objectForKey:LAST_MODIFIED] forKey:LAST_MODIFIED];
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    [receiveData appendData:data];
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
    dispatch_async(dispatch_get_main_queue(), ^{
        if (showLoadingIndicator) {
            [self stopLoading];
        }
        
        if (receiveData && statusCode == 200) {
            UIImage *originalImage = [[UIImage alloc] initWithData:receiveData];
            
            UIImage *processedImage = originalImage;
            if (resizeImage)
                processedImage = [processedImage resize:self.frame.size];
            
            [self setBackgroundImage:processedImage forState:UIControlStateNormal];
            
            NSString *etagValue=[extendedAttributes objectForKey:ETAG];
            NSString *lastModifiedValue=[extendedAttributes objectForKey:LAST_MODIFIED];
            [AppShare saveImageToCache:[NSDictionary dictionaryWithObjectsAndKeys:receiveData,IMAGE_DATA,urlStr,IMAGE_NAME,etagValue?etagValue:[NSNull null],ETAG,lastModifiedValue?lastModifiedValue:[NSNull null],LAST_MODIFIED, nil]];
            
            SAFE_RELEASE(originalImage);
        }

        if (urlStr)
        {
            if (![[AppShare refreshedImageArray] containsObject:urlStr])
                [[AppShare refreshedImageArray] addObject:urlStr];
        }
        
        [self cancelLoad];
        
        SAFE_RELEASE(receiveData);
        SAFE_RELEASE(urlStr);
        SAFE_RELEASE(extendedAttributes);
    });
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
    dispatch_async(dispatch_get_main_queue(), ^{
        if (showLoadingIndicator) {
            [self stopLoading];
        }
        
        UILabel *label = [[UILabel alloc] initWithFrame:self.bounds];
        label.backgroundColor = [UIColor clearColor];
        label.textAlignment = NSTextAlignmentCenter;
        label.font = [UIFont systemFontOfSize:13];
        label.textColor = [UIColor whiteColor];
        label.adjustsFontSizeToFitWidth=YES;
        /////****** leader need hidden label *****/////
        //label.text = @"图片加载失败";
        label.text = @"";
        [self addSubview:label];
        [label release];
        
        [self cancelLoad];
        
        SAFE_RELEASE(receiveData);
        SAFE_RELEASE(urlStr);
        SAFE_RELEASE(extendedAttributes);
    });
}

-(void)dealloc
{
    [self cancelLoad];
    
    SAFE_RELEASE(receiveData);
    SAFE_RELEASE(urlStr);
    SAFE_RELEASE(extendedAttributes);
    SAFE_RELEASE(defaultImage);
    
    [super dealloc];
}

@end
