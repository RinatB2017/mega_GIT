//
//  HLSaveLoadColorsView.m
//  HomeLed
//
//  Created by Pavel Molodkin on 12.08.15.
//  Copyright © 2015 Voevoda's Incorporated. All rights reserved.
//

#import "HLSaveLoadColorsView.h"
#import "HLLoadColorsViewController.h"
#import "HLGeneralPaintViewController.h"

static CGFloat kXOffset = 2.f;

@interface HLSaveLoadColorsView ()

@property(strong, nonatomic) UIAlertAction *okAction;
@property(strong, nonatomic) IBOutlet UIButton *saveButton;
@property(strong, nonatomic) IBOutlet UIButton *loadButton;
@property(strong, nonatomic) NSArray *colorsArray;

@end

@implementation HLSaveLoadColorsView

- (void)awakeFromNib
{
    _colorsArray = [HLSettings shared].multiColorsList;
    _saveButton.layer.cornerRadius = 5.f;
    _loadButton.layer.cornerRadius = 5.f;
}

- (void)layoutSubviews
{
    [super layoutSubviews];
    [self layoutSaveButton];
    [self layoutLoadButton];
}

- (void)layoutSaveButton
{
    CGRect frame = _saveButton.frame;
    frame.origin.x = kXOffset;
    frame.size.width = self.bounds.size.width / 2 - kXOffset * 2;
    _saveButton.frame = frame;
}

- (void)layoutLoadButton
{
    CGRect frame = _loadButton.frame;
    frame.size.width = self.bounds.size.width / 2 - kXOffset * 2;
    frame.origin.x = self.bounds.size.width - kXOffset - frame.size.width;
    _loadButton.frame = frame;
}

- (IBAction)saveButtonTapped:(id)sender
{
    UIAlertController *alertController =
        [UIAlertController alertControllerWithTitle:@"Сохранить сборку цветов"
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
                                           NSMutableArray *savedColorsArray =
                                               [[HLSettings shared].savedColorsArray mutableCopy];
                                           if (!savedColorsArray) {
                                               savedColorsArray = [NSMutableArray new];
                                           }
                                           NSString *textName = alertController.textFields.firstObject.text;
                                           if (textName && [HLSettings shared].multiColorsList) {
                                               NSDictionary *descriptionDictionary = @{
                                                   @"name" : textName,
                                                   @"colorsArray" : [HLSettings shared].multiColorsList
                                               };
                                               [savedColorsArray insertObject:descriptionDictionary atIndex:0];
                                               [HLSettings shared].savedColorsArray = [savedColorsArray copy];
                                           }
                                       }];

    UIAlertAction *closeAction = [UIAlertAction actionWithTitle:NSLocalizedString(@"Close", @"Close action")
                                                          style:UIAlertActionStyleDefault
                                                        handler:^(UIAlertAction *action) {}];
    [alertController addAction:_okAction];
    [alertController addAction:closeAction];
    _okAction.enabled = NO;
    [_viewController presentViewController:alertController animated:YES completion:nil];
}

- (void)alertTextFieldDidChange:(UITextField *)textField { _okAction.enabled = textField.text.length > 0; }

- (IBAction)loadButtonTapped:(id)sender
{
    __weak typeof(self) wself = self;
    [HLLoadColorsViewController presentLoadColorsFromViewController:nil
                                                         completion:^(NSDictionary *dictionary) {
                                                             NSArray *loadColorsArray = nil;
                                                             if ([_colorsArray isKindOfClass:[NSArray class]]) {
                                                                 if ([dictionary isKindOfClass:[NSDictionary class]]) {
                                                                     loadColorsArray = dictionary[@"colorsArray"];
                                                                     [HLSettings shared].multiColorsList =
                                                                         loadColorsArray;
                                                                 }
                                                             }
                                                             if (wself.completionBlock) {
                                                                 wself.completionBlock(dictionary != nil);
                                                             }
                                                         }];
}

@end
