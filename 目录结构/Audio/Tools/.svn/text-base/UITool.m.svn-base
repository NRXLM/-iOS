//
//  UITool.m
//  REducation
//
//  Created by changxm on 14-3-22.
//  Copyright (c) 2014年 changxm. All rights reserved.
//

#import "UITool.h"

@implementation UITool

+ (void) showSampleMsg:(NSString *)title message:(NSString *) strMsg
{
    UIAlertView* alert = [[UIAlertView alloc] initWithTitle:title
                                                    message:strMsg
                                                   delegate:nil
                                          cancelButtonTitle:@"确定"
                                          otherButtonTitles:nil];
    [alert show];
    [alert release];
}

// 不需要委托alert
+ (void) alertNotifyError:(NSString* )title contentStr:(NSString* )content thread:(BOOL)isOnMainThread
{
    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:title
                                                        message:content
                                                       delegate:nil
                                              cancelButtonTitle:@"取消"
                                              otherButtonTitles:@"确定", nil];
    if (isOnMainThread)
    {
        [alertView show];
    }
    else
    {
        [alertView performSelectorOnMainThread:@selector(show) withObject:nil waitUntilDone:NO];
    }
    [alertView release];
}

// 只要一个确定按钮
+ (void) alertNotify:(NSString* )title contentStr:(NSString* )content thread:(BOOL)isOnMainThread
{
    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:title message:content delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
    
    if (isOnMainThread)
    {
        [alertView show];
    }
    else
    {
        [alertView performSelectorOnMainThread:@selector(show) withObject:nil waitUntilDone:NO];
    }
    [alertView release];
}

// 有确认，取消两个按钮的Alert
+ (void) alertOkAndCancel:(NSString* )title contentStr:(NSString* )content delegate:(id)target thread:(BOOL)isOnMainThread
{
    UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:title
                                                        message:content
                                                       delegate:target
                                              cancelButtonTitle:@"取消"
                                              otherButtonTitles:@"确认",nil];
    if (isOnMainThread)
    {
        [alertView show];
    }
    else
    {
        [alertView performSelectorOnMainThread:@selector(show) withObject:nil waitUntilDone:NO];
    }
    [alertView release];
}

+ (void) alertOkAndCancel:(NSString* )title contentStr:(NSString* )content delegate:(id)target thread:(BOOL)isOnMainThread withTag:(int)tag
{
    UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:title
                                                        message:content
                                                       delegate:target
                                              cancelButtonTitle:@"取消"
                                              otherButtonTitles:@"确认",nil];
    alertView.tag = tag;
    if (isOnMainThread)
    {
        [alertView show];
    }
    else
    {
        [alertView performSelectorOnMainThread:@selector(show) withObject:nil waitUntilDone:NO];
    }
    [alertView release];
}

// 不需要回调时target设为nil即可
+ (void) showMessage:(NSString* )msg titleStr:(NSString* )title delegate:(id)target
{
    UIAlertView * alert = [[[UIAlertView alloc] initWithTitle:title message:msg delegate:target cancelButtonTitle:nil otherButtonTitles:@"确定",nil] autorelease];
    [alert show];
}

