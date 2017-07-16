//
//  HLLoadColorsAnimationViewController.m
//  HomeLed
//
//  Created by Pavel Molodkin on 04.09.15.
//  Copyright © 2015 Voevoda's Incorporated. All rights reserved.
//

#import "HLLoadColorsAnimationViewController.h"
#import "HLGeneralPaintViewController.h"

@interface HLLoadColorsAnimationViewController ()

@property(strong, nonatomic) IBOutlet UITableView *tableView;
@property(copy, nonatomic) void (^completionBlock)(NSDictionary *dictionary);
@property(strong, nonatomic) NSArray *colorsAnimationArray;

@end

@implementation HLLoadColorsAnimationViewController

#pragma mark - Initialization

+ (void)presentLoadColorsWithCompletion:(void (^)(NSDictionary *dictionary))completionBlock
{
    HLLoadColorsAnimationViewController *loadAnimationViewController =
        [[HLLoadColorsAnimationViewController alloc] initWithNibName:@"HLLoadColorsAnimationViewController" bundle:nil];
    loadAnimationViewController.completionBlock = completionBlock;
    UINavigationController *navigationController =
        [[UINavigationController alloc] initWithRootViewController:loadAnimationViewController];
    [[HLGeneralPaintViewController shared] presentViewController:navigationController animated:YES completion:nil];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    _colorsAnimationArray = [HLSettings shared].savedColorsAnimationArray;
    self.navigationItem.rightBarButtonItem = self.editButtonItem;
    UIBarButtonItem *backButton = [[UIBarButtonItem alloc] initWithTitle:@"Back"
                                                                   style:UIBarButtonItemStylePlain
                                                                  target:self
                                                                  action:@selector(tappedBackButton:)];

    self.navigationItem.leftBarButtonItem = backButton;
}

- (void)tappedBackButton:(id)sender { [self dismissViewControllerAnimated:YES completion:nil]; }

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - UITableViewDataSource
- (NSInteger)tableView:(nonnull UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _colorsAnimationArray.count;
}

- (UITableViewCell *)tableView:(nonnull UITableView *)tableView cellForRowAtIndexPath:(nonnull NSIndexPath *)indexPath
{
    static NSString *cellIdentifier = @"loadColorsAnimationCell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier];
    if (!cell) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:cellIdentifier];
        cell.selectionStyle = UITableViewCellSelectionStyleGray;
    }
    if (_colorsAnimationArray.count > indexPath.row) {
        if ([_colorsAnimationArray isKindOfClass:[NSArray class]]) {
            NSDictionary *colorsDictionary = [_colorsAnimationArray objectAtIndex:indexPath.row];
            if ([colorsDictionary isKindOfClass:[NSDictionary class]]) {
                cell.textLabel.text = colorsDictionary[@"nameAnimation"];
            }
        }
    }
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    NSDictionary *loadColorsAnimationDictionary = nil;
    if (_colorsAnimationArray.count > indexPath.row) {
        if ([_colorsAnimationArray isKindOfClass:[NSArray class]]) {
            loadColorsAnimationDictionary = [_colorsAnimationArray objectAtIndex:indexPath.row];
            __weak typeof(self) wself = self;
            [[HLGeneralPaintViewController shared] dismissViewControllerAnimated:YES
                                                                      completion:^{
                                                                          if (wself.completionBlock) {
                                                                              wself.completionBlock(
                                                                                  loadColorsAnimationDictionary);
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
        NSMutableArray *savedColorsAnimationArray = [_colorsAnimationArray mutableCopy];
        [savedColorsAnimationArray removeObjectAtIndex:indexPath.row];
        [HLSettings shared].savedColorsAnimationArray = [savedColorsAnimationArray copy];
        _colorsAnimationArray = [HLSettings shared].savedColorsAnimationArray;
        [self.tableView beginUpdates];
        [self.tableView deleteRowsAtIndexPaths:[NSArray arrayWithObjects:indexPath, nil]
                              withRowAnimation:UITableViewRowAnimationFade];
        [self.tableView endUpdates];
    }
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
