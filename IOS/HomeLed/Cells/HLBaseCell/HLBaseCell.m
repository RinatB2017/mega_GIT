//
//  HLBaseCell.m
//  HomeLed
//
//  Created by ANDREI VAYAVODA on 06.08.15.
//  Copyright (c) 2015 Voevoda's Incorporated. All rights reserved.
//

#import "HLBaseCell.h"

@implementation HLBaseCell

+ (NSString *)reuseIdentifier {
    return NSStringFromClass(self);
}

- (void)awakeFromNib {
    // Initialization code
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

- (NSString *)reuseIdentifier {
    return [[self class] reuseIdentifier];
}

#pragma mark - Public
- (CGFloat)expandedHeight {
    return 44.f;
}


@end
