#ifndef BOOKMARKSMANAGER_H
#define BOOKMARKSMANAGER_H

#include <QtWidgets>

class BookmarksManager : public QFrame
{
    Q_OBJECT
    public:
        explicit BookmarksManager(QWidget *parent = 0);

    signals:

    private slots:
        void deleteItems();

    protected:
        void resizeEvent(QResizeEvent *event);

    public slots:
        void updateItems();
        void saveBookmarks();

    private:
        QStandardItemModel *model;
        QTableView *tableView;
        QTextEdit *textView;
        QPushButton *btnDelete;

};

#endif // BOOKMARKSMANAGER_H
