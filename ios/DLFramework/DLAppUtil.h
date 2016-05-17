//
//  DLAppUtil.h
//  qunyou
//
//  Created by Donal on 14-3-13.
//  Copyright (c) 2014年 vikaa. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Tool.h"
#import "NSString+Wrapper.h"
#import "UIView+Utils.h"

//#import <SMS_SDK/SMSSDK.h>
//#import <SMS_SDK/SMSSDK+AddressBookMethods.h>
//#import <SMS_SDK/SMSSDK+DeprecatedMethods.h>
//#import <SMS_SDK/SMSSDK+ExtexdMethods.h>

@interface DLAppUtil : NSObject

#define WeChatAppID @"wx862d2680ba50c858"
#define WeChatSECRET @"21f63c64977c2b519e03387586170499"

//QQ
#define QQAppID @"101008107"
#define QQAppKey @"981895697a8193379d84538afb40adb2"

//Sina KEY
#define WeiboKey @"2409464355"
#define WeiboSecret  @"0afd424c45e7c95ff16c8c44d8d749a8"

#define ChatNewMsgNotifaction @"ChatNewMsgNotifaction"
#define ChatUpdateMsgNotifaction @"ChatUpdateMsgNotifaction"

#define ReLoginNotification @"ReLoginNotification"

#define PutActivitySucNotification @"PutActivitySucNotification"
#define PutPhonebookSucNotification @"PutPhonebookSucNotification"
#define PutCardNotification @"PutCardNotification"

#define HideAddSomethingViewNotification @"HideAddSomethingViewNotification"

#define WechatLoginNotification @"WechatLoginNotification"

#define KNOTIFICATION_LOGINCHANGE @"loginStateChange"
//FMDB
#define FMDBQuickCheck(SomeBool) { if (!(SomeBool)) { NSLog(@"Failure on line %d", __LINE__);} }
#define DATABASE_PATH [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES)[0]stringByAppendingString:[NSString stringWithFormat:@"/%@-%@.db", @"mi", getUserID]]

#ifdef DEBUG
//调试模式
#define debugLog(fmt, ...) NSLog((@"%s [Line %d] " fmt), __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)

//发布模式
#else

#define debugLog(...)

#endif

#define WEAKSELF typeof(self) __weak weakSelf = self;

#define UIColorFromRGB(rgbValue,al) [UIColor colorWithRed:((float)((rgbValue & 0xFF0000) >> 16))/255.0 green:((float)((rgbValue & 0xFF00) >> 8))/255.0 blue:((float)(rgbValue & 0xFF))/255.0 alpha:al]
#define IOS7 ([[[UIDevice currentDevice] systemVersion] floatValue] >= 7.0)
#define CURRENT_SYS_VERSION [[[UIDevice currentDevice] systemVersion] floatValue]
#define IS_IPHONE5 ([UIScreen instancesRespondToSelector:@selector(currentMode)] ? CGSizeEqualToSize(CGSizeMake(640, 1136), [[UIScreen mainScreen] currentMode].size) : NO)

#define SYSTEM_VERSION_GREATER_THAN(v) ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedDescending)

#define isGuided [[NSUserDefaults standardUserDefaults] boolForKey:IsGuide]
#define setIsGuided [[NSUserDefaults standardUserDefaults] setBool:YES forKey:IsGuide];[[NSUserDefaults standardUserDefaults] synchronize];
#define setNotGuided [[NSUserDefaults standardUserDefaults] setBool:No forKey:IsGuide];[[NSUserDefaults standardUserDefaults] synchronize];

#define setBaiduPushType(user) [[NSUserDefaults standardUserDefaults] setObject:(user) forKey:BaiduPushType];[[NSUserDefaults standardUserDefaults] synchronize];
#define getBaiduPushType [[NSUserDefaults standardUserDefaults] objectForKey:BaiduPushType]

#define setBaiduUserID(user) [[NSUserDefaults standardUserDefaults] setObject:(user) forKey:BaiduChannelId];[[NSUserDefaults standardUserDefaults] synchronize];
#define getBaiduUserID [[NSUserDefaults standardUserDefaults] objectForKey:BaiduChannelId]

#define setBaiduChannelID(user) [[NSUserDefaults standardUserDefaults] setObject:(user) forKey:BaiduUserId];[[NSUserDefaults standardUserDefaults] synchronize];
#define getBaiduChannelID [[NSUserDefaults standardUserDefaults] objectForKey:BaiduUserId]

