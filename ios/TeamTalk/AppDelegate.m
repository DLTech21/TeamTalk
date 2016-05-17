//
//  AppDelegate.m
//  TeamTalk
//
//  Created by 宪法 on 15/6/18.
//  Copyright (c) 2015年 MoguIM. All rights reserved.
//

#import "AppDelegate.h"
#import "MTTLoginViewController.h"
#import "ChattingMainViewController.h"
#import "DDClientStateMaintenanceManager.h"
#import "SessionModule.h"
#import "NSDictionary+Safe.h"
#import <Fabric/Fabric.h>
//#import <Crashlytics/Crashlytics.h>
#define MAP_AK @"xdNbFSrVXeS5pzG5cpnLvfhZ"
#define AppKey @"272bc3eaa8dc"
#define AppSecret @"a10211aac04334b49d39b779a350621e"
#import <BaiduMapAPI_Base/BMKBaseComponent.h>
#import "Tool.h"
#import "GeTuiSdk.h"
#import "DLAppUtil.h"
@interface AppDelegate ()<GeTuiSdkDelegate>

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
//    [Fabric with:@[CrashlyticsKit]];
    [GeTuiSdk startSdkWithAppId:kGtAppId appKey:kGtAppKey appSecret:kGtAppSecret delegate:self];
    BMKMapManager* mapManager = [[BMKMapManager alloc]init];
    BOOL ret = [mapManager start:MAP_AK  generalDelegate:nil];
    if (!ret) {
        debugLog(@"manager start failed!");
    }
    [DDClientStateMaintenanceManager shareInstance];
    [RuntimeStatus instance];
    
    // 移除webview cache
    [[NSURLCache sharedURLCache] removeAllCachedResponses];
    
    // 推送消息的注册方式
    if ([application respondsToSelector:@selector(registerUserNotificationSettings:)]) {
        // for iOS 8
        UIUserNotificationSettings* settings = [UIUserNotificationSettings settingsForTypes:UIUserNotificationTypeAlert | UIUserNotificationTypeBadge | UIUserNotificationTypeSound categories:nil];
        [[UIApplication sharedApplication] registerUserNotificationSettings:settings];
        [application registerForRemoteNotifications];
    } else {
        // for iOS 7 or iOS 6
        [[UIApplication sharedApplication] registerForRemoteNotificationTypes:(UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound | UIRemoteNotificationTypeAlert)];
    }
    
    if( SYSTEM_VERSION >=8 ) {
        [[UINavigationBar appearance] setTranslucent:YES];
    }
    
    [[UINavigationBar appearance] setBarStyle:UIBarStyleDefault];
    [[UINavigationBar appearance] setTitleTextAttributes:[NSDictionary dictionaryWithObject:[UIColor blackColor] forKey:NSForegroundColorAttributeName]];
    
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    self.window.backgroundColor = [UIColor whiteColor];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(loginStateChange:)
                                                 name:KNOTIFICATION_LOGINCHANGE
                                               object:nil];
//    MTTLoginViewController *loginVC =[[MTTLoginViewController alloc] initWithNibName:@"MTTLoginViewController" bundle:nil];
//    UINavigationController *navRoot =[[UINavigationController alloc] initWithRootViewController:loginVC];
//    navRoot.hidesBottomBarWhenPushed =YES;
//    self.window.rootViewController =navRoot;
    [self loginStateChange:nil];
    [self.window makeKeyAndVisible];
    
    return YES;
}

-(void)loginStateChange:(NSNotification *)notification
{
    [SVProgressHUD setDefaultMaskType:SVProgressHUDMaskTypeBlack];
    BOOL loginSuccess = [notification.object boolValue];
    if (isLogin || loginSuccess) {
        if (_welcomeViewController == nil) {
            _welcomeViewController = [[MTTRootViewController alloc]init];
            
        }
        _mainNav = [[UINavigationController alloc] initWithRootViewController:_welcomeViewController];
        self.window.rootViewController = _mainNav;
    }
    else{
        _welcomeViewController = nil;
        _mainNav = nil;
        MTTLoginViewController *loginVC =[[MTTLoginViewController alloc] initWithNibName:@"MTTLoginViewController" bundle:nil];
        UINavigationController *navRoot =[[UINavigationController alloc] initWithRootViewController:loginVC];
        navRoot.hidesBottomBarWhenPushed =YES;
        self.window.rootViewController = navRoot;
    }
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
    
    if ([[SessionModule instance]getAllUnreadMessageCount] == 0) {
        [[UIApplication sharedApplication] setApplicationIconBadgeNumber:0];
    }else{
        [[UIApplication sharedApplication] setApplicationIconBadgeNumber:[[SessionModule instance]getAllUnreadMessageCount]];
    }
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

//#ifdef __IPHONE_8_0
//- (void)application:(UIApplication *)application didRegisterUserNotificationSettings:(UIUserNotificationSettings *)notificationSettings
//{
//    //register to receive notifications
//    [application registerForRemoteNotifications];
//}
//
//- (void)application:(UIApplication *)application handleActionWithIdentifier:(NSString *)identifier forRemoteNotification:(NSDictionary *)userInfo completionHandler:(void(^)())completionHandler
//{
//    //handle the actions
//    if ([identifier isEqualToString:@"declineAction"]){
//    }
//    else if ([identifier isEqualToString:@"answerAction"]){
//    }
//}
//#endif

- (void)application:(UIApplication *)app didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
    NSString *token = [NSString stringWithFormat:@"%@", deviceToken];
    NSString *dt = [token stringByTrimmingCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"<>"]];
    NSString *dn = [dt stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    TheRuntime.pushToken= [dn stringByReplacingOccurrencesOfString:@" " withString:@""];
    [GeTuiSdk registerDeviceToken:TheRuntime.pushToken];
}

- (void)application:(UIApplication *)app didFailToRegisterForRemoteNotificationsWithError:(NSError *)error {
    NSString *error_str = [NSString stringWithFormat: @"%@", error];
}
// 处理推送消息
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo{
    UIApplicationState state =application.applicationState;
    if ( state != UIApplicationStateBackground) {
        return;
    }
    NSString *jsonString = [userInfo safeObjectForKey:@"custom"];
    NSData* infoData = [jsonString dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary* info = [NSJSONSerialization JSONObjectWithData:infoData options:0 error:nil];
    NSInteger from_id =[[info safeObjectForKey:@"from_id"] integerValue];
    SessionType type = (SessionType)[[info safeObjectForKey:@"msg_type"] integerValue];
    NSInteger group_id =[[info safeObjectForKey:@"group_id"] integerValue];
    if (from_id) {
        NSInteger sessionId = type==1?from_id:group_id;
        MTTSessionEntity *session = [[MTTSessionEntity alloc] initWithSessionID:[MTTUtil changeOriginalToLocalID:(UInt32)sessionId SessionType:(int)type] type:type] ;
        [[ChattingMainViewController shareInstance] showChattingContentForSession:session];
    }
}

- (void)GeTuiSdkDidRegisterClient:(NSString *)clientId {
    setUserCode(clientId);
}

/** SDK遇到错误回调 */
- (void)GeTuiSdkDidOccurError:(NSError *)error {
    debugLog(@"\n>>>[GexinSdk error]:%@\n\n", [error localizedDescription]);
}
@end
