//
//  HLSaveLoadColorsView.h
//  HomeLed
//
//  Created by Pavel Molodkin on 12.08.15.
//  Copyright © 2015 Voevoda's Incorporated. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface HLSaveLoadColorsView : UIView

@property(weak, nonatomic) UIViewController *viewController;
@property(copy, nonatomic) void (^completionBlock)(BOOL finished);

@end
