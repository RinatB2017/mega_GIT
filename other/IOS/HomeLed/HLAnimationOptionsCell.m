//
//  HLAnimationOptionsCell.m
//  HomeLed
//
//  Created by Pavel Molodkin on 28.08.15.
//  Copyright © 2015 Voevoda's Incorporated. All rights reserved.
//

#import "HLLoadColorsAnimationViewController.h"
#import "HLColorsAnimationViewController.h"
#import "HLGeneralPaintViewController.h"
#import "HLAnimationOptionsCell.h"
#import "HLRemoteClient.h"
#import "HLSlider.h"

static CGFloat kXOffset = 10.f;
static CGFloat kYOffset = 10.f;
static CGFloat kHeightCell = 40.f;
static CGFloat kSliderRange = 10.f;

@interface HLAnimationOptionsCell ()

@property(strong, nonatomic) IBOutlet UILabel *titleLabel;
@property(strong, nonatomic) IBOutlet UILabel *disabledAnimationLabel;
@property(strong, nonatomic) IBOutlet UISwitch *switchSettingsAnimation;
@property(strong, nonatomic) IBOutlet UILabel *speedAnimationLabel;
@property(strong, nonatomic) IBOutlet HLSlider *animationSpeedSlider;
@property(strong, nonatomic) IBOutlet UILabel *valueSpeedLabel;
@property(strong, nonatomic) IBOutlet UIView *view;
@property(strong, nonatomic) IBOutlet UIButton *colorsAnimationButton;
@property(strong, nonatomic) IBOutlet UIButton *saveColorsAnimationButton;
@property(strong, nonatomic) IBOutlet UIButton *loadColorsAnimationButton;
@property(strong, nonatomic) UIAlertAction *okAction;

@end

@implementation HLAnimationOptionsCell

#pragma mark - Initialization

