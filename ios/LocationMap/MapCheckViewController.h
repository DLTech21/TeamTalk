//
//  MapCheckViewController.h
//  happychat
//
//  Created by Donal Tong on 16/1/7.
//  Copyright © 2016年 dl. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <BaiduMapAPI_Base/BMKBaseComponent.h>//引入base相关所有的头文件
#import <BaiduMapAPI_Location/BMKLocationComponent.h>//引入定位功能所有的头文件
#import <BaiduMapAPI_Map/BMKMapComponent.h>//引入地图功能所有的头文件
#import <BaiduMapAPI_Search/BMKSearchComponent.h>//引入搜索功能所有的头文件

@interface MapCheckViewController : UIViewController

@property(nonatomic) CLLocationCoordinate2D _pt;
@property(nonatomic, strong) NSString * address;

@end
