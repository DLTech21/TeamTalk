//
//  ApiClient.m
//  TeamTalk
//
//  Created by Donal Tong on 16/4/12.
//  Copyright © 2016年 DL. All rights reserved.
//

#import "ApiClient.h"
#import "MTTConfig.h"

@implementation ApiClient

+ (id)sharedInstance{
    static ApiClient *_sharedInstance;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _sharedInstance = [[ApiClient alloc] initWithBaseURL:[NSURL URLWithString:APIURL]];
    });
    return _sharedInstance;
}

- (id)initWithBaseURL:(NSURL *)url {
    self = [super initWithBaseURL:url];
    if (self) {
        NSMutableSet *contentTypes = [[NSMutableSet alloc] initWithSet:self.responseSerializer.acceptableContentTypes];
        [contentTypes addObject:@"text/html"];
        [contentTypes addObject:@"text/plain"];
        [contentTypes addObject:@"multipart/form-data"];
        AFJSONResponseSerializer *responseSerializer = [AFJSONResponseSerializer serializerWithReadingOptions:NSJSONReadingAllowFragments];
        responseSerializer.acceptableContentTypes = contentTypes;
        AFHTTPRequestSerializer *request =  [AFHTTPRequestSerializer serializer];
        [request setTimeoutInterval:120];
        [self setRequestSerializer:request];
        [self setResponseSerializer:responseSerializer];
    }
    return self;
}

- (NSMutableDictionary *)defaultGetParameters{
    NSMutableDictionary *parameters = [NSMutableDictionary dictionary];
    return parameters;
}


-(void)registerUser:(NSString *)name
           password:(NSString *)password
            Success:(void (^)(id model) )success
            failure:(void (^)(NSString *message) )failure
{
    NSMutableDictionary *parameters = [self defaultGetParameters];
    [parameters setValue:name forKey:@"name"];
    [parameters setValue:name forKey:@"nickname"];
    [parameters setValue:@"1" forKey:@"departId"];
    [parameters setValue:@"1" forKey:@"sex"];
    [parameters setValue:password forKey:@"pass"];
    [parameters setValue:@"" forKey:@"avatar"];
    [parameters setValue:@"" forKey:@"phone"];
    [parameters setValue:@"" forKey:@"email"];
    [parameters setValue:@"0" forKey:@"domain"];
    [[ApiClient sharedInstance] POST:@"/Home/User/registerUser"
                          parameters:parameters
                             success:^(AFHTTPRequestOperation *operation, id responseObject) {
                                 success(responseObject);
                             }
                             failure:^(AFHTTPRequestOperation *operation, NSError *error) {
                                 failure(error.description);
                             }];
}
@end
