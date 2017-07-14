#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QSet>

class QTreeWidgetItem;
class QSettings;

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget( QWidget* parent = 0 );
    ~MainWidget();

private slots:
    void onRegistryItemExpanded( QTreeWidgetItem* item );
    void onRegistryItemClicked( QTreeWidgetItem* item, int column );

    void onCreate();
    void onRemove();

private:
    void addRegistryGroup( QTreeWidgetItem* root, QSettings* settings );

    QString findPathForItem( QTreeWidgetItem* item ) const;

private:
    Ui::MainWidget *ui;
};

#endif // MAINWIDGET_H