#pragma mark - 自定义 导航条 按钮
+ (void)customizeNavBar:(UIViewController*)targetView withTitle:(NSString *)title
            haveLButton:(BOOL)bHaveL haveRButton:(BOOL)bHaveR
       normalImageNameL:(NSString*)nmlImgNameL highlightedImageNameL:(NSString*)hltImgNameL buttonNameL:(NSString*)btnNameL actionL:(SEL)actionL
       normanImageNameR:(NSString*)nmlImgNameR highlightedImageNameR:(NSString*)hltImgNameR buttonNameR:(NSString*)btnNameR actionR:(SEL)actionR
{
    // 有左边的按钮
    if (bHaveL)
    {
        // 设置左边按钮
        UIImage *normalImage = [UIImage imageNamed:nmlImgNameL];
        UIImage *scaledImage=[UIImage imageWithCGImage:normalImage.CGImage scale:2 orientation:UIImageOrientationUp];
        normalImage=[scaledImage stretchableImageWithLeftCapWidth:scaledImage.size.width/2 topCapHeight:scaledImage.size.height/2];
        
        UIImage* downImage = [UIImage imageNamed:hltImgNameL];
        UIImage* downScaleImage = [UIImage imageWithCGImage:downImage.CGImage scale:2 orientation:UIImageOrientationUp];
        downImage = [downScaleImage stretchableImageWithLeftCapWidth:scaledImage.size.width/2 topCapHeight:scaledImage.size.height/2];
        
        UIButton* leftButton = [[UIButton alloc] initWithFrame:CGRectMake(0, 0, normalImage.size.width-2, normalImage.size.height)];
        [leftButton setTitle:btnNameL forState:UIControlStateNormal];
        leftButton.titleLabel.font = [UIFont systemFontOfSize:BTN_TITLE_SIZE];
        [leftButton setTitleColor:[UIColor darkGrayColor] forState:UIControlStateNormal];
        leftButton.titleEdgeInsets=UIEdgeInsetsMake(0, 7, 0, 0);
        [leftButton setBackgroundImage:normalImage forState:UIControlStateNormal];
        [leftButton setBackgroundImage:downScaleImage forState:UIControlStateHighlighted];
        [leftButton addTarget:targetView action:actionL/*@selector(leftBtnMethod)*/ forControlEvents:UIControlEventTouchUpInside];
        UIBarButtonItem* leftBarItem = [[UIBarButtonItem alloc] initWithCustomView:leftButton];
        targetView.navigationItem.leftBarButtonItem = leftBarItem;
        [leftBarItem release];
        [leftButton release];
    }
    
    // 有右边的按钮
    if (bHaveR)
    {
        // 设置右边按钮
        UIImage* normalImage = [UIImage imageNamed:nmlImgNameR];
        UIImage* scaledImage=[UIImage imageWithCGImage:normalImage.CGImage scale:2 orientation:UIImageOrientationUp];
        normalImage=[scaledImage stretchableImageWithLeftCapWidth:scaledImage.size.width/2 topCapHeight:scaledImage.size.height/2];
        
        UIImage* downImage = [UIImage imageNamed:hltImgNameR];
        UIImage* downScaleImage = [UIImage imageWithCGImage:downImage.CGImage scale:2 orientation:UIImageOrientationUp];
        downImage = [downScaleImage stretchableImageWithLeftCapWidth:scaledImage.size.width/2 topCapHeight:scaledImage.size.height/2];
        
        UIButton *rightBtn = [UIButton buttonWithType:UIButtonTypeCustom];
        rightBtn.frame = CGRectMake(0, 0, normalImage.size.width-2, normalImage.size.height);
        [rightBtn setBackgroundImage:normalImage forState:UIControlStateNormal];
        [rightBtn setBackgroundImage:downScaleImage forState:UIControlStateHighlighted];
        [rightBtn setTitle:btnNameR forState:UIControlStateNormal];
        [rightBtn setTitleColor:[UIColor darkGrayColor] forState:UIControlStateNormal];
        rightBtn.titleLabel.font = [UIFont systemFontOfSize:BTN_TITLE_SIZE];
        rightBtn.titleEdgeInsets=UIEdgeInsetsMake(0, 4, 0, 0);
        [rightBtn addTarget:targetView action:actionR/*@selector(rightBtnMethod)*/ forControlEvents:UIControlEventTouchUpInside];
        
        UIBarButtonItem *rightBarButtonItem=[[UIBarButtonItem alloc] initWithCustomView:rightBtn];
        targetView.navigationItem.rightBarButtonItem=rightBarButtonItem;
        [rightBarButtonItem release];
        [rightBtn release];
    }
    
    // 设置导航条标题
    UILabel *titleLabel = [[[UILabel alloc] initWithFrame:CGRectMake(0, 0, 220, 44)] autorelease];
    titleLabel.backgroundColor = [UIColor clearColor];
    titleLabel.textColor = [UIColor darkGrayColor];
    titleLabel.textAlignment = NSTextAlignmentCenter;
    titleLabel.text = title;
    targetView.navigationItem.titleView = titleLabel;
}

