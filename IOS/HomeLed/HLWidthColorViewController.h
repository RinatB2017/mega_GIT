//
//  HLWidthColorViewController.h
//  HomeLed
//
//  Created by Pavel Molodkin on 22.08.15.
//  Copyright © 2015 Voevoda's Incorporated. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface HLWidthColorViewController : UIViewController

@property(nonatomic, copy) void (^didChangeSliderBlock)(NSInteger widthValue);

+ (instancetype)createWithInitialValue:(CGFloat)value;

@end
