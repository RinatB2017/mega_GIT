//
//  HLColorPicker.m
//  HomeLed
//
//  Created by ANDREI VAYAVODA on 07.08.15.
//  Copyright (c) 2015 Voevoda's Incorporated. All rights reserved.
//

#import "HLColorPicker.h"
#import "HLSlider.h"

static CGFloat kSliderYOffset = 40.f;
static CGFloat kXOffset = 10.f;
static CGFloat kYDelta = 10.f;

@interface HLColorPicker ()

@property(strong, nonatomic) IBOutlet NKOColorPickerView *colorPicker;
@property(strong, nonatomic) IBOutlet UILabel *redLabel;
@property(strong, nonatomic) IBOutlet UILabel *greenLabel;
@property(strong, nonatomic) IBOutlet UILabel *blueLabel;
@property(strong, nonatomic) IBOutlet HLSlider *redSlider;
@property(strong, nonatomic) IBOutlet HLSlider *greenSlider;
@property(strong, nonatomic) IBOutlet HLSlider *blueSlider;

@end

@implementation HLColorPicker

#pragma mark - Initialization
- (void)awakeFromNib
{
    [super awakeFromNib];
    __weak typeof(self) wself = self;
    _colorPicker.didChangeColorBlock = ^(UIColor *color) {
        [wself updateSlidersWithColor:color];
        if (wself.didChangeColorBlock) {
            wself.didChangeColorBlock(color);
        }
    };
    dispatch_block_t block = ^{ [wself sliderValueChanged:nil]; };
    _redSlider.valueChangedBlock = block;
    _greenSlider.valueChangedBlock = block;
    _blueSlider.valueChangedBlock = block;
}

#pragma mark - Observers
- (void)sliderValueChanged:(id)sender
{
    UIColor *color = [UIColor colorWithRed:_redSlider.value green:_greenSlider.value blue:_blueSlider.value alpha:1.f];
    _colorPicker.color = color;
}

#pragma mark - UIView
- (void)layoutSubviews
{
    [super layoutSubviews];
    [self layoutColorPicker];
    [self layoutLabels];
    [self layoutSliders];
}

- (void)layoutColorPicker
{
    CGRect frame = self.bounds;
    frame.size.height = 280.f;
    _colorPicker.frame = frame;
}

- (void)layoutLabels
{
    if (!_redLabel) {
        return;
    }
    CGRect frame = self.bounds;
    frame.origin.x = kXOffset;
    frame.origin.y = _colorPicker.y + _colorPicker.height + kYDelta;
    frame.size.width = 35.f;
    frame.size.height = _redSlider.height;
    for (UILabel *label in @[ _redLabel, _greenLabel, _blueLabel ]) {
        label.frame = frame;
        frame.origin.y += kSliderYOffset;
    }
}

- (void)layoutSliders
{
    if (!_redSlider) {
        return;
    }
    CGRect frame = self.bounds;
    frame.origin.y = _colorPicker.y + _colorPicker.height + kYDelta;
    for (NSArray *array in
         @[ @[ _redSlider, _redLabel ], @[ _greenSlider, _greenLabel ], @[ _blueSlider, _blueLabel ] ]) {
        UISlider *slider = array.firstObject;
        UILabel *label = array.lastObject;
        frame.origin.x = label.x + label.width + kXOffset;
        frame.size.width = self.width - kXOffset * 2 - frame.origin.x;
        frame.size.height = slider.height;
        slider.frame = frame;
        frame.origin.y += kSliderYOffset;
    }
}

#pragma mark - Public Methods
- (CGFloat)desiredHeight { return _blueSlider.y + _blueSlider.height + kYDelta; }

#pragma mark - Private Methods
- (void)updateSlidersWithColor:(UIColor *)color
{
    CGFloat red, green, blue;
    [color getRed:&red green:&green blue:&blue alpha:nil];
    _redSlider.value = red;
    _greenSlider.value = green;
    _blueSlider.value = blue;

    _redLabel.text = [@(@(255 * red).integerValue) stringValue];
    _greenLabel.text = [@(@(255 * green).integerValue) stringValue];
    _blueLabel.text = [@(@(255 * blue).integerValue) stringValue];
}

@end
