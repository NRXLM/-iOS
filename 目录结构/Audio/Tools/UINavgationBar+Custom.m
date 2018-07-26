//
//  UINavgationBar+Custom.m
//  REducation
//
//  Created by changxm on 14-3-22.
//  Copyright (c) 2014年 changxm. All rights reserved.
//

#import "UINavgationBar+Custom.h"

@implementation UINavigationBar (Customized)

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/

-(void)loadNavigationBar
{
    UIImage* backImg = nil;
    if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 7) {
        backImg = [UIImage imageNamed:@"title-128.png"];
    }
    else
    {
        backImg = [UIImage imageNamed:@"title.png"];
    }
    UIImage* normalImg = [UIImage imageWithCGImage:backImg.CGImage scale:2 orientation:UIImageOrientationUp];
    [self setBackgroundImage:normalImg forBarMetrics:UIBarMetricsDefault];
    //self.translucent = YES;
}


@end
