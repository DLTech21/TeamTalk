//
//  ApiClient.h
//  TeamTalk
//
//  Created by Donal Tong on 16/4/12.
//  Copyright © 2016年 DL. All rights reserved.
//

#import <AFNetworking/AFNetworking.h>

@interface ApiClient : AFHTTPSessionManager

+ (id)sharedInstance;

-(void)registerUser:(NSString *)name
           password:(NSString *)password
            Success:(void (^)(id model) )success
            failure:(void (^)(NSString *message) )failure;

@end
