//
//  HLWidthColorViewController.m
//  HomeLed
//
//  Created by Pavel Molodkin on 22.08.15.
//  Copyright © 2015 Voevoda's Incorporated. All rights reserved.
//

#import "HLWidthColorViewController.h"
#import "HLGeneralPaintViewController.h"
#import "HLSlider.h"

static CGFloat kWidthPopover = 0.9f;
static CGFloat kXOffset = 10.f;
static CGFloat kYOffset = 10.f;
static CGFloat kSliderRange = 50.f;

@interface HLWidthColorViewController ()

@property(strong, nonatomic) IBOutlet HLSlider *slider;
@property(strong, nonatomic) IBOutlet UILabel *sliderCountLabel;
@property(strong, nonatomic) IBOutlet UIButton *finishButton;

@end

@implementation HLWidthColorViewController

+ (instancetype)createWithInitialValue:(CGFloat)value
{
    HLWidthColorViewController *viewController =
        [[HLWidthColorViewController alloc] initWithNibName:@"HLWidthColorViewController" bundle:nil];
    [viewController view];
    viewController.slider.bottomValueSlider = kSliderRange;
    viewController.slider.slider.minimumValue = 1.f / kSliderRange;
    viewController.slider.value = (float)value / kSliderRange;
    [viewController sliderValueChanged:nil];
    return viewController;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    [self layoutSubview];
    __weak typeof(self) wself = self;
    _slider.valueChangedBlock = ^{ [wself sliderValueChanged:nil]; };
}

#pragma mark - Observers
- (void)sliderValueChanged:(id)sender
{
    NSInteger sliderValue = @(kSliderRange * _slider.value).integerValue;
    _sliderCountLabel.text = [@(sliderValue) stringValue];
    if (_didChangeSliderBlock) {
        _didChangeSliderBlock(sliderValue);
    }
}

- (void)layoutSubview
{
    [self layoutSlider];
    [self layoutSliderCountLabel];
}

- (void)layoutSlider
{
    CGRect frame = self.slider.frame;
    frame.size.width = self.view.bounds.size.width * kWidthPopover - 2 * kXOffset;
    frame.origin.x = kXOffset;
    frame.origin.y = self.view.height / 2 - self.slider.height / 2 - kYOffset;
    self.slider.frame = frame;
}

- (void)layoutSliderCountLabel
{
    CGRect frame = self.sliderCountLabel.frame;
    frame.origin.x = self.view.width / 2 - self.sliderCountLabel.width / 2 + kXOffset;
    frame.origin.y = self.view.height - _slider.height;
    self.sliderCountLabel.frame = frame;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
