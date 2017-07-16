//
//  HLPreviousColorsViewController.m
//  HomeLed
//
//  Created by Pavel Molodkin on 07.08.15.
//  Copyright © 2015 Voevoda's Incorporated. All rights reserved.
//

#import "HLPreviousColorsViewController.h"
#import "HLGeneralPaintViewController.h"
#import "HLPreviousColorsCell.h"
#import "HLRemoteClient.h"

@interface HLPreviousColorsViewController ()<UITableViewDelegate, UITableViewDataSource>
@property(copy, nonatomic) void (^completionBlock)(UIColor *color);
@property(strong, nonatomic) IBOutlet UIButton *finishButton;
@property(strong, nonatomic) IBOutlet UITableView *tableView;
@property(strong, nonatomic) NSArray *colorsArray;
@end

@implementation HLPreviousColorsViewController

+ (instancetype)createWithCompletionBlock:(void (^)(UIColor *color))completionBlock
{
    HLPreviousColorsViewController *viewController =
        [[HLPreviousColorsViewController alloc] initWithNibName:@"HLPreviousColorsViewController" bundle:nil];
    viewController.completionBlock = completionBlock;
    return viewController;
}

#pragma mark - Initialization

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.tableView.separatorColor = [UIColor clearColor];
    _colorsArray = [HLSettings shared].previousColorsList;
    [_tableView reloadData];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)viewWillLayoutSubviews
{
    [super viewWillLayoutSubviews];
    [self layoutFinishButton];
}

- (void)layoutFinishButton
{
    CGRect frame = _finishButton.frame;
    frame.origin.y = self.tableView.height + _finishButton.height + 20.f;
    frame.origin.x = self.view.width / 2 - _finishButton.width / 2;
    _finishButton.frame = frame;
}

- (IBAction)finishButtonTapped:(id)sender
{
    __weak typeof(self) wself = self;
    [self dismissViewControllerAnimated:YES
                             completion:^{
                                 if (wself.completionBlock) {
                                     wself.completionBlock(nil);
                                 }
                             }];
}

#pragma mark - UITableViewDataSource
- (NSInteger)tableView:(nonnull UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _colorsArray.count;
}

- (UITableViewCell *)tableView:(nonnull UITableView *)tableView cellForRowAtIndexPath:(nonnull NSIndexPath *)indexPath
{
    HLPreviousColorsCell *cell = [tableView dequeueReusableCellWithIdentifier:[HLPreviousColorsCell reuseIdentifier]];
    if (!cell) {
        cell = (HLPreviousColorsCell *)
               [[NSBundle mainBundle] loadNibNamed:@"HLPreviousColorsCell" owner:self options:nil].firstObject;
    }
    if (_colorsArray.count > indexPath.row) {
        UIColor *color = [_colorsArray objectAtIndex:indexPath.row];
        [cell fillWithColor:color];
    }
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    UIColor *color = nil;
    if (_colorsArray.count > indexPath.row) {
        color = [_colorsArray objectAtIndex:indexPath.row];
    }
    __weak typeof(self) wself = self;
    [self dismissViewControllerAnimated:YES
                             completion:^{
                                 if (wself.completionBlock) {
                                     wself.completionBlock(color);
                                 }
                             }];
}

#pragma mark - UITableViewDelegate

@end
