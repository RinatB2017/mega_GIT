//
//  HLColorLineView.h
//  HomeLed
//
//  Created by Pavel Molodkin on 14.08.15.
//  Copyright © 2015 Voevoda's Incorporated. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface HLColorLineView : UIView

@property(nonatomic, copy) void (^didChangeWidthColorBlock)(NSInteger widthValue);
@property(strong, nonatomic) IBOutlet UIButton *popoverWidthButton;

- (id)initWithColor:(UIColor *)color didChangeColorBlock:(void (^)(UIColor *color, BOOL finished))didChangeColorBlock;

@end