- (void)awakeFromNib
{
    _colorsAnimationButton.layer.cornerRadius = 10.f;
    _saveColorsAnimationButton.layer.cornerRadius = 10.f;
    _loadColorsAnimationButton.layer.cornerRadius = 10.f;
    self.animationSpeedSlider.bottomValueSlider = 100.f;
    __weak typeof(self) wself = self;
    _animationSpeedSlider.valueChangedBlock = ^{ [wself sliderValueChanged:wself.animationSpeedSlider.value]; };
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

- (CGFloat)expandedHeight
{
    return _titleLabel.height + _switchSettingsAnimation.height + _speedAnimationLabel.height +
           _animationSpeedSlider.height + _valueSpeedLabel.height + _colorsAnimationButton.height +
           _saveColorsAnimationButton.height + kYOffset * 5;
}

#pragma mark - Layout methods

- (void)layoutSubviews
{
    [super layoutSubviews];
    [self titleLabelLayout];
    [self disabledAnimationLabelLayout];
    [self switchSettingsAnimationLayout];
    [self speedAnimationLabelLayout];
    [self animationSpeedSliderLayout];
    [self valueSpeedLabelLayout];
    [self colorosAnimationButtonLayout];
    [self saveColorsAnimationButtonLayout];
    [self loadColorsAnimationButtonLayout];
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

- (void)disabledAnimationLabelLayout
{
    CGRect frame = self.disabledAnimationLabel.frame;
    frame.size.width = self.bounds.size.width - _switchSettingsAnimation.width;
    frame.origin.x = 2 * kXOffset;
    frame.origin.y = _titleLabel.height + kYOffset;
    self.disabledAnimationLabel.frame = frame;
}

- (void)switchSettingsAnimationLayout
{
    CGRect frame = self.switchSettingsAnimation.frame;
    frame.origin.x = self.bounds.size.width - kXOffset - _switchSettingsAnimation.width;
    frame.origin.y = _titleLabel.height + kYOffset;
    self.switchSettingsAnimation.frame = frame;
}

- (void)speedAnimationLabelLayout
{
    CGRect frame = self.speedAnimationLabel.frame;
    frame.origin.x = (self.bounds.size.width - kXOffset - _speedAnimationLabel.width) / 2;
    frame.origin.y = _titleLabel.height + _switchSettingsAnimation.height + kYOffset;
    self.speedAnimationLabel.frame = frame;
}

- (void)animationSpeedSliderLayout
{
    CGRect frame = self.animationSpeedSlider.frame;
    frame.size.width = self.bounds.size.width - 3 * kXOffset;
    frame.origin.x = kXOffset;
    frame.origin.y = _titleLabel.height + _switchSettingsAnimation.height + _speedAnimationLabel.height + kYOffset;
    self.animationSpeedSlider.frame = frame;
}

- (void)valueSpeedLabelLayout
{
    CGRect frame = self.valueSpeedLabel.frame;
    frame.size.width = self.bounds.size.width / 2;
    frame.origin.x = (self.bounds.size.width - frame.size.width) / 2;
    frame.origin.y = _titleLabel.height + _switchSettingsAnimation.height + _speedAnimationLabel.height +
                     _valueSpeedLabel.height + kYOffset;
    self.valueSpeedLabel.frame = frame;
}

- (void)colorosAnimationButtonLayout
{
    CGRect frame = self.colorsAnimationButton.frame;
    frame.size.width = self.bounds.size.width / 2 + kXOffset * 2;
    frame.origin.x = (self.bounds.size.width - frame.size.width) / 2;
    frame.origin.y = _titleLabel.height + _switchSettingsAnimation.height + _speedAnimationLabel.height +
                     _valueSpeedLabel.height + kYOffset * 2 + _speedAnimationLabel.height;
    self.colorsAnimationButton.frame = frame;
}

- (void)saveColorsAnimationButtonLayout
{
    CGRect frame = self.saveColorsAnimationButton.frame;
    frame.size.width = self.bounds.size.width / 2 - kXOffset;
    frame.origin.x = kXOffset / 2;
    frame.origin.y = _titleLabel.height + _switchSettingsAnimation.height + _speedAnimationLabel.height +
                     _valueSpeedLabel.height + kYOffset * 3 + _speedAnimationLabel.height +
                     self.colorsAnimationButton.height;
    self.saveColorsAnimationButton.frame = frame;
}

- (void)loadColorsAnimationButtonLayout
{
    CGRect frame = self.loadColorsAnimationButton.frame;
    frame.size.width = self.bounds.size.width / 2 - kXOffset;
    frame.origin.x = self.bounds.size.width / 2 + kXOffset / 2;
    frame.origin.y = _titleLabel.height + _switchSettingsAnimation.height + _speedAnimationLabel.height +
                     _valueSpeedLabel.height + kYOffset * 3 + _speedAnimationLabel.height +
                     self.colorsAnimationButton.height;
    self.loadColorsAnimationButton.frame = frame;
}

#pragma mark - Expended cell methods

- (void)setExpandedMode:(BOOL)expandedMode
{
    [super setExpandedMode:expandedMode];
    _disabledAnimationLabel.hidden = !expandedMode;
    _switchSettingsAnimation.hidden = !expandedMode;
    _speedAnimationLabel.hidden = !expandedMode;
    _animationSpeedSlider.hidden = !expandedMode;
    _valueSpeedLabel.hidden = !expandedMode;
    _colorsAnimationButton.hidden = !expandedMode;
    _saveColorsAnimationButton.hidden = !expandedMode;
    _loadColorsAnimationButton.hidden = !expandedMode;
    if (expandedMode) {
        _animationSpeedSlider.value = [HLSettings shared].speedAnimation;
        [self sliderValueChanged:_animationSpeedSlider.value];
        _switchSettingsAnimation.on = [HLSettings shared].animationEnabled;
        _titleLabel.backgroundColor =
            [UIColor colorWithRed:239.f / 255.f green:239.f / 255.f blue:239.f / 255.f alpha:0.5];
    } else {
        _titleLabel.backgroundColor = [UIColor whiteColor];
    }
}

#pragma mark - Action methods

- (CGFloat)speedValue {
    CGFloat sliderValue = kSliderRange * _animationSpeedSlider.value;
    CGFloat rounded = sliderValue < 0.1f ? 0.1f : floorf(sliderValue * 10) / 10;
    rounded /= 10;
    return rounded;
}

- (void)sliderValueChanged:(CGFloat)value
{
    _valueSpeedLabel.text = [@([self speedValue]) stringValue];
    [HLSettings shared].speedAnimation = value;
    [self send];
}

- (IBAction)actionSwitchSettingsAnimation:(id)sender
{
    [HLSettings shared].animationEnabled = _switchSettingsAnimation.isOn;
    [self doSend];
}

- (void)send {
    [NSObject cancelPreviousPerformRequestsWithTarget:self selector:@selector(doSend) object:nil];
    [self performSelector:@selector(doSend) withObject:nil afterDelay:0.3];
}

- (void)doSend {
    [HLRemoteClient setAnimationEnabled:_switchSettingsAnimation.on speed:[self speedValue] toRight:YES];
}

- (IBAction)tappedColorsAnimationButton:(id)sender { [HLColorsAnimationViewController presentColorsAnimation]; }

- (IBAction)tappedSaveColorsAnimationButton:(id)sender
{
    UIAlertController *alertController =
        [UIAlertController alertControllerWithTitle:@"Сохранить сборку для анимации"
                                            message:@"Введите название сборки"
                                     preferredStyle:UIAlertControllerStyleAlert];

    [alertController addTextFieldWithConfigurationHandler:^(UITextField *textField) {
        textField.placeholder = NSLocalizedString(@"Name", @"NamePlaceholder");
        [textField addTarget:self
                      action:@selector(alertTextFieldDidChange:)
            forControlEvents:UIControlEventEditingChanged];
    }];
    _okAction = [UIAlertAction actionWithTitle:NSLocalizedString(@"OK", @"OK action")
                                         style:UIAlertActionStyleDefault
                                       handler:^(UIAlertAction *action) {
                                           NSMutableArray *savedColorsAnimationArray =
                                               [[HLSettings shared].savedColorsAnimationArray mutableCopy];
                                           if (!savedColorsAnimationArray) {
                                               savedColorsAnimationArray = [NSMutableArray new];
                                           }
                                           NSString *textName = alertController.textFields.firstObject.text;
                                           if (textName && [HLSettings shared].animationColorsArray) {
                                               NSDictionary *descriptionDictionary = @{
                                                   @"nameAnimation" : textName,
                                                   @"colorsAnimationArray" : [HLSettings shared].animationColorsArray
                                               };
                                               [savedColorsAnimationArray insertObject:descriptionDictionary atIndex:0];
                                               [HLSettings shared].savedColorsAnimationArray =
                                                   [savedColorsAnimationArray copy];
                                           }
                                       }];

    UIAlertAction *closeAction = [UIAlertAction actionWithTitle:NSLocalizedString(@"Close", @"Close action")
                                                          style:UIAlertActionStyleDefault
                                                        handler:^(UIAlertAction *action) {}];
    [alertController addAction:_okAction];
    [alertController addAction:closeAction];
    _okAction.enabled = NO;
    [[HLGeneralPaintViewController shared] presentViewController:alertController animated:YES completion:nil];
}

- (IBAction)tappedLoadColorsAnimationButton:(id)sender
{
    [HLLoadColorsAnimationViewController presentLoadColorsWithCompletion:^(NSDictionary *dictionary) {
        if (dictionary) {
            NSMutableArray *loadAnimationColorsArray = [NSMutableArray new];
            if ([dictionary isKindOfClass:[NSDictionary class]]) {
                loadAnimationColorsArray = dictionary[@"colorsAnimationArray"];
                [HLSettings shared].animationColorsArray = [loadAnimationColorsArray copy];
                [HLRemoteClient setAnimationColorsList:loadAnimationColorsArray speed:0.1 framesCount:0.1];
            }
        }
    }];
}

#pragma mark - Allert methods

- (void)alertTextFieldDidChange:(UITextField *)textField { _okAction.enabled = textField.text.length > 0; }

@end
