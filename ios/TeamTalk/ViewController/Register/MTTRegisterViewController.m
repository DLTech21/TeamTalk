//
//  MTTRegisterViewController.m
//  TeamTalk
//
//  Created by Donal Tong on 16/4/12.
//  Copyright © 2016年 DL. All rights reserved.
//

#import "MTTRegisterViewController.h"
#import "UIView+SDAutoLayout.h"

@interface MTTRegisterViewController () <UITextFieldDelegate>
@property (nonatomic, strong) UITextField *accountTF;
@property (nonatomic, strong) UITextField *passTF;
@property (nonatomic, strong) UIButton *loginButton;
@end

@implementation MTTRegisterViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    [self initUi];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}

-(void)initUi
{
    self.view.backgroundColor = UIColorFromRGB(0xffffff, 1.0);
    _accountTF = [UITextField new];
    _accountTF.returnKeyType = UIReturnKeyNext;
    _accountTF.delegate = self;
    _accountTF.placeholder = @"账号";
    [_accountTF setBorderStyle:UITextBorderStyleNone];
    [_accountTF.layer setBorderWidth:1];
    [_accountTF.layer setBorderColor:UIColorFromRGB(0xdddddd, 1.0).CGColor];
    [self.view addSubview:_accountTF];
    _accountTF.sd_layout
    .leftSpaceToView(self.view, 20)
    .rightSpaceToView(self.view, 20)
    .topSpaceToView(self.view, 100)
    .heightIs(44)
    .centerXEqualToView(self.view);
    
    _passTF = [UITextField new];
    _passTF.delegate = self;
    _passTF.returnKeyType = UIReturnKeyDone;
    [self.view addSubview:_passTF];
    [_passTF setBorderStyle:UITextBorderStyleNone];
    _passTF.placeholder = @"密码";
    [_passTF.layer setBorderWidth:1];
    [_passTF.layer setBorderColor:UIColorFromRGB(0xdddddd, 1.0).CGColor];
    _passTF.sd_layout
    .leftSpaceToView(self.view, 20)
    .rightSpaceToView(self.view, 20)
    .heightIs(44)
    .topSpaceToView(_accountTF, 20 )
    .centerXEqualToView(self.view);
    UIView *padView1                = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 10, _passTF.frame.size.height)];
    _passTF.leftView              = padView1;
    _passTF.leftViewMode          = UITextFieldViewModeAlways;
    _passTF.secureTextEntry = YES;
    UIView *padView2                = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 10, _accountTF.frame.size.height)];
    _accountTF.leftView              = padView2;
    _accountTF.leftViewMode          = UITextFieldViewModeAlways;
    
    _loginButton = [UIButton new];
    [_loginButton setBackgroundColor:UIColorFromRGB(0x00abee, 1.0)];
    [_loginButton setTitle:@"注册" forState:UIControlStateNormal];
    [_loginButton addTarget:self action:@selector(login) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:_loginButton];
    _loginButton.sd_layout
    .leftSpaceToView(self.view, 20)
    .rightSpaceToView(self.view, 20)
    .heightIs(44)
    .topSpaceToView(_passTF, 60)
    .centerXEqualToView(self.view);
    
}

-(void)login
{
    NSString *account = _accountTF.text;
    NSString *password = _passTF.text;
    if (!(account.trim.length > 0) || !(password.trim.length > 0)) {
        [OHAlertView showAlertWithTitle:@"提示" message:@"" dismissButton:@"好的"];
        return;
    }
    [SVProgressHUD show];
    [[ApiClient sharedInstance] registerUser:account
                                    password:password
                                     Success:^(id model) {
                                         [SVProgressHUD dismiss];
                                         if ([[model valueForKey:@"status"] integerValue] == 0) {
                                             [self.navigationController popViewControllerAnimated:YES];
                                         }
                                         else {
                                             [OHAlertView showAlertWithTitle:@"提示" message:[model valueForKey:@"msg"] dismissButton:@"好的"];
                                         }
                                     }
                                     failure:^(NSString *message) {
                                         [SVProgressHUD dismiss];
                                         [OHAlertView showAlertWithTitle:@"提示" message:message dismissButton:@"好的"];
                                     }];
}

@end
