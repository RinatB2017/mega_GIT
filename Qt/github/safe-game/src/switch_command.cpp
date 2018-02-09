#include "switch_command.h"

#include "safe_widget.h"

safe::SwitchCommand::SwitchCommand(SafeWidget* safeWidget,
                                   int row,
                                   int column)
    : safeWidget_(safeWidget)
    , row_(row)
    , column_(column)
{
}

void safe::SwitchCommand::undo()
{
    safeWidget_->doSwitch(row_, column_);
}

void safe::SwitchCommand::redo()
{
    safeWidget_->doSwitch(row_, column_);
}
