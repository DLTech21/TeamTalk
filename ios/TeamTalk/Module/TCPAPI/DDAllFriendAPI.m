//
//  DDAllFriendAPI.m
//  TeamTalk
//
//  Created by Donal Tong on 16/5/5.
//  Copyright © 2016年 DL. All rights reserved.
//

#import "DDAllFriendAPI.h"
#import "MTTUserEntity.h"
#import "IMBuddy.pb.h"
@implementation DDAllFriendAPI
/**
 *  请求超时时间
 *
 *  @return 超时时间
 */
- (int)requestTimeOutTimeInterval
{
    return TimeOutTimeInterval;
}

/**
 *  请求的serviceID
 *
 *  @return 对应的serviceID
 */
- (int)requestServiceID
{
    return SID_BUDDY_LIST;
}

/**
 *  请求返回的serviceID
 *
 *  @return 对应的serviceID
 */
- (int)responseServiceID
{
    return SID_BUDDY_LIST;
}

/**
 *  请求的commendID
 *
 *  @return 对应的commendID
 */
- (int)requestCommendID
{
    return IM_ALL_FRIEND_REQ;
}

/**
 *  请求返回的commendID
 *
 *  @return 对应的commendID
 */
- (int)responseCommendID
{
    return IM_ALL_FRIEND_RES;
}

/**
 *  解析数据的block
 *
 *  @return 解析数据的block
 */
- (Analysis)analysisReturnData
{
    
    Analysis analysis = (id)^(NSData* data)
    {
        IMAllFriendRsp *allFriendRsp = [IMAllFriendRsp parseFromData:data];
        uint32_t alllastupdatetime = allFriendRsp.latestUpdateTime;
        NSMutableDictionary *userAndVersion = [NSMutableDictionary new];
        [userAndVersion setObject:@(alllastupdatetime) forKey:@"friendlastupdatetime"];
        NSMutableArray *userList = [[NSMutableArray alloc] init];
        for (UserInfo *userInfo in [allFriendRsp userList]) {
            MTTUserEntity *user = [[MTTUserEntity alloc] initWithPB:userInfo];
            [userList addObject:user];
        }
        
        
        //        for (uint32_t i = 0; i < userCnt; i++) {
        //            NSString *userId =[bodyData readUTF];
        //            uint32_t version = [bodyData readInt];
        //            NSDictionary* result = nil;
        //            result = @{
        //                       @"userId":userId,
        //                       @"version":@(version),
        //                       };
        //            MTTUserEntity *user = [MTTUserEntity dicToUserEntity:result];
        //            [userList addObject:user];
        //
        //        }
        [userAndVersion setObject:userList forKey:@"userlist"];
        return userAndVersion;
    };
    return analysis;
}

/**
 *  打包数据的block
 *
 *  @return 打包数据的block
 */
- (Package)packageRequestObject
{
    Package package = (id)^(id object,uint32_t seqNo)
    {
        IMAllFriendReqBuilder *reqBuilder = [IMAllFriendReq builder];
        NSInteger version = [object[0] integerValue];
        [reqBuilder setUserId:0];
        [reqBuilder setLatestUpdateTime:version];
        
        DDDataOutputStream *dataout = [[DDDataOutputStream alloc] init];
        [dataout writeInt:0];
        [dataout writeTcpProtocolHeader:SID_BUDDY_LIST
                                    cId:IM_ALL_FRIEND_REQ
                                  seqNo:seqNo];
        [dataout directWriteBytes:[reqBuilder build].data];
        [dataout writeDataCount];
        return [dataout toByteArray];
    };
    return package;
}
@end
