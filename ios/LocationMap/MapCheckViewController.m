//
//  MapCheckViewController.m
//  happychat
//
//  Created by Donal Tong on 16/1/7.
//  Copyright © 2016年 dl. All rights reserved.
//

#import "MapCheckViewController.h"
#import "UIView+SDAutoLayout.h"
#import <BaiduMapAPI_Utils/BMKUtilsComponent.h>
#import "SVProgressHUD.h"
#import <MapKit/MapKit.h>
#import <AddressBook/AddressBook.h>
#import "LCActionSheet.h"
@interface MapCheckViewController () <BMKMapViewDelegate, BMKLocationServiceDelegate>
{
    BMKMapView *_mapView;
    UIView *bottomView;
    UILabel *tapLabel;
    UILabel *locationLabel;
    UIButton *navButton;
    BMKLocationService *_locService;
    BMKUserLocation *currentUserLocation;
    NSMutableArray *navapps;
}
@end

@implementation MapCheckViewController

-(void)dealloc
{
    [SVProgressHUD dismiss];
}

-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    [_mapView viewWillAppear];
    _mapView.delegate=self;
    _locService.delegate = self;
}
-(void)viewWillDisappear:(BOOL)animated
{
    [_mapView viewWillDisappear];
    _mapView.delegate = nil; //不用时，置nil
    _locService.delegate = nil;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    [self initMap];
    [self initLocationService];
    [self initUI];
}

-(void)initUI
{
    UIButton *backButton = [UIButton new];
    backButton.left = 10;
    backButton.top = 30;
    backButton.width = 48;
    backButton.height = 48;
    [backButton setTitle:@"" forState:UIControlStateNormal];
    [backButton setImage:[UIImage imageNamed:@"icon_back"] forState:UIControlStateNormal];
    [backButton addTarget:self action:@selector(back:) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:backButton];
    navapps = [NSMutableArray new];
    [navapps addObject:@"苹果地图"];
    if ( [[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:@"baidumap://"]])
    {
        [navapps addObject:@"百度地图"];
    }
    if ( [[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:@"iosamap://"]])
    {
        [navapps addObject:@"高德地图"];
    }
    if ( [[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:@"comgooglemaps://"]])
    {
        [navapps addObject:@"谷歌地图"];
    }
}

-(void)back:(id)sender
{
    [self dismissViewControllerAnimated:YES completion:nil];
}

-(void)initLocationService
{
    if (_locService==nil) {
        
        _locService = [[BMKLocationService alloc]init];
        
        [_locService setDesiredAccuracy:kCLLocationAccuracyBest];
    }
    
    _locService.delegate = self;
    
    
    
}

-(void)initMap
{
    [self.view setBackgroundColor:UIColorFromRGB(0xffffff, 1.0)];
    if (_mapView==nil) {
        
        _mapView=[[BMKMapView alloc]initWithFrame:CGRectMake(0, 0, screen_width, screen_height)];
        
        [_mapView setMapType:BMKMapTypeStandard];// 地图类型 ->卫星／标准、
        
        _mapView.zoomLevel=19;
        
        _mapView.showsUserLocation = YES;
    }
    
    _mapView.delegate=self;
    
    _mapView.centerCoordinate = self._pt;
    [self.view addSubview:_mapView];
    
    BMKPointAnnotation* annotation = [[BMKPointAnnotation alloc]init];
    annotation.coordinate = self._pt;
    annotation.title = self.address;
    [_mapView addAnnotation:annotation];
    _mapView.sd_layout.bottomSpaceToView(self.view, 80);
    bottomView = [UIView new];
    [self.view addSubview:bottomView];
    bottomView.sd_layout
    .leftSpaceToView(self.view, 0)
    .rightSpaceToView(self.view, 0)
    .topSpaceToView(_mapView, 0)
    .bottomSpaceToView(self.view, 0);
    
    tapLabel = [UILabel new];
    [bottomView addSubview:tapLabel];
    tapLabel.sd_layout
    .leftSpaceToView(bottomView, 10)
    .topSpaceToView(bottomView, 10)
    .rightSpaceToView(bottomView, 0)
    .autoHeightRatio(0);
    tapLabel.text = @"[位置]";
    tapLabel.textColor = UIColorFromRGB(0x000000, 1.0);
    tapLabel.font = [UIFont systemFontOfSize:21];
    
    locationLabel = [UILabel new];
    [bottomView addSubview:locationLabel];
    locationLabel.sd_layout
    .leftSpaceToView(bottomView, 10)
    .rightSpaceToView(bottomView, 100)
    .topSpaceToView(tapLabel, 10)
    .autoHeightRatio(0);
    locationLabel.text = _address;
    locationLabel.textColor = UIColorFromRGB(0xdddddd, 1.0);
    locationLabel.font = [UIFont systemFontOfSize:16];
    
    navButton = [UIButton new];
    [bottomView addSubview:navButton];
    [navButton setImage:[UIImage imageNamed:@"btn_nav"] forState:UIControlStateNormal];
    navButton.sd_layout
    .widthIs(48)
    .heightIs(48)
    .centerYEqualToView(bottomView)
    .rightSpaceToView(bottomView, 20)
    ;
    [navButton addTarget:self action:@selector(startNav) forControlEvents:UIControlEventTouchUpInside];
}

-(void)startNav
{
    
    if (currentUserLocation) {
        [self openNativeNavi];
    }
    else {
        [SVProgressHUD show];
        [_locService startUserLocationService];
    }
    
}

