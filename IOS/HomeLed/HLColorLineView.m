//
//  HLColorLineView.m
//  HomeLed
//
//  Created by Pavel Molodkin on 14.08.15.
//  Copyright © 2015 Voevoda's Incorporated. All rights reserved.
//

#import "HLGeneralPaintViewController.h"
#import "HLSelectColorViewController.h"
#import "HLWidthColorViewController.h"
#import "HLColorLineView.h"
#import "HLSlider.h"

static CGFloat kXOffset = 10.f;

@interface HLColorLineView ()

@property(nonatomic, copy) void (^didChangeColorBlock)(UIColor *color, BOOL finished);
@property(strong, nonatomic) IBOutlet UIButton *colorButton;
@property(strong, nonatomic) IBOutlet UIButton *removeColorButton;

@end

@implementation HLColorLineView

- (id)initWithColor:(UIColor *)color didChangeColorBlock:(void (^)(UIColor *color, BOOL finished))didChangeColorBlock
{
    if (self = [super init]) {
        HLColorLineView *colorLineView =
            (HLColorLineView *)
            [[NSBundle mainBundle] loadNibNamed:@"HLColorLineView" owner:self options:nil].firstObject;
        self = colorLineView;
        self.didChangeColorBlock = didChangeColorBlock;
        CGFloat red = 0.0, green = 0.0, blue = 0.0, alpha = 0.0;
        [color getRed:&red green:&green blue:&blue alpha:&alpha];
        CGFloat multiplier = 1.f / MAX(red, MAX(green, blue));
        red = red * multiplier;
        green = green * multiplier;
        blue = blue * multiplier;
        UIColor *colorBackground = [UIColor colorWithRed:red green:green blue:blue alpha:alpha];
        _colorButton.backgroundColor = colorBackground;
        _colorButton.layer.cornerRadius = 3.f;
    }
    return self;
}

- (void)layoutSubviews
{
    [super layoutSubviews];
    [self layoutRemoveColorButton];
    [self layoutPopoverWidthButton];
}

- (void)layoutRemoveColorButton
{
    CGRect frame = _removeColorButton.frame;
    frame.origin.x = self.bounds.size.width - _removeColorButton.width;
    _removeColorButton.frame = frame;
}

- (void)layoutPopoverWidthButton
{
    CGRect frame = _popoverWidthButton.frame;
    frame.origin.x = self.bounds.size.width / 2 - kXOffset;
    _popoverWidthButton.frame = frame;
}

- (IBAction)colorButtonTapped:(id)sender
{
    UIButton *colorButton = self.subviews.firstObject;
    __weak typeof(self) wself = self;
    [HLSelectColorViewController presentWithColor:colorButton.backgroundColor
                                       completion:^(UIColor *color, BOOL finished) {
                                           colorButton.backgroundColor = color;
                                           if (wself.didChangeColorBlock) {
                                               wself.didChangeColorBlock(color, finished);
                                           }
                                       }];
}

- (IBAction)removeButtonTapped:(id)sender
{
    if (self.didChangeColorBlock) {
        self.didChangeColorBlock(nil, YES);
    }
}
- (IBAction)showPopover:(id)sender
{
    NSString *valueSliderString = [_popoverWidthButton.titleLabel.text substringFromIndex:6];
    HLWidthColorViewController *sliderPopover =
        [HLWidthColorViewController createWithInitialValue:[valueSliderString floatValue]];
    __weak typeof(self) wself = self;
    sliderPopover.didChangeSliderBlock = ^(NSInteger widthValue) {
        if (wself.didChangeWidthColorBlock) {
            wself.didChangeWidthColorBlock(widthValue);
        }
    };
    UINavigationController *navigationController =
        [[UINavigationController alloc] initWithRootViewController:sliderPopover];
    sliderPopover.preferredContentSize = CGSizeMake((self.bounds.size.width * 0.9f), 50);
    navigationController.modalPresentationStyle = UIModalPresentationPopover;
    UIPopoverPresentationController *popoverPresentationController = nil;
    popoverPresentationController = navigationController.popoverPresentationController;
    popoverPresentationController.delegate = self;
    popoverPresentationController.sourceView = self;
    popoverPresentationController.sourceRect = [sender frame];
    navigationController.modalPresentationStyle = UIModalPresentationPopover;
    navigationController.navigationBarHidden = YES;
    [[HLGeneralPaintViewController shared] presentViewController:navigationController animated:YES completion:nil];
}

#pragma mark - Private methods

- (UIModalPresentationStyle)adaptivePresentationStyleForPresentationController:(UIPresentationController *)controller
{
    return UIModalPresentationNone;
}

@end
