//
//  HLSlider.h
//  HomeLed
//
//  Created by Pavel Molodkin on 18.08.15.
//  Copyright © 2015 Voevoda's Incorporated. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "HLWidthColorViewController.h"

@interface HLSlider : UIView

@property(copy, nonatomic) dispatch_block_t valueChangedBlock;
@property(strong, nonatomic) IBOutlet UISlider *slider;
@property(assign, nonatomic) CGFloat value;
@property(assign, nonatomic) CGFloat bottomValueSlider;

@end
