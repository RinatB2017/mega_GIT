//
//  HLSettingsCell.m
//  HomeLed
//
//  Created by Pavel Molodkin on 01.09.15.
//  Copyright © 2015 Voevoda's Incorporated. All rights reserved.
//

#import "HLSettingsCell.h"
#import "HLSlider.h"

static CGFloat kXOffset = 10.f;
static CGFloat kYOffset = 10.f;
static CGFloat kHeightCell = 40.f;
static CGFloat kSliderRange = 100.f;

@interface HLSettingsCell ()

@property(strong, nonatomic) IBOutlet UILabel *titleLabel;
@property(strong, nonatomic) IBOutlet HLSlider *brightnessSlider;
@property(strong, nonatomic) IBOutlet UILabel *brightnessLabel;
@property(strong, nonatomic) IBOutlet UILabel *valueSettingsSliderLabel;

@end

@implementation HLSettingsCell

- (void)awakeFromNib
{
    self.brightnessSlider.bottomValueSlider = kSliderRange;
    __weak typeof(self) wself = self;
    _brightnessSlider.valueChangedBlock = ^{ [wself sliderValueChanged:wself.brightnessSlider.value]; };
}

- (void)layoutSubviews
{
    [super layoutSubviews];
    [self titleLabelLayout];
    [self brightnessLabelLayout];
    [self brightnessSliderLayout];
}

- (void)titleLabelLayout
{
    CGRect frame = self.titleLabel.frame;
    frame.size.width = self.bounds.size.width;
    frame.size.height = kHeightCell;
    frame.origin.x = 0.f;
    frame.origin.y = 0.f;
    self.titleLabel.frame = frame;
}

- (void)brightnessLabelLayout
{
    CGRect frame = self.brightnessLabel.frame;
    frame.size.width = self.bounds.size.width;
    frame.origin.x = 0.f;
    frame.origin.y = kHeightCell + kYOffset;
    self.brightnessLabel.frame = frame;
}

- (void)brightnessSliderLayout
{
    CGRect frame = self.brightnessSlider.frame;
    frame.size.width = self.bounds.size.width - kXOffset * 3;
    frame.origin.x = kXOffset;
    frame.origin.y = kHeightCell + _brightnessLabel.height + kYOffset;
    self.brightnessSlider.frame = frame;
}

- (void)valueSettingsSliderLabelLayout
{
    CGRect frame = self.valueSettingsSliderLabel.frame;
    frame.size.width = self.bounds.size.width / 2;
    frame.origin.x = kXOffset;
    frame.origin.y = kHeightCell + _brightnessLabel.height + _brightnessSlider.height + kYOffset * 2;
    self.valueSettingsSliderLabel.frame = frame;
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

- (CGFloat)expandedHeight
{
    return kHeightCell + _brightnessLabel.height + _brightnessSlider.height + _valueSettingsSliderLabel.height +
           kYOffset * 4;
}

- (void)sliderValueChanged:(CGFloat)value
{
    _valueSettingsSliderLabel.text = [@(@(kSliderRange * _brightnessSlider.value).integerValue) stringValue];
    [HLSettings shared].brightnessValue = value;
}

- (void)setExpandedMode:(BOOL)expandedMode
{
    [super setExpandedMode:expandedMode];
    _brightnessLabel.hidden = !expandedMode;
    _brightnessSlider.hidden = !expandedMode;
    _valueSettingsSliderLabel.hidden = !expandedMode;
    if (expandedMode) {
        _brightnessSlider.value = [HLSettings shared].brightnessValue;
        [self sliderValueChanged:_brightnessSlider.value];
        _titleLabel.backgroundColor =
            [UIColor colorWithRed:239.f / 255.f green:239.f / 255.f blue:239.f / 255.f alpha:0.5];
    } else {
        _titleLabel.backgroundColor = [UIColor whiteColor];
    }
}

@end
