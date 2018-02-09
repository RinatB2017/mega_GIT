#include "history.h"

#include <QAction>
#include <QUndoStack>

safe::History::History(QObject* parent)
    : QObject(parent)
{
    undoStack_ = new QUndoStack(this);
}

void safe::History::push(QUndoCommand* command)
{
    undoStack_->push(command);
}

void safe::History::clear()
{
    undoStack_->clear();
}

void safe::History::block()
{
    undo_->setDisabled(true);
    redo_->setDisabled(true);
}

void safe::History::unblock()
{
    undo_->setEnabled(undoStack_->canUndo());
    redo_->setEnabled(undoStack_->canRedo());
}

QAction* safe::History::createUndoAction(QWidget* parent, const QString& text)
{
    undo_ = new QAction(text, parent);
    undo_->setDisabled(true);
    connect(undo_, &QAction::triggered, this, &History::undo);
    return undo_;
}

QAction* safe::History::createRedoAction(QWidget* parent, const QString& text)
{
    redo_ = new QAction(text, parent);
    redo_->setDisabled(true);
    connect(redo_, &QAction::triggered, this, &History::redo);
    return redo_;
}

void safe::History::undo()
{
    undoStack_->undo();
}

void safe::History::redo()
{
    undoStack_->redo();
}