#define IsNetwork [[NSUserDefaults standardUserDefaults] boolForKey:IsNetworking]
#define setIsNetwork(work) [[NSUserDefaults standardUserDefaults] setBool:work forKey:IsNetworking];[[NSUserDefaults standardUserDefaults] synchronize];

#define isLogin [[NSUserDefaults standardUserDefaults] boolForKey:IsUserLogin]
#define setIsLogin [[NSUserDefaults standardUserDefaults] setBool:YES forKey:IsUserLogin];[[NSUserDefaults standardUserDefaults] synchronize];
#define setLogout [[NSUserDefaults standardUserDefaults] setBool:FALSE forKey:IsUserLogin];[[NSUserDefaults standardUserDefaults] synchronize];

#define setUserID(user) [[NSUserDefaults standardUserDefaults] setObject:(user) forKey:APPUserID];[[NSUserDefaults standardUserDefaults] synchronize];
#define getUserID [[NSUserDefaults standardUserDefaults] objectForKey:APPUserID]

#define setUserMomentConver(user) [[NSUserDefaults standardUserDefaults] setObject:(user) forKey:APPUserMomentConver];[[NSUserDefaults standardUserDefaults] synchronize];
#define getUserMomentConver [[NSUserDefaults standardUserDefaults] objectForKey:APPUserMomentConver]


#define setUserAvatar(user) [[NSUserDefaults standardUserDefaults] setObject:(user) forKey:APPUserAvatar];[[NSUserDefaults standardUserDefaults] synchronize];
#define getUserAvatar [[NSUserDefaults standardUserDefaults] objectForKey:APPUserAvatar]

#define setUserNickname(user) [[NSUserDefaults standardUserDefaults] setObject:(user) forKey:APPUserNickname];[[NSUserDefaults standardUserDefaults] synchronize];
#define getUserNickname [[NSUserDefaults standardUserDefaults] objectForKey:APPUserNickname]

#define setUserSign(user) [[NSUserDefaults standardUserDefaults] setObject:(user) forKey:APPUserSign];[[NSUserDefaults standardUserDefaults] synchronize];
#define getUserSign [[NSUserDefaults standardUserDefaults] objectForKey:APPUserSign]

#define setUserHash(user) [[NSUserDefaults standardUserDefaults] setObject:(user) forKey:APPUserHash];[[NSUserDefaults standardUserDefaults] synchronize];
#define getUserHash [[NSUserDefaults standardUserDefaults] objectForKey:APPUserHash]

#define setDeviceToken(user) [[NSUserDefaults standardUserDefaults] setObject:(user) forKey:DeviceToken];[[NSUserDefaults standardUserDefaults] synchronize];
#define getDeviceToken [[NSUserDefaults standardUserDefaults] objectForKey:DeviceToken]

#define setOauthType(oauthType) [[NSUserDefaults standardUserDefaults] setInteger:oauthType forKey:OauthType];[[NSUserDefaults standardUserDefaults] synchronize];
#define getOauthType [[NSUserDefaults standardUserDefaults] integerForKey:OauthType]

#define setQiniuToken(user) [[NSUserDefaults standardUserDefaults] setObject:(user) forKey:QiniuToken];[[NSUserDefaults standardUserDefaults] synchronize];
#define getQiniuToken [[NSUserDefaults standardUserDefaults] objectForKey:QiniuToken]

#define setUserSex(user) [[NSUserDefaults standardUserDefaults] setObject:(user) forKey:UserSex];[[NSUserDefaults standardUserDefaults] synchronize];
#define getUserSex [[NSUserDefaults standardUserDefaults] objectForKey:UserSex]

#define setUserCode(user) [[NSUserDefaults standardUserDefaults] setObject:(user) forKey:APPUserCode];[[NSUserDefaults standardUserDefaults] synchronize];
#define getUserCode [[NSUserDefaults standardUserDefaults] objectForKey:APPUserCode]

#define setUserPhone(user) [[NSUserDefaults standardUserDefaults] setObject:(user) forKey:UserPhone];[[NSUserDefaults standardUserDefaults] synchronize];
#define getUserPhone [[NSUserDefaults standardUserDefaults] objectForKey:UserPhone]

#define setUserPassword(user) [[NSUserDefaults standardUserDefaults] setObject:(user) forKey:UserPassword];[[NSUserDefaults standardUserDefaults] synchronize];
#define getUserPassword [[NSUserDefaults standardUserDefaults] objectForKey:UserPassword]

