//
//  HLPreviousColorsViewController.h
//  HomeLed
//
//  Created by Pavel Molodkin on 07.08.15.
//  Copyright © 2015 Voevoda's Incorporated. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "HLSelectColorViewController.h"

@interface HLPreviousColorsViewController : UIViewController

+ (instancetype) createWithCompletionBlock:(void(^)(UIColor *color))completionBlock;

@end
