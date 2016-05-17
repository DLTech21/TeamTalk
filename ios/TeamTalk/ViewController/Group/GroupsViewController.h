//
//  GroupsViewController.h
//  TeamTalk
//
//  Created by Donal Tong on 16/5/11.
//  Copyright © 2016年 DL. All rights reserved.
//

#import "MTTPullScrollViewController.h"
#import "MTTSessionEntity.h"
@interface GroupsViewController : MTTPullScrollViewController<UITableViewDataSource,UITableViewDelegate,UISearchBarDelegate,UISearchDisplayDelegate,UIScrollViewDelegate>
// 搜索关键字
@property(nonatomic,strong)NSString *searchKey;
// 主页面搜索
@property(nonatomic,assign)BOOL isFromSearch;
// @功能搜索
@property(nonatomic,assign)BOOL isFromAt;

@property (nonatomic,copy) void(^selectUser)(MTTUserEntity *userEntity);

@end