#define setUserPersonalSign(user) [[NSUserDefaults standardUserDefaults] setObject:(user) forKey:UserPersonalSign];[[NSUserDefaults standardUserDefaults] synchronize];
#define getUserPersonalSign [[NSUserDefaults standardUserDefaults] objectForKey:UserPersonalSign]

#define setUserArea(user) [[NSUserDefaults standardUserDefaults] setObject:(user) forKey:UserArea];[[NSUserDefaults standardUserDefaults] synchronize];
#define getUserArea [[NSUserDefaults standardUserDefaults] objectForKey:UserArea]

#define setFontScale(user) [[NSUserDefaults standardUserDefaults] setObject:(user) forKey:FontScale];[[NSUserDefaults standardUserDefaults] synchronize];
#define getFontScale [[NSUserDefaults standardUserDefaults] objectForKey:FontScale]

#define setSoundOn(user) [[NSUserDefaults standardUserDefaults] setObject:(user) forKey:SoundOn];[[NSUserDefaults standardUserDefaults] synchronize];
#define getSoundOn [[NSUserDefaults standardUserDefaults] objectForKey:SoundOn]

#define setVibrationOn(user) [[NSUserDefaults standardUserDefaults] setObject:(user) forKey:VibrationOn];[[NSUserDefaults standardUserDefaults] synchronize];
#define getVibrationOn [[NSUserDefaults standardUserDefaults] objectForKey:VibrationOn]

//nsuserdefault key
#define IsGuide @"isGuide"
#define BaiduPushType @"BaiduPushType"
#define BaiduUserId @"BaiduUserId"
#define BaiduChannelId @"BaiduChannelId"
#define IsNetworking @"IsNetwork"
#define IsUserLogin @"isUserLogin"
#define APPUserID @"app.user.id"
#define APPUserAvatar @"app.user.avatar"
#define APPUserNickname @"app.user.nickname"
#define APPUserSign @"app.user.sign"
#define APPUserHash @"app.user.hash"
#define APPUserCode @"app.user.code"
#define APPUserMomentConver @"app.user.moment_cover"
#define OauthType @"OauthType"
#define DeviceToken @"DeviceToken"
#define QiniuToken @"QiniuToken"
#define UserSex @"UserSex"
#define UserPersonalSign @"app.user.personalsign"
#define UserPhone @"app.user.phone"
#define UserArea @"UserArea"
#define UserPassword @"app.user.password"
#define FontScale @"FontScale"

#define SoundOn @"SoundOn"
#define VibrationOn @"VibrationOn"


//TABLEVIEW SCROLL STATE
#define TABLEVIEW_ACTION_NORMAL 0
#define TABLEVIEW_ACTION_INIT 1
#define TABLEVIEW_ACTION_REFRESH 2
#define TABLEVIEW_ACTION_SCROLL 3
//TABLEVIEW DATA STATE
#define TABLEVIEW_DATA_NORMAL 0
#define TABLEVIEW_DATA_MORE 1
#define TABLEVIEW_DATA_LOADING 2
#define TABLEVIEW_DATA_FULL 3
#define TABLEVIEW_DATA_EMPTY 4
#define TABLEVIEW_DATA_ERROR 5

#define PageCount 10

#define TableViewDragUpHeight 10

#define ButtonEnLargeEdge 40

#define StatusBarHeight 20
#define screenframe [[UIScreen mainScreen] bounds]

//设备物理尺寸
#define screen_width [UIScreen mainScreen].bounds.size.width
#define screen_height [UIScreen mainScreen].bounds.size.height
#define Myuser [NSUserDefaults standardUserDefaults]

//oauth type
#define WXOauthType 1
#define ShareOauthType 2

#define QiNiuAccessKey @"ZJQmyoJNrIuXq81X-naqxYkMkjlvhNUiVgdQXBk6"
#define QiNiuSecertKey @"NrM8Fl1OmxqliLTtoP9m94JkIGf8vIuca7O_OG-9"
#define QiNiuBucket @"dchat"
#define QINIU_PREFIX @"http://7xj66h.com1.z0.glb.clouddn.com/"

#define liheurl @"http://60.212.40.125:8888/tc_gift/"

#define AVATAR_PLACEURL @"http://7xj66h.com1.z0.glb.clouddn.com/super_icon.png"
//正常字体大小
#define FONT_NORMAL_TITLE 15
#define FONT_NORMAL_SUBTITLE 14
@end
