//
//  DDChatLocationCell.h
//  TeamTalk
//
//  Created by Donal Tong on 16/3/21.
//  Copyright © 2016年 DL. All rights reserved.
//

#import "DDChatBaseCell.h"

@interface DDChatLocationCell : DDChatBaseCell <DDChatCellProtocol>
@property(nonatomic,strong)UIImageView *msgImgView;
- (void)sendLocationAgain:(MTTMessageEntity*)message;
@end
