#ifndef REGISTRYITEMCREATIONDIALOG_H
#define REGISTRYITEMCREATIONDIALOG_H

#include <QDialog>

namespace Ui {
class RegistryItemCreationDialog;
}

class RegistryItemCreationDialog : public QDialog {
    Q_OBJECT

public:
    explicit RegistryItemCreationDialog( QWidget* parent = 0 );
    ~RegistryItemCreationDialog();

    QString getName() const;
    QString getValue() const;

private:
    Ui::RegistryItemCreationDialog *ui;
};

#endif // REGISTRYITEMCREATIONDIALOG_H
