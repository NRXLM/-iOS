//
//  ImageCacheView.m
//  RealTimeTransport
//
//  Created by tan on 13-5-20.
//  Copyright (c) 2013年 常 贤明. All rights reserved.
//

#import "ImageCacheView.h"

@implementation ImageCacheView

@synthesize defaultImage;

- (id)initWithFrame:(CGRect)frame defaultImage:(UIImage *)image showLoadingIndicator:(BOOL)showLoading resizeImage:(BOOL)resize refreshImage:(BOOL)refresh delegate:(NSObject <ImageCacheViewDelegate>*)del
{
    self = [super initWithFrame:frame];
    if (self) {
        
        [self setWithDefaultImage:image showLoadingIndicator:showLoading resizeImage:resize refreshImage:refresh delegate:del];
        
    }
    return self;
}

- (void)setWithDefaultImage:(UIImage *)image showLoadingIndicator:(BOOL)showLoading resizeImage:(BOOL)resize refreshImage:(BOOL)refresh delegate:(NSObject <ImageCacheViewDelegate>*)del
{
    self.defaultImage = image;
    
    self.image=defaultImage;
    
    delegate=del;
    
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
    
    if (delegate) {
        self.userInteractionEnabled=YES;
    }
}

- (void)SetLastDefaultImage:(UIImage* )image
{
    self.defaultImage = image;
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
    if (!urlString || ![urlString isKindOfClass:[NSString class]] || [urlString isEqualToString:@""] || [urlString isEqual:[NSNull null]])
    {
        self.image=defaultImage;
        return ;
    }
    
    UIImage *image = [AppShare cacheImage:urlString];
    if (image) {
        if (resizeImage)
            image = [image resize:self.frame.size];
        self.image=image;
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
    
    __block ImageCacheView *imageCacheView=self;
    
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
        
        connection=[[NSURLConnection alloc] initWithRequest:request delegate:imageCacheView];
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
            
            self.image=processedImage;
            
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
        label.text = @"";
        [self addSubview:label];
        [label release];
        
        [self cancelLoad];
        
        SAFE_RELEASE(receiveData);
        SAFE_RELEASE(urlStr);
        SAFE_RELEASE(extendedAttributes);
    });
}

#pragma mark - Touches Events
-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (self.userInteractionEnabled) {
        if ([delegate respondsToSelector:@selector(imageCacheViewTouchDown:)])
            [delegate imageCacheViewTouchDown:self];
    }
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (self.userInteractionEnabled) {
        if ([delegate respondsToSelector:@selector(imageCacheViewTouchUpInside:)])
            [delegate imageCacheViewTouchUpInside:self];
    }
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
