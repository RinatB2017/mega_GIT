//
//  HLSettings.h
//  HomeLed
//
//  Created by ANDREI VAYAVODA on 06.08.15.
//  Copyright (c) 2015 Voevoda's Incorporated. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface HLSettings : NSObject

@property(strong, nonatomic) NSString *ipAddress;
@property(strong, nonatomic) NSNumber *port;
@property(strong, nonatomic) UIColor *entireStripColor;
@property(strong, nonatomic) NSArray *multiColorsList;
@property(strong, nonatomic) NSArray *previousColorsList;
@property(strong, nonatomic) NSArray *savedColorsArray;
@property(strong, nonatomic) NSArray *animationColorsArray;
@property(strong, nonatomic) NSArray *savedColorsAnimationArray;
@property(assign, nonatomic) CGFloat brightnessValue;
@property(assign, nonatomic) CGFloat speedAnimation;
@property(assign, nonatomic) BOOL animationEnabled;

+ (instancetype)shared;

@end
