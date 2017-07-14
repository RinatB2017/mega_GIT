#include <QApplication>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QStandardItemModel>
#include <QTableView>
#include <QComboBox>

#include "ComboBoxDelegate.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ComboBoxDelegate* delegate = new ComboBoxDelegate();

#if 0
    QComboBox *cb = new QComboBox();
    cb->setItemDelegate(delegate);
    cb->addItem("a1");
    cb->addItem("a2");
    cb->addItem("a3");
    cb->show();
#else
    QStandardItemModel model(4, 2);
    QTableView tableView;
    tableView.setModel(&model);

    // Column 0 can take any value, column 1 can only take values up to 8.
    tableView.setItemDelegateForColumn(1, delegate);

    for (int row = 0; row < 4; ++row)
    {
        for (int column = 0; column < 2; ++column)
        {
            QModelIndex index = model.index(row, column, QModelIndex());
            int value = (row+1) * (column+1);
            model.setData(index, QVariant(value));
        }
    }

    // Make the combo boxes always displayed.
    for ( int i = 0; i < model.rowCount(); ++i )
    {
        tableView.openPersistentEditor( model.index(i, 1) );
    }

    tableView.show();
#endif
    return app.exec();
}
