//
//  HLSelectColorViewController.h
//  HomeLed
//
//  Created by ANDREI VAYAVODA on 07.08.15.
//  Copyright (c) 2015 Voevoda's Incorporated. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "HLColorPicker.h"


@interface HLSelectColorViewController : UIViewController

+ (void)presentWithColor:(UIColor *)color completion:(void(^)(UIColor *color, BOOL finished))completionBlock;

@end
