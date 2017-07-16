//
//  HLChangeColorCell.m
//  HomeLed
//
//  Created by ANDREI VAYAVODA on 06.08.15.
//  Copyright (c) 2015 Voevoda's Incorporated. All rights reserved.
//

#import "NKOColorPickerView.h"
#import "HLChangeColorCell.h"
#import "HLRemoteClient.h"
#import "HLColorPicker.h"

@interface HLChangeColorCell ()

@property (strong, nonatomic) IBOutlet HLColorPicker *colorPicker;
@property (strong, nonatomic) IBOutlet UILabel *titleLabel;

@property (assign, nonatomic) BOOL scrollingDisabled;

@end

@implementation HLChangeColorCell

#pragma mark - Initialization
- (void)awakeFromNib {
    [super awakeFromNib];
    _colorPicker = [[NSBundle mainBundle] loadNibNamed:@"HLColorPicker" owner:self options:nil].firstObject;
    [self.contentView addSubview:_colorPicker];
    _colorPicker.hidden = YES;
    __weak typeof (self) wself = self;
    _colorPicker.colorPicker.color = [HLSettings shared].entireStripColor;
    
    _colorPicker.colorPicker.touchesBlock = ^(BOOL touchesEnded) {
        if (touchesEnded) {
            if (wself.scrollingDisabled) {
                wself.scrollingDisabled = NO;
                UITableView *tableView = (UITableView *)wself.superview.superview;
                tableView.scrollEnabled = YES;
            }
        }
        else {
            wself.scrollingDisabled = YES;
            UITableView *tableView = (UITableView *)wself.superview.superview;
            tableView.scrollEnabled = NO;
        }
    };
    _colorPicker.didChangeColorBlock = ^(UIColor *color) {
        [HLSettings shared].entireStripColor = color;
        [HLRemoteClient setColor:color];
    };
}

#pragma mark - UIView
- (void)layoutSubviews {
    [super layoutSubviews];
    [self layoutTitleLabel];
    [self layoutColorPicker];
}

- (void)layoutTitleLabel {
    CGRect frame = self.bounds;
    frame.origin.x = 10.f;
    frame.size.height = 44.f;
    _titleLabel.frame = frame;
}

- (void)layoutColorPicker {
    CGRect frame = self.bounds;
    frame.origin.y = _titleLabel.y + _titleLabel.height;
    frame.size.height = [_colorPicker desiredHeight];
    _colorPicker.frame = frame;
}

#pragma mark - Overriden
- (void)setExpandedMode:(BOOL)expandedMode {
    [super setExpandedMode:expandedMode];
    _colorPicker.hidden = !expandedMode;
    if (expandedMode) {
        [HLRemoteClient setColor:[HLSettings shared].entireStripColor];
    }
}

#pragma mark - Public
- (CGFloat)expandedHeight {
    return _colorPicker.y + _colorPicker.height;
}

@end
