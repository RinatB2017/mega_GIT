#include "registryitemcreationdialog.h"
#include "ui_registryitemcreationdialog.h"

RegistryItemCreationDialog::RegistryItemCreationDialog( QWidget* parent ) : QDialog(parent),
    ui( new Ui::RegistryItemCreationDialog ) {
    ui->setupUi( this );

    setWindowTitle( trUtf8( "Новое значение" ) );
}

RegistryItemCreationDialog::~RegistryItemCreationDialog() {
    delete ui;
}

QString RegistryItemCreationDialog::getName() const {
    return ui->edName->text();
}

QString RegistryItemCreationDialog::getValue() const {
    return ui->edValue->text();
}
