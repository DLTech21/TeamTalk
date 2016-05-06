//
//  DDSendVoiceMessageAPI.h
//  TeamTalk
//
//  Created by Donal Tong on 16/3/30.
//  Copyright © 2016年 DL. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface DDSendVoiceMessageAPI : NSObject
+ (DDSendVoiceMessageAPI *)sharedVoiceCache;
- (void)uploadVoice:(NSString*)voicePath success:(void(^)(NSString* voiceURL))success failure:(void(^)(id error))failure;
@end
