//
//  RootViewController.m
//  vh264enc
//
//  Created by hubo on 2018/8/10.
//  Copyright © 2018年 OIT. All rights reserved.
//

#import "RootViewController.h"
#import "ViewController.h"

@interface RootViewController ()<UITextFieldDelegate>
@property (weak, nonatomic) IBOutlet UITextField *phoneTF;
@property (weak, nonatomic) IBOutlet UITextField *workTF;
@property (weak, nonatomic) IBOutlet UITextField *lbsTF;

@end

@implementation RootViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    self.navigationItem.title = @"请设置参数";
}

-(void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    [self.view endEditing:YES];
}

-(BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [self.view endEditing:YES];
    return YES;
}

- (IBAction)clickEvent:(id)sender {
    
    if (_phoneTF.text.length == 0 || _workTF.text.length == 0 || _lbsTF.text.length == 0) {
        return;
    }
    
    ViewController *vc = [[ViewController alloc] init];
    vc.phoneNum = _phoneTF.text;
    vc.workNum = _workTF.text;
    vc.lbsNum = _lbsTF.text;
    
    [self.navigationController pushViewController:vc animated:YES];
}

@end
