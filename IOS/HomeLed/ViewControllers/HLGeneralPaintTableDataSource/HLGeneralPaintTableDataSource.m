//
//  HLGeneralPaintTableDataSource.m
//  HomeLed
//
//  Created by ANDREI VAYAVODA on 06.08.15.
//  Copyright (c) 2015 Voevoda's Incorporated. All rights reserved.
//

#import "HLGeneralPaintTableDataSource.h"
#import "HLAnimationOptionsCell.h"
#import "HLNetworkAddressCell.h"
#import "HLChangeColorCell.h"
#import "HLMultiColorsCell.h"
#import "HLSettingsCell.h"

@interface HLGeneralPaintTableDataSource ()<UITableViewDelegate, UITableViewDataSource>
@property(weak, nonatomic) IBOutlet UITableView *tableView;
@property(strong, nonatomic) IBOutlet UIViewController *viewController;
@property(strong, nonatomic) NSMutableArray *expandedCellsArray;
@end

@implementation HLGeneralPaintTableDataSource {
    NSInteger _indexOfNetworkAddressCell;
    NSInteger _indexOfChangeColorCell;
    NSInteger _indexOfMultiColorsCell;
    NSInteger _indexOfAnimationOptionsCell;
    NSInteger _indexOfSettingsCell;
}

- (id)init
{
    if (self = [super init]) {
        self.expandedCellsArray = [NSMutableArray array];
    }
    return self;
}

#pragma mark - UITableViewDataSource
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    [self resetIndexes];
    NSInteger numberOfCells = 0;
    _indexOfNetworkAddressCell = numberOfCells++;
    _indexOfChangeColorCell = numberOfCells++;
    _indexOfMultiColorsCell = numberOfCells++;
    _indexOfAnimationOptionsCell = numberOfCells++;
    _indexOfSettingsCell = numberOfCells++;
    return numberOfCells;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    HLBaseCell *cell = nil;
    if (indexPath.row == _indexOfNetworkAddressCell) {
        cell = [self networkAddressCell:indexPath];
    } else if (indexPath.row == _indexOfChangeColorCell) {
        cell = [self changeColorCell:indexPath];
    } else if (indexPath.row == _indexOfMultiColorsCell) {
        cell = [self multiColorsCell:indexPath];
    } else if (indexPath.row == _indexOfAnimationOptionsCell) {
        cell = [self animationsOptionsCell:indexPath];
    } else if (indexPath.row == _indexOfSettingsCell) {
        cell = [self settingsCell:indexPath];
    }
    cell.expandedMode = [self.expandedCellsArray containsObject:indexPath];

    return cell;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    HLBaseCell *cell = (HLBaseCell *)[self tableView:tableView cellForRowAtIndexPath:indexPath];
    if ([self.expandedCellsArray containsObject:indexPath]) {
        return cell.expandedHeight;
    }
    return 44.f;
}

#pragma mark - Cells Methods
- (HLNetworkAddressCell *)networkAddressCell:(NSIndexPath *)indexPath
{
    HLNetworkAddressCell *cell = [_tableView dequeueReusableCellWithIdentifier:[HLNetworkAddressCell reuseIdentifier]];
    if (!cell) {
        cell = (HLNetworkAddressCell *)
               [[NSBundle mainBundle] loadNibNamed:@"HLNetworkAddressCell" owner:self options:nil].firstObject;
    }
    return cell;
}

- (HLChangeColorCell *)changeColorCell:(NSIndexPath *)indexPath
{
    HLChangeColorCell *cell = [_tableView dequeueReusableCellWithIdentifier:[HLChangeColorCell reuseIdentifier]];
    if (!cell) {
        cell = (HLChangeColorCell *)
               [[NSBundle mainBundle] loadNibNamed:@"HLChangeColorCell" owner:self options:nil].firstObject;
    }
    return cell;
}

- (HLMultiColorsCell *)multiColorsCell:(NSIndexPath *)indexPath
{
    HLMultiColorsCell *cell = [_tableView dequeueReusableCellWithIdentifier:[HLMultiColorsCell reuseIdentifier]];
    if (!cell) {
        cell = (HLMultiColorsCell *)
               [[NSBundle mainBundle] loadNibNamed:@"HLMultiColorsCell" owner:self options:nil].firstObject;
        cell.viewController = _viewController;
        [cell initialize];
    }
    return cell;
}

- (HLAnimationOptionsCell *)animationsOptionsCell:(NSIndexPath *)indexPath
{
    HLAnimationOptionsCell *cell =
        [_tableView dequeueReusableCellWithIdentifier:[HLAnimationOptionsCell reuseIdentifier]];
    if (!cell) {
        cell = (HLAnimationOptionsCell *)
               [[NSBundle mainBundle] loadNibNamed:@"HLAnimationOptionsCell" owner:self options:nil].firstObject;
    }
    return cell;
}

- (HLSettingsCell *)settingsCell:(NSIndexPath *)indexPath
{
    HLSettingsCell *cell = [_tableView dequeueReusableCellWithIdentifier:[HLSettingsCell reuseIdentifier]];
    if (!cell) {
        cell =
            (HLSettingsCell *)[[NSBundle mainBundle] loadNibNamed:@"HLSettingsCell" owner:self options:nil].firstObject;
    }
    return cell;
}

#pragma mark - UITableViewDelegate
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if ([self.expandedCellsArray containsObject:indexPath]) {
        [self.expandedCellsArray removeObject:indexPath];
    } else {
        [self.expandedCellsArray addObject:indexPath];
    }
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
    HLBaseCell *cell = (HLBaseCell *)[tableView cellForRowAtIndexPath:indexPath];
    if (!cell.disallowExpanding) {
        [self.tableView beginUpdates];
        cell.expandedMode = !cell.expandedMode;
        [self.tableView reloadRowsAtIndexPaths:[NSArray arrayWithObjects:indexPath, nil]
                              withRowAnimation:UITableViewRowAnimationFade];
        [self.tableView endUpdates];
    }
}

#pragma mark - Private Methods
- (void)resetIndexes
{
    _indexOfNetworkAddressCell = NSNotFound;
    _indexOfChangeColorCell = NSNotFound;
}

@end
