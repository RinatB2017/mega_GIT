//
//  HLColorPicker.h
//  HomeLed
//
//  Created by ANDREI VAYAVODA on 07.08.15.
//  Copyright (c) 2015 Voevoda's Incorporated. All rights reserved.
//

#import "NKOColorPickerView.h"
#import <UIKit/UIKit.h>

@interface HLColorPicker : UIView

- (CGFloat)desiredHeight;

@property(nonatomic, strong) NKOColorPickerDidChangeColorBlock didChangeColorBlock;
@property(readonly, nonatomic) NKOColorPickerView *colorPicker;

- (void)sliderValueChanged:(id)sender;

@end
