//
//  Place.h
//  SmallKitchen
//
//  Created by Alesary on 15/11/24.
//  Copyright © 2015年 Alesary. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <BaiduMapAPI_Base/BMKBaseComponent.h>//引入base相关所有的头文件
#import <BaiduMapAPI_Location/BMKLocationComponent.h>//引入定位功能所有的头文件
#import <BaiduMapAPI_Map/BMKMapComponent.h>//引入地图功能所有的头文件
#import <BaiduMapAPI_Search/BMKSearchComponent.h>//引入搜索功能所有的头文件
@interface Place : NSObject

@property(nonatomic,strong)NSString *name;

@property(nonatomic,strong)NSString *address;

@property(nonatomic) CLLocationCoordinate2D _pt;
@end
