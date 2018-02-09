#pragma once

#include <QUndoCommand>

namespace safe
{
    class SafeWidget;

    class SwitchCommand
        : public QUndoCommand
    {
    public:
        SwitchCommand(SafeWidget* safeWidget, int row, int column);

        void undo() override;
        void redo() override;

    private:
        SafeWidget* const safeWidget_;

        const int row_;
        const int column_;
    };
}
