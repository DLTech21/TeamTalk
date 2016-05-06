//
//  RegexUtil.h
//  qunyou
//
//  Created by Donal on 14-3-11.
//  Copyright (c) 2014年 vikaa. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RegexKitLite.h"
#import "NSString+Wrapper.h"

@interface RegexUtil : NSObject
+(BOOL)isChineseName:(NSString *)chineseName;
+(BOOL)isMobileNo:(NSString *)mobile;
+(BOOL)isEmail:(NSString *)email;
+(BOOL)isURL:(NSString *)url;
+(BOOL)isQYLogo:(NSString *)url;
+(NSString *)isBook:(NSString *)url;
@end
