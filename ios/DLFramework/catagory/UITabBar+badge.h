//
//  UITabBar+badge.h
//  happychat
//
//  Created by Donal Tong on 16/3/15.
//  Copyright © 2016年 dl. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UITabBar (badge)

- (void)showBadgeOnItemIndex:(int)index;   //显示小红点

- (void)hideBadgeOnItemIndex:(int)index; //隐藏小红点

@end