- (void)openNativeNavi
{
    LCActionSheet *sheet = [LCActionSheet sheetWithTitle:nil buttonTitles:navapps redButtonIndex:-1 clicked:^(NSInteger buttonIndex) {
        if (buttonIndex < navapps.count) {
            if (buttonIndex == 0) {
                [self openApple];
            }
            else if (buttonIndex == 1) {
                if ([navapps[buttonIndex] containsString:@"百度"]) {
                    [self openBaidu];
                }
                else if ([navapps[buttonIndex] containsString:@"高德"]) {
                    [self openGaode];
                }
                else if ([navapps[buttonIndex] containsString:@"谷歌"]) {
                    [self openGoogle];
                }
            }
            else if (buttonIndex == 2) {
                if ([navapps[buttonIndex] containsString:@"高德"]) {
                    [self openGaode];
                }
                else if ([navapps[buttonIndex] containsString:@"谷歌"]) {
                    [self openGoogle];
                }
            }
            else if (buttonIndex == 3) {
                if ([navapps[buttonIndex] containsString:@"谷歌"]) {
                    [self openGoogle];
                }
            }
        }
    }];
    [sheet show];
    
}

-(void)openApple
{
    NSDictionary *addressDict = @{
                                  (__bridge NSString *) kABPersonAddressStreetKey : currentUserLocation.title
                                  };
    
    MKMapItem *currentLocation = [[MKMapItem alloc] initWithPlacemark:[[MKPlacemark alloc] initWithCoordinate:[self GCJ02FromBD09:currentUserLocation.location.coordinate] addressDictionary:addressDict]];
    
    NSDictionary *toaddressDict = @{
                                    (__bridge NSString *) kABPersonAddressStreetKey : self.address
                                    };
    MKMapItem *toLocation = [[MKMapItem alloc] initWithPlacemark:[[MKPlacemark alloc] initWithCoordinate:[self GCJ02FromBD09:self._pt] addressDictionary:toaddressDict]];
    
    [MKMapItem openMapsWithItems:@[currentLocation, toLocation]
                   launchOptions:@{MKLaunchOptionsDirectionsModeKey: MKLaunchOptionsDirectionsModeDriving,MKLaunchOptionsShowsTrafficKey: [NSNumber numberWithBool:YES]}];
}

-(void)openBaidu
{
    BMKNaviPara* para = [[BMKNaviPara alloc]init];
    BMKPlanNode* start = [[BMKPlanNode alloc]init];
    start.pt = currentUserLocation.location.coordinate;
    start.name = @"我的位置";
    para.startPoint = start;
    BMKPlanNode* end = [[BMKPlanNode alloc]init];
    end.pt = self._pt;
    end.name = _address;
    para.endPoint = end;
    para.appScheme = @"baidumapsdk://mapsdk.baidu.com";
    [BMKNavigation openBaiduMapNavigation:para];
}

-(void)openGaode
{
    CLLocationCoordinate2D gaode = [self GCJ02FromBD09:self._pt];
    NSString *urlString = [[NSString stringWithFormat:@"iosamap://navi?sourceApplication=%@&backScheme=%@&lat=%f&lon=%f&dev=0&style=2",@"1500米", @"happychat://", gaode.latitude, gaode.longitude] stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:urlString]];
}

-(void)openGoogle
{
    CLLocationCoordinate2D gaode = [self GCJ02FromBD09:self._pt];
    NSString *urlString = [[NSString stringWithFormat:@"comgooglemaps://?x-source=%@&x-success=%@&saddr=&daddr=%f,%f&directionsmode=driving", @"1500米", @"happychat://", gaode.latitude, gaode.longitude] stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:urlString]];
}

#pragma mark BMKLocationServiceDelegate
- (void)didUpdateUserHeading:(BMKUserLocation *)userLocation
{
    [SVProgressHUD dismiss];
    [_locService stopUserLocationService];
    currentUserLocation = userLocation;
    [self openNativeNavi];
}

- (BMKAnnotationView *)mapView:(BMKMapView *)mapView viewForAnnotation:(id <BMKAnnotation>)annotation
{
    if ([annotation isKindOfClass:[BMKPointAnnotation class]]) {
        BMKPinAnnotationView *newAnnotationView = [[BMKPinAnnotationView alloc] initWithAnnotation:annotation reuseIdentifier:@"pic_pin_search_MapModel"];
        newAnnotationView.pinColor = BMKPinAnnotationColorPurple;
        newAnnotationView.animatesDrop = YES;// 设置该标注点动画显示
        return newAnnotationView;
    }
    return nil;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (CLLocationCoordinate2D)GCJ02FromBD09:(CLLocationCoordinate2D)coor
{
    CLLocationDegrees x_pi = 3.14159265358979324 * 3000.0 / 180.0;
    CLLocationDegrees x = coor.longitude - 0.0065, y = coor.latitude - 0.006;
    CLLocationDegrees z = sqrt(x * x + y * y) - 0.00002 * sin(y * x_pi);
    CLLocationDegrees theta = atan2(y, x) - 0.000003 * cos(x * x_pi);
    CLLocationDegrees gg_lon = z * cos(theta);
    CLLocationDegrees gg_lat = z * sin(theta);
    return CLLocationCoordinate2DMake(gg_lat, gg_lon);
}

@end
