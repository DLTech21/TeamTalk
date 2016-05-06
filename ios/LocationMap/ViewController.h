//
//  ViewController.h
//  LoctionMAP
//
//  Created by Alesary on 15/11/24.
//  Copyright © 2015年 Mr.Chen. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol LocationViewControllerDelegate <NSObject>

-(void)sendLocationLatitude:(double)latitude longitude:(double)longitude andAddress:(NSString *)address;

@end

@interface ViewController : UIViewController

@property (nonatomic, assign) id<LocationViewControllerDelegate> delegate;
@end

