//
//  HLLoadColorsViewController.m
//  HomeLed
//
//  Created by Pavel Molodkin on 13.08.15.
//  Copyright © 2015 Voevoda's Incorporated. All rights reserved.
//

#import "HLLoadColorsViewController.h"
#import "HLGeneralPaintViewController.h"
#import "HLColorsAnimationViewController.h"

@interface HLLoadColorsViewController ()

@property(strong, nonatomic) IBOutlet UITableView *tableView;
@property(strong, nonatomic) NSArray *colorsArray;
@property(strong, nonatomic) IBOutlet UIButton *cancelButton;
@property(copy, nonatomic) void (^completionBlock)(NSDictionary *dictionary);
@property(strong, nonatomic) UIViewController *presentViewController;

@end

@implementation HLLoadColorsViewController

#pragma mark - Initialization

+ (void)presentLoadColorsFromViewController:(UIViewController *)viewController
                                 completion:(void (^)(NSDictionary *dictionary))completionBlock
{
    HLLoadColorsViewController *loadViewController =
        [[HLLoadColorsViewController alloc] initWithNibName:@"HLLoadColorsViewController" bundle:nil];
    loadViewController.completionBlock = completionBlock;
    UINavigationController *navigationController =
        [[UINavigationController alloc] initWithRootViewController:loadViewController];
    if (!viewController) {
        viewController = [HLGeneralPaintViewController shared];
    }
    [viewController presentViewController:navigationController animated:YES completion:nil];
    loadViewController.presentViewController = viewController;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    _colorsArray = [HLSettings shared].savedColorsArray;
    self.navigationItem.rightBarButtonItem = self.editButtonItem;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)viewWillLayoutSubviews
{
    [super viewWillLayoutSubviews];
    [self layoutCancelButton];
}

- (void)layoutCancelButton
{
    CGRect frame = _cancelButton.frame;
    frame.origin.y = self.tableView.height + _cancelButton.height + 44.f;
    frame.origin.x = self.view.width / 2 - _cancelButton.width / 2;
    _cancelButton.frame = frame;
}

- (IBAction)finishButtonTapped:(id)sender { [self dismissViewControllerAnimated:YES completion:nil]; }

#pragma mark - UITableViewDataSource
- (NSInteger)tableView:(nonnull UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _colorsArray.count;
}

- (UITableViewCell *)tableView:(nonnull UITableView *)tableView cellForRowAtIndexPath:(nonnull NSIndexPath *)indexPath
{
    static NSString *cellIdentifier = @"loadColorsCell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier];
    if (!cell) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:cellIdentifier];
        cell.selectionStyle = UITableViewCellSelectionStyleGray;
    }
    if (_colorsArray.count > indexPath.row) {
        if ([_colorsArray isKindOfClass:[NSArray class]]) {
            NSDictionary *colorsDictionary = [_colorsArray objectAtIndex:indexPath.row];
            if ([colorsDictionary isKindOfClass:[NSDictionary class]]) {
                cell.textLabel.text = colorsDictionary[@"name"];
            }
        }
    }
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    NSDictionary *loadColorsDictionary = nil;
    if (_colorsArray.count > indexPath.row) {
        if ([_colorsArray isKindOfClass:[NSArray class]]) {
            loadColorsDictionary = [_colorsArray objectAtIndex:indexPath.row];
            __weak typeof(self) wself = self;
            [self.presentViewController dismissViewControllerAnimated:YES
                                                           completion:^{
                                                               if (wself.completionBlock) {
                                                                   wself.completionBlock(loadColorsDictionary);
                                                               }
                                                           }];
        }
    }
}

- (void)setEditing:(BOOL)editing animated:(BOOL)animated
{
    [super setEditing:editing animated:animated];
    [_tableView setEditing:editing animated:YES];
}

- (void)tableView:(UITableView *)tableView
    commitEditingStyle:(UITableViewCellEditingStyle)editingStyle
     forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        // add code here for when you hit delete
        NSMutableArray *savedColorsArray = [_colorsArray mutableCopy];
        [savedColorsArray removeObjectAtIndex:indexPath.row];
        [HLSettings shared].savedColorsArray = [savedColorsArray copy];
        _colorsArray = [HLSettings shared].savedColorsArray;
        [self.tableView beginUpdates];
        [self.tableView deleteRowsAtIndexPaths:[NSArray arrayWithObjects:indexPath, nil]
                              withRowAnimation:UITableViewRowAnimationFade];
        [self.tableView endUpdates];
    }
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little
preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
