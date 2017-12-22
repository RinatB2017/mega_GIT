#ifndef MEMORYVIEWER_H
#define MEMORYVIEWER_H

#include <QDialog>

namespace Ui {
    class MemoryViewer;
}

class MemoryViewer : public QDialog
{
    Q_OBJECT

public:
    explicit MemoryViewer(QWidget *parent = 0);
    ~MemoryViewer();

private:
    Ui::MemoryViewer *ui;
};

#endif // MEMORYVIEWER_H
