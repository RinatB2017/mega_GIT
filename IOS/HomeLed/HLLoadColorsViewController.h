//
//  HLLoadColorsViewController.h
//  HomeLed
//
//  Created by Pavel Molodkin on 13.08.15.
//  Copyright © 2015 Voevoda's Incorporated. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface HLLoadColorsViewController : UIViewController

+ (void)presentLoadColorsFromViewController:(UIViewController *)viewController
                                 completion:(void (^)(NSDictionary *dictionary))completionBlock;

@end
