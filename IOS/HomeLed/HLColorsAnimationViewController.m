//
//  HLColorsAnimationViewController.m
//  HomeLed
//
//  Created by Pavel Molodkin on 02.09.15.
//  Copyright © 2015 Voevoda's Incorporated. All rights reserved.
//

#import "HLColorsAnimationViewController.h"
#import "HLGeneralPaintViewController.h"
#import "HLLoadColorsViewController.h"

static const CGFloat kYOffset = 15.f;

@interface HLColorsAnimationViewController ()

@property(strong, nonatomic) IBOutlet UITableView *tableView;
@property(strong, nonatomic) IBOutlet UIButton *addSavesColorsButton;
@property(strong, nonatomic) NSArray *colorsArray;

@end

@implementation HLColorsAnimationViewController

+ (void)presentColorsAnimation
{
    HLColorsAnimationViewController *viewController =
        [[HLColorsAnimationViewController alloc] initWithNibName:@"HLColorsAnimationViewController" bundle:nil];
    UINavigationController *navigationController =
        [[UINavigationController alloc] initWithRootViewController:viewController];
    [[HLGeneralPaintViewController shared] presentViewController:navigationController animated:YES completion:nil];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.navigationItem.rightBarButtonItem = self.editButtonItem;
    UIBarButtonItem *closeButton = [[UIBarButtonItem alloc] initWithTitle:@"Back"
                                                                    style:UIBarButtonItemStylePlain
                                                                   target:self
                                                                   action:@selector(tappedCloseButton:)];

    self.navigationItem.leftBarButtonItem = closeButton;
    _addSavesColorsButton.layer.cornerRadius = 10.f;
    // Do any additional setup after loading the view from its nib.
}

- (void)viewWillAppear:(BOOL)animated { _colorsArray = [HLSettings shared].animationColorsArray; }

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)viewWillLayoutSubviews
{
    [super viewWillLayoutSubviews];
    [self addSavesColorsButtonLayout];
    _tableView.frame = CGRectMake(0, 0, self.view.bounds.size.width,
                                  self.view.bounds.size.height - 2 * kYOffset - _addSavesColorsButton.height);
}

- (void)addSavesColorsButtonLayout
{
    CGRect frame = _addSavesColorsButton.frame;
    frame.origin.x = self.view.bounds.size.width / 2 - frame.size.width / 2;
    frame.origin.y = self.view.bounds.size.height - kYOffset - frame.size.height;
    _addSavesColorsButton.frame = frame;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section { return _colorsArray.count; }

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *cellIdentifier = @"animationColorsCell";
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

- (void)tableView:(UITableView *)tableView
    moveRowAtIndexPath:(NSIndexPath *)fromIndexPath
           toIndexPath:(NSIndexPath *)toIndexPath
{
    if (fromIndexPath != toIndexPath) {
        NSMutableArray *colorsArray = [[HLSettings shared].animationColorsArray mutableCopy];
        NSDictionary *dictionary = [colorsArray objectAtIndex:fromIndexPath.row];
        [tableView beginUpdates];
        [colorsArray removeObjectAtIndex:fromIndexPath.row];
        [colorsArray insertObject:dictionary atIndex:toIndexPath.row];
        [tableView moveRowAtIndexPath:fromIndexPath toIndexPath:toIndexPath];
        [self.tableView endUpdates];
        [HLSettings shared].animationColorsArray = [colorsArray copy];
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
        NSMutableArray *savedAnimationColorsArray = [_colorsArray mutableCopy];
        [savedAnimationColorsArray removeObjectAtIndex:indexPath.row];
        [HLSettings shared].animationColorsArray = [savedAnimationColorsArray copy];
        _colorsArray = [HLSettings shared].animationColorsArray;
        [self.tableView beginUpdates];
        [self.tableView deleteRowsAtIndexPaths:[NSArray arrayWithObjects:indexPath, nil]
                              withRowAnimation:UITableViewRowAnimationFade];
        [self.tableView endUpdates];
    }
}

#pragma marl - Action button

- (IBAction)tappedAddSavesColorsButton:(id)sender
{
    __weak typeof(self) wself = self;
    [HLLoadColorsViewController presentLoadColorsFromViewController:self
                                                         completion:^(NSDictionary *dictionary) {
                                                             NSMutableArray *animationColorsMutableArray =
                                                                 [[HLSettings shared].animationColorsArray mutableCopy];
                                                             if (!animationColorsMutableArray) {
                                                                 animationColorsMutableArray = [NSMutableArray new];
                                                             }
                                                             if ((dictionary != nil)) {
                                                                 [animationColorsMutableArray addObject:dictionary];
                                                                 _colorsArray = [animationColorsMutableArray copy];
                                                                 [HLSettings shared].animationColorsArray =
                                                                     _colorsArray;
                                                                 [wself.tableView reloadData];
                                                             }
                                                         }];
}

- (void)tappedCloseButton:(id)sender { [self dismissViewControllerAnimated:YES completion:nil]; }

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
