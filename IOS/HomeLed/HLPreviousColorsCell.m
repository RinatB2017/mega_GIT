//
//  HLPreviousColorsCell.m
//  HomeLed
//
//  Created by Pavel Molodkin on 08.08.15.
//  Copyright © 2015 Voevoda's Incorporated. All rights reserved.
//

#import "HLPreviousColorsCell.h"

@interface HLPreviousColorsCell ()
@property (strong, nonatomic) IBOutlet UIView *colorView;
@end


@implementation HLPreviousColorsCell

- (void)awakeFromNib {
    [super awakeFromNib];
}

- (void)layoutSubviews {
    [super layoutSubviews];
    _colorView.width = self.width - 20.f;
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

- (void)fillWithColor:(UIColor *)color {
    _colorView.backgroundColor = color;
}


@end
