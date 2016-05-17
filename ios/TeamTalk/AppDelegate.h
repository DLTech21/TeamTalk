//
//  AppDelegate.h
//  TeamTalk
//
//  Created by 宪法 on 15/6/18.
//  Copyright (c) 2015年 MoguIM. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MTTRootViewController.h"
@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) UINavigationController *mainNav;
@property (strong, nonatomic) MTTRootViewController *welcomeViewController;

@end