#pragma mark - 设置导航条, 左边为凌形按钮，右边为长方形按钮
+ (void)customizeNavBar:(UIViewController*)targetView withTitle:(NSString *)title buttonNameL:(NSString*)btnNameL buttonNameR:(NSString*)btnNameR actionL:(SEL)actionL actionR:(SEL)actionR
{
    // 有左边的按钮
    if (btnNameL)
    {
        // 设置左边按钮
        UIImage *normalImage = [UIImage imageNamed:@"back_normal.png"];
        UIImage *scaledImage=[UIImage imageWithCGImage:normalImage.CGImage scale:2 orientation:UIImageOrientationUp];
        normalImage=[scaledImage stretchableImageWithLeftCapWidth:scaledImage.size.width/2 topCapHeight:scaledImage.size.height/2];
        
        UIImage* downImage = [UIImage imageNamed:@"back_push.png"];
        UIImage* downScaleImage = [UIImage imageWithCGImage:downImage.CGImage scale:2 orientation:UIImageOrientationUp];
        downImage = [downScaleImage stretchableImageWithLeftCapWidth:scaledImage.size.width/2 topCapHeight:scaledImage.size.height/2];
        
        UIButton* leftButton = [[UIButton alloc] initWithFrame:CGRectMake(0, 0, normalImage.size.width-2, normalImage.size.height)];
        [leftButton setTitle:btnNameL forState:UIControlStateNormal];
        leftButton.titleLabel.font = [UIFont systemFontOfSize:BTN_TITLE_SIZE];
        [leftButton setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
        leftButton.titleEdgeInsets=UIEdgeInsetsMake(0, 7, 0, 0);
        [leftButton setBackgroundImage:normalImage forState:UIControlStateNormal];
        [leftButton setBackgroundImage:downScaleImage forState:UIControlStateHighlighted];
        [leftButton addTarget:targetView action:actionL forControlEvents:UIControlEventTouchUpInside];
        UIBarButtonItem* leftBarItem = [[UIBarButtonItem alloc] initWithCustomView:leftButton];
        targetView.navigationItem.leftBarButtonItem = leftBarItem;
        [leftBarItem release];
        [leftButton release];
    }
    
    // 有右边的按钮
    if (btnNameR)
    {
        // 设置右边按钮
        UIImage* normalImage = [UIImage imageNamed:@"btn_nav_next_normal"];
        UIImage* scaledImage=[UIImage imageWithCGImage:normalImage.CGImage scale:2 orientation:UIImageOrientationUp];
        normalImage=[scaledImage stretchableImageWithLeftCapWidth:scaledImage.size.width/2 topCapHeight:scaledImage.size.height/2];
        
        UIImage* downImage = [UIImage imageNamed:@"btn_nav_next_push.png"];
        UIImage* downScaleImage = [UIImage imageWithCGImage:downImage.CGImage scale:2 orientation:UIImageOrientationUp];
        downImage = [downScaleImage stretchableImageWithLeftCapWidth:scaledImage.size.width/2 topCapHeight:scaledImage.size.height/2];
        
        UIButton *rightBtn = [UIButton buttonWithType:UIButtonTypeCustom];
        rightBtn.frame = CGRectMake(0, 0, normalImage.size.width-2, normalImage.size.height);
        [rightBtn setBackgroundImage:normalImage forState:UIControlStateNormal];
        [rightBtn setBackgroundImage:downScaleImage forState:UIControlStateHighlighted];
        //加载的是图片
        if ([btnNameR rangeOfString:@".png"].location != NSNotFound)
        {
            UIImageView *btnImage = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 29, 25)];
            UIImage* normalImage = [UIImage imageNamed:btnNameR];
            btnImage.image = normalImage;
            btnImage.center = rightBtn.center;
            btnImage.backgroundColor = [UIColor clearColor];
            [rightBtn addSubview:btnImage];
            [btnImage release];
        
        }else{
            [rightBtn setTitle:btnNameR forState:UIControlStateNormal];
            [rightBtn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
            rightBtn.titleLabel.font = [UIFont systemFontOfSize:BTN_TITLE_SIZE];
            rightBtn.titleEdgeInsets=UIEdgeInsetsMake(0, 4, 0, 0);
        }        
        [rightBtn addTarget:targetView action:actionR forControlEvents:UIControlEventTouchUpInside];
        
        UIBarButtonItem *rightBarButtonItem=[[UIBarButtonItem alloc] initWithCustomView:rightBtn];
        targetView.navigationItem.rightBarButtonItem=rightBarButtonItem;
        [rightBarButtonItem release];
    }
    
    // 设置导航条标题
//    UILabel *titleLabel = [[[UILabel alloc] initWithFrame:CGRectMake(0, 0, 220, 44)] autorelease];
//    titleLabel.backgroundColor = [UIColor clearColor];
//    titleLabel.textColor = [UIColor whiteColor];
//    titleLabel.textAlignment = NSTextAlignmentCenter;
//    titleLabel.text = title;
//    targetView.navigationItem.titleView = titleLabel;
    targetView.title = title;
}

@end
