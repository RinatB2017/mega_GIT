//
//  HLWelcomeViewController.m
//  HomeLed
//
//  Created by ANDREI VAYAVODA on 04.08.15.
//  Copyright (c) 2015 Voevoda's Incorporated. All rights reserved.
//

#import "HLGeneralPaintViewController.h"
#import "HLWelcomeViewController.h"
#import "HLRemoteClient.h"

@interface HLWelcomeViewController ()

@property (strong, nonatomic) IBOutlet UITextField *ipTextField;
@property (strong, nonatomic) IBOutlet UITextField *portTextField;
@property (strong, nonatomic) IBOutlet UILabel *redLabel;
@property (strong, nonatomic) IBOutlet UISlider *redSlider;
@property (strong, nonatomic) IBOutlet UILabel *greenLabel;
@property (strong, nonatomic) IBOutlet UISlider *greenSlider;
@property (strong, nonatomic) IBOutlet UILabel *blueLabel;
@property (strong, nonatomic) IBOutlet UISlider *blueSlider;

@property (assign, nonatomic) CGFloat red;
@property (assign, nonatomic) CGFloat green;
@property (assign, nonatomic) CGFloat blue;

@end

@implementation HLWelcomeViewController

- (void)viewDidLoad {
    self.red = 1.f;
    self.green = 0.5f;
    self.blue = 0.f;
    self.redSlider.value = self.red;
    self.greenSlider.value = self.green;
    self.blueSlider.value = self.blue;
    
    [super viewDidLoad];
    [self colorChanged];
    
    HLGeneralPaintViewController *viewController = [[HLGeneralPaintViewController alloc] initWithNibName:@"HLGeneralPaintViewController" bundle:nil];
    [self.navigationController pushViewController:viewController animated:NO];
    // Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
- (IBAction)doConnect:(id)sender {
    [self colorChanged];
}

#pragma mark - Private Methods
- (void)colorChanged {
    self.redLabel.text = [NSString stringWithFormat:@"Красный: %@", @(self.red)];
    self.greenLabel.text = [NSString stringWithFormat:@"Зеленый: %@", @(self.green)];
    self.blueLabel.text = [NSString stringWithFormat:@"Синий: %@", @(self.blue)];
    [HLRemoteClient setColor:[UIColor colorWithRed:self.red green:self.green blue:self.blue alpha:1.f]];
}

- (IBAction)redValueChanged:(UISlider *)sender {
    self.red = self.redSlider.value;
    [self colorChanged];
}
- (IBAction)greenValueChanged:(UISlider *)sender {
    self.green = self.greenSlider.value;
    [self colorChanged];
}
- (IBAction)blueValueChanged:(UISlider *)sender {
    self.blue = self.blueSlider.value;
    [self colorChanged];
}

@end
