//
//  HLGeneralPaintViewController.m
//  HomeLed
//
//  Created by ANDREI VAYAVODA on 06.08.15.
//  Copyright (c) 2015 Voevoda's Incorporated. All rights reserved.
//

#import "HLGeneralPaintTableDataSource.h"
#import "HLGeneralPaintViewController.h"

@interface HLGeneralPaintViewController ()

@property (strong, nonatomic) IBOutlet HLGeneralPaintTableDataSource *dataSource;
@property (strong, nonatomic) IBOutlet UITableView *tableView;
@end

@implementation HLGeneralPaintViewController

static HLGeneralPaintViewController *sharedInstance = nil;

+ (instancetype)shared {
    return sharedInstance;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    sharedInstance = self;
    // Do any additional setup after loading the view from its nib.
}

- (void)viewWillLayoutSubviews {
    [super viewWillLayoutSubviews];
    CGRect frame = self.view.bounds;
    frame.origin.y = 20.f;
    _tableView.frame = frame;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
