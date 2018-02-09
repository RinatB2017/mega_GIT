#pragma once

#include <QObject>

class QAction;
class QUndoCommand;
class QUndoStack;

namespace safe
{
    class History
        : public QObject
    {
        Q_OBJECT
    public:
        explicit History(QObject* parent);

        void push(QUndoCommand* command);

        void clear();

        void block();
        void unblock();

        QAction* createUndoAction(QWidget* parent, const QString& text);
        QAction* createRedoAction(QWidget* parent, const QString& text);

    private slots:
        void undo();
        void redo();

    private:
        QUndoStack* undoStack_;

        QAction* undo_;
        QAction* redo_;
    };
}
