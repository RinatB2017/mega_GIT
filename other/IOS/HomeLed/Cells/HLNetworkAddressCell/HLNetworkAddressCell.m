//
//  HLNetworkAddressCell.m
//  HomeLed
//
//  Created by ANDREI VAYAVODA on 06.08.15.
//  Copyright (c) 2015 Voevoda's Incorporated. All rights reserved.
//

#import "HLNetworkAddressCell.h"

@interface HLNetworkAddressCell ()
@property (strong, nonatomic) IBOutlet UITextField *ipTextField;
@property (strong, nonatomic) IBOutlet UITextField *portTextField;

@end

@implementation HLNetworkAddressCell

- (void)awakeFromNib {
    self.disallowExpanding = YES;
    self.ipTextField.text = [HLSettings shared].ipAddress;
    self.portTextField.text = [[HLSettings shared].port stringValue];
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}
- (IBAction)textChanged:(id)sender {
    [HLSettings shared].ipAddress = self.ipTextField.text;
    [HLSettings shared].port = @([self.portTextField.text integerValue]);
}

@end
