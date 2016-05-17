//
//  DDMTTLoginViewController.m
//  IOSDuoduo
//
//  Created by 独嘉 on 14-5-26.
//  Copyright (c) 2014年 dujia. All rights reserved.
//

#import "MTTLoginViewController.h"
#import "MTTRootViewController.h"
#import "LoginModule.h"
#import "SendPushTokenAPI.h"
#import "MBProgressHUD.h"
#import "SCLAlertView.h"
#import "UIView+SDAutoLayout.h"
#import "MTTRegisterViewController.h"
#import "DLAppUtil.h"
#import "MTTUserEntity.h"
@interface MTTLoginViewController ()<UITextFieldDelegate>

@property(assign)CGPoint defaultCenter;
@property (nonatomic, strong) UIButton *registerButton;
@property (nonatomic,weak)IBOutlet UITextField* userNameTextField;
@property (nonatomic,weak)IBOutlet UITextField* userPassTextField;
@property (nonatomic,weak)IBOutlet UIButton* userLoginBtn;
@property(assign)BOOL isRelogin;

@end

@implementation MTTLoginViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(handleWillShowKeyboard)
                                                     name:UIKeyboardWillShowNotification
                                                   object:nil];
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(handleWillHideKeyboard)
                                                     name:UIKeyboardWillHideNotification
                                                   object:nil];
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
  
    if ([[NSUserDefaults standardUserDefaults] objectForKey:@"username"]!=nil) {
        _userNameTextField.text =[[NSUserDefaults standardUserDefaults] objectForKey:@"username"];
    }
    if ([[NSUserDefaults standardUserDefaults] objectForKey:@"password"]!=nil) {
        _userPassTextField.text=[[NSUserDefaults standardUserDefaults] objectForKey:@"password"];
    }
    if(!self.isRelogin)
    {
        if ([[NSUserDefaults standardUserDefaults] objectForKey:@"username"] && [[NSUserDefaults standardUserDefaults] objectForKey:@"password"])
        {
            if ([[[NSUserDefaults standardUserDefaults] objectForKey:@"autologin"] boolValue] == YES) {
//                [self loginButtonPressed:nil];
            }
        }
    }
    
    self.defaultCenter=self.view.center;
    self.userNameTextField.leftViewMode=UITextFieldViewModeAlways;
    self.userPassTextField.leftViewMode=UITextFieldViewModeAlways;
    
    UIImageView *usernameLeftView = [[UIImageView alloc] init];
    usernameLeftView.contentMode = UIViewContentModeCenter;
    usernameLeftView.frame=CGRectMake(0, 0, 18, 22.5);
    UIImageView *pwdLeftView = [[UIImageView alloc] init];
    pwdLeftView.contentMode = UIViewContentModeCenter;
    pwdLeftView.frame=CGRectMake(0, 0,18, 22.5);
    self.userNameTextField.leftView=usernameLeftView;
    self.userPassTextField.leftView=pwdLeftView;
    [self.userNameTextField.layer setBorderColor:RGB(211, 211, 211).CGColor];
    [self.userNameTextField.layer setBorderWidth:0.5];
    [self.userNameTextField.layer setCornerRadius:4];
    [self.userPassTextField.layer setBorderColor:RGB(211, 211, 211).CGColor];
    [self.userPassTextField.layer setBorderWidth:0.5];
    [self.userPassTextField.layer setCornerRadius:4];
    
    [self.userLoginBtn.layer setCornerRadius:4];
    
    // 设置用户名
    [self.view setBackgroundColor:[UIColor whiteColor]];
    
    _registerButton = [ UIButton new];
    [_registerButton setBackgroundColor:UIColorFromRGB(0x00abee, 1.0)];
    [_registerButton setTitle:@"注册" forState:UIControlStateNormal];
    [_registerButton addTarget:self action:@selector(registerUser:) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:_registerButton];
    _registerButton.sd_layout
    .rightSpaceToView(self.view, 20)
    .widthIs(60)
    .heightIs(44)
    .topSpaceToView(self.userLoginBtn, 60)
    ;
    
}

