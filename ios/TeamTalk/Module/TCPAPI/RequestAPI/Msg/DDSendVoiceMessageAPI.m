//
//  DDSendVoiceMessageAPI.m
//  TeamTalk
//
//  Created by Donal Tong on 16/3/30.
//  Copyright © 2016年 DL. All rights reserved.
//

#import "DDSendVoiceMessageAPI.h"
#import "AFHTTPRequestOperationManager.h"

#import "MTTMessageEntity.h"
#import "MTTPhotosCache.h"
#import "NSDictionary+Safe.h"
#import "MTTUtil.h"
static int max_try_upload_times = 5;
@interface DDSendVoiceMessageAPI ()
@property(nonatomic,strong)AFHTTPRequestOperationManager *manager;
@property(nonatomic,strong)NSOperationQueue *queue;
@property(assign)bool isSending;
@end

@implementation DDSendVoiceMessageAPI
+ (DDSendVoiceMessageAPI *)sharedVoiceCache
{
    static dispatch_once_t oncev;
    static id instance;
    dispatch_once(&oncev, ^{
        instance = [self new];
    });
    return instance;
}
- (instancetype)init
{
    self = [super init];
    if (self) {
        self.manager = [AFHTTPRequestOperationManager manager];
        self.manager.responseSerializer.acceptableContentTypes
        = [NSSet setWithObject:@"text/html"];
        self.queue = [NSOperationQueue new];
        self.queue.maxConcurrentOperationCount = 1;
        
    }
    return self;
}
- (void)uploadVoice:(NSString*)voicePath success:(void(^)(NSString* voiceURL))success failure:(void(^)(id error))failure;
{
    
    NSBlockOperation *operation = [NSBlockOperation blockOperationWithBlock:^(){
        NSURL *url = [NSURL URLWithString:[MTTUtil getMsfsUrl]];
        NSString *urlString =  [url.absoluteString stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
        @autoreleasepool
        {
            __block NSData *imageData = [NSData dataWithContentsOfFile:voicePath];
            if (imageData == nil) {
                failure(@"data is emplty");
                return;
            }
            NSString *voiceName = [NSString stringWithFormat:@"voice_%@.spx",[Tool getMd5_16Bit_String:voicePath]];
            NSDictionary *params =[NSDictionary dictionaryWithObjectsAndKeys:@"im_voice",@"type", nil];
            [self.manager POST:urlString parameters:params constructingBodyWithBlock:^(id<AFMultipartFormData> formData) {
                [formData appendPartWithFileData:imageData name:@"voice" fileName:voiceName mimeType:@"audio/ogg"];
            } success:^(AFHTTPRequestOperation *operation, id responseObject) {
                debugLog(@"%@", responseObject);
                imageData =nil;
                NSInteger statusCode = [operation.response statusCode];
                if (statusCode == 200) {
                    NSString *imageURL=nil;
                    if ([responseObject isKindOfClass:[NSDictionary class]]) {
                        if ([[responseObject safeObjectForKey:@"error_code"] intValue]==0) {
                            imageURL = [responseObject safeObjectForKey:@"url"];
                        }else{
                            failure([responseObject safeObjectForKey:@"error_msg"]);
                        }
                        
                    }
                    
//                    NSMutableString *url = [NSMutableString stringWithFormat:@"%@",DD_MESSAGE_IMAGE_PREFIX];
                    if (!imageURL)
                    {
                        max_try_upload_times --;
                        if (max_try_upload_times > 0)
                        {
                            
                            [self uploadVoice:voicePath success:^(NSString *voiceURL) {
                                success(voiceURL);
                            } failure:^(id error) {
                                failure(error);
                            }];
                        }
                        else
                        {
                            failure(nil);
                        }
                        
                    }
                    if (imageURL) {
//                        [url appendString:imageURL];
//                        [url appendString:@":}]&$~@#@"];
                        success(imageURL);
                    }
                }
                else
                {
                    self.isSending=NO;
                    failure(nil);
                }
                
            } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
                debugLog(@"%@", error.debugDescription);
                self.isSending=NO;
                NSDictionary* userInfo = error.userInfo;
                NSHTTPURLResponse* response = userInfo[AFNetworkingOperationFailingURLResponseErrorKey];
                NSInteger stateCode = response.statusCode;
                if (!(stateCode >= 300 && stateCode <=307))
                {
                    failure(@"断网");
                }
            }];
        }
    }];
    [self.queue addOperation:operation];
    
}
+(NSString *)imageUrl:(NSString *)content{
    NSRange range = [content rangeOfString:@"path="];
    NSString* url = nil;
    if ([content length] > range.location + range.length)
    {
        url = [content substringFromIndex:range.location+range.length];
    }
    url = [(NSString *)url stringByReplacingOccurrencesOfString:@"+" withString:@" "];
    url = [url stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    return url;
}
@end
