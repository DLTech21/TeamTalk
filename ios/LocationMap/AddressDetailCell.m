//
//  AddressDetailCell.m
//  SmallKitchen
//
//  Created by Alesary on 15/11/24.
//  Copyright © 2015年 Alesary. All rights reserved.
//

#import "AddressDetailCell.h"
#import "Place.h"
@interface AddressDetailCell ()

@property (strong, nonatomic) IBOutlet UILabel *name;

@property (strong, nonatomic) IBOutlet UILabel *address;

@end

@implementation AddressDetailCell

-(void)setPlace:(Place *)place
{

    self.name.text=place.name;
    self.address.text=place.address;
    
}

@end