-(void)registerUser:(id)sender
{
    MTTRegisterViewController *vc = [MTTRegisterViewController new];
    [self.navigationController pushViewController:vc animated:YES];
}

-(void)viewWillAppear:(BOOL)animated{
    
    [super viewWillAppear:animated];
    
    self.navigationController.navigationBarHidden =YES;
}

-(void)viewDidAppear:(BOOL)animated{

    [super viewDidAppear:animated];
    self.navigationController.navigationBarHidden =NO;
    self.defaultCenter=self.view.center;
}

#pragma mark - keyboard hide and show notification

-(void)handleWillShowKeyboard
{
    [UIView animateWithDuration:0.2 animations:^{
        self.view.center=CGPointMake(self.view.center.x, self.defaultCenter.y-(IPHONE4?120:40));
    }];
}
-(void)handleWillHideKeyboard
{
    [UIView animateWithDuration:0.2 animations:^{
        self.view.center=self.defaultCenter;
    }];
}


#pragma mark - button pressed

-(IBAction)hiddenKeyboard:(id)sender
{
    [_userNameTextField resignFirstResponder];
    [_userPassTextField resignFirstResponder];
}


- (IBAction)loginButtonPressed:(UIButton*)button{
    
    [self.userLoginBtn setEnabled:NO];
    NSString* userName = _userNameTextField.text ;
    NSString* password = _userPassTextField.text ;
    if (userName.length ==0 || password.length == 0) {
        [self.userLoginBtn setEnabled:YES];
        return;
    }
    MBProgressHUD *HUD = [[MBProgressHUD alloc] initWithView:self.view];
    [self.view addSubview:HUD];
    [HUD show:YES];
    HUD.dimBackground = YES;
    HUD.labelText = @"正在登录";
    
    [[LoginModule instance] loginWithUsername:userName password:password success:^(MTTUserEntity *user) {
        
        [HUD removeFromSuperview];
        
        [self.userLoginBtn setEnabled:YES];
        if (user) {
            TheRuntime.user=user ;
            [TheRuntime updateData];
            
            if (TheRuntime.pushToken) {
//                SendPushTokenAPI *pushToken = [[SendPushTokenAPI alloc] init];
//                [pushToken requestWithObject:TheRuntime.pushToken Completion:^(id response, NSError *error) {
//                    debugLog(@"%@", error.description);
//                }];
                TheRuntime.clientId_gettui = getUserCode;
                if (TheRuntime.clientId_gettui) {
                    [[ApiClient sharedInstance] updateUserPush:TheRuntime.clientId_gettui Success:^(id model) {
                    
                    } failure:^(NSString *message) {
                        
                    }];
                }
            }
            setUserID(user.objID);
            setUserNickname(user.nick);
            setUserAvatar(user.avatar);
            setUserPhone(userName);
            setUserPassword(password)
            setIsLogin;
            [[NSNotificationCenter defaultCenter] postNotificationName:KNOTIFICATION_LOGINCHANGE object:@YES];
        }
    } failure:^(NSString *error) {
        [self.userLoginBtn setEnabled:YES];

        [HUD removeFromSuperview];
        
        [OHAlertView showAlertWithTitle:@"提示" message:error dismissButton:@"好的"];
        
        
//        if([error isEqualToString:@"版本过低"])
//        {
//            DDLog(@"强制更新");
//            SCLAlertView *alert = [SCLAlertView new];
//            [alert addButton:@"确定" actionBlock:^{
//                [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"http://tt.mogu.io"]];
//            }];
//            [alert showError:self title:@"升级提示" subTitle:@"版本过低，需要强制更新" closeButtonTitle:nil duration:0];
//            
//        }else{
//            [self.userLoginBtn setEnabled:YES];
//            
//            [alert showError:self title:@"错误" subTitle:error closeButtonTitle:@"确定" duration:0];
//        }
    }];
    
}

#pragma mark - UITextFieldDelegate

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [textField resignFirstResponder];
    [self loginButtonPressed:nil];
    
    return YES;
}
@end
