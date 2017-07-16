//
//  HLLoadColorsAnimationViewController.h
//  HomeLed
//
//  Created by Pavel Molodkin on 04.09.15.
//  Copyright © 2015 Voevoda's Incorporated. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface HLLoadColorsAnimationViewController : UIViewController

+ (void)presentLoadColorsWithCompletion:(void (^)(NSDictionary *dictionary))completionBlock;

@end
