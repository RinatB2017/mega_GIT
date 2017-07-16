//
//  HLSlider.m
//  HomeLed
//
//  Created by Pavel Molodkin on 18.08.15.
//  Copyright © 2015 Voevoda's Incorporated. All rights reserved.
//

#import "HLSlider.h"

static CGFloat kHeightWidthButton = 35.f;

@interface HLSlider ()

@property(strong, nonatomic) IBOutlet UIButton *minusButton;
@property(strong, nonatomic) IBOutlet UIButton *plusButton;

@end

@implementation HLSlider

- (void)awakeFromNib
{
    self.slider = [UISlider new];
    self.minusButton = [UIButton new];
    self.plusButton = [UIButton new];

    [_minusButton setTitle:@"-" forState:UIControlStateNormal];
    [_minusButton setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [_plusButton setTitle:@"+" forState:UIControlStateNormal];
    [_plusButton setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    UIColor *backgroundColor = [UIColor colorWithRed:239.0 / 255.0 green:239.0 / 255.0 blue:239.0 / 255.0 alpha:1];
    _minusButton.backgroundColor = backgroundColor;
    _plusButton.backgroundColor = backgroundColor;
    _minusButton.layer.cornerRadius = 10.f;
    _plusButton.layer.cornerRadius = 10.f;

    [_minusButton addTarget:self action:@selector(tappedMinusButton:) forControlEvents:UIControlEventTouchUpInside];
    [_plusButton addTarget:self action:@selector(tappedPlusButton:) forControlEvents:UIControlEventTouchUpInside];
    [_slider addTarget:self action:@selector(valueChanged:) forControlEvents:UIControlEventValueChanged];
    [self addSubview:_slider];
    [self addSubview:_minusButton];
    [self addSubview:_plusButton];

    if (_bottomValueSlider == 0) {
        _bottomValueSlider = 255;
    }
}

- (void)layoutSubviews
{
    [self layoutMinusButton];
    [self layoutPlusButton];
    [self layoutSlider];
}

- (void)layoutMinusButton
{
    CGRect frame = self.frame;
    frame.origin.x = 0.f;
    frame.origin.y = 0.f;
    frame.size.height = kHeightWidthButton;
    frame.size.width = kHeightWidthButton;
    _minusButton.frame = frame;
}

- (void)layoutPlusButton
{
    CGRect frame = self.frame;
    frame.origin.x = _slider.width + _minusButton.width + 10.f;
    frame.origin.y = 0.f;
    frame.size.height = kHeightWidthButton;
    frame.size.width = kHeightWidthButton;
    _plusButton.frame = frame;
}

- (void)layoutSlider
{
    CGRect frame = self.frame;
    frame.origin.x = self.minusButton.width + 5.f;
    frame.origin.y = 0.f;
    frame.size.height = self.height;
    frame.size.width = self.width - self.minusButton.width - self.plusButton.width;
    _slider.frame = frame;
}

- (void)tappedMinusButton:(id)sender
{
    CGFloat top = 1;
    if (IsEqualFloat(_slider.value, 0)) {
        return;
    } else {
        _slider.value -= top / _bottomValueSlider;
        [self valueChanged:nil];
    }
}

- (void)tappedPlusButton:(id)sender
{
    CGFloat top = 1;
    if (IsEqualFloat(_slider.value, _bottomValueSlider)) {
        return;
    } else {
        _slider.value += top / _bottomValueSlider;
        [self valueChanged:nil];
    }
}

- (void)valueChanged:(id)sender
{
    if (_valueChangedBlock) {
        _valueChangedBlock();
    }
}

- (CGFloat)value { return _slider.value; }

- (void)setValue:(CGFloat)value { _slider.value = value; }

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
