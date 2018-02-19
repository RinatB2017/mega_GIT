#include "addressdialog.h"
#include "ui_addressdialog.h"
#include "addressbookmainwindow.h"
#include "basecombomodel.h"
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QItemSelectionModel>

AddressDialog::AddressDialog( QVariant id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddressDialog)
{
    ui->setupUi( this );
    m_addressTableModelPtr = new QSqlTableModel( this, QSqlDatabase::database() );
    m_proxyModel.setSourceModel( m_addressTableModelPtr );
    m_addressTableModelPtr->setEditStrategy( QSqlTableModel::OnFieldChange );
    ui->addressTable->setModel( m_addressTableModelPtr );
    m_addressTableModelPtr->setTable( AddressBookMainWindow::ADDRESS_TABLE_NAME );
    m_addressTableModelPtr->select();
    AddressBookMainWindow::ColumnsNames columnsNames;
    columnsNames.insert( AddressBookMainWindow::ADDRESS_LINE1_COL_NAME, "Addr. Line 1" );
    columnsNames.insert( AddressBookMainWindow::ADDRESS_LINE2_COL_NAME, "Addr. Line 2" );
    columnsNames.insert( AddressBookMainWindow::ADDRESS_LINE3_COL_NAME, "Addr. Line 3" );
    AddressBookMainWindow::setColumnNamesAndHideAnother( ui->addressTable, columnsNames );
    ui->countryCombo->setModel( NEW_COUNTRY_MODEL );
    ui->addressTable->setSortingEnabled( true );
    m_widgetHelpers.setupForm( this, AddressBookMainWindow::ADDRESS_TABLE_NAME );
    m_widgetHelpers.setAdditionalDisableWidgets( QWidgetList() << ui->deleteButton << ui->selectButton );
    connect( ui->addressTable->selectionModel(), SIGNAL( selectionChanged( QItemSelection,QItemSelection)  ),  SLOT( selectionChangedSlot() ) );

    // Select appropriate record in table
    if( id.isValid() )
    {
        const QModelIndexList rows = m_addressTableModelPtr->match( m_addressTableModelPtr->index( 0, Id ), Qt::DisplayRole, id, 1 /*, Qt::MatchExactly */ );  // TODO: improwe search

        if( !rows.isEmpty() )
        {
            QModelIndex proxyIndex = m_proxyModel.mapFromSource( rows.first() );
            ui->addressTable->selectRow( proxyIndex.row() ); // And call selectionChangedSlot()
        }
    }

    if( m_addressTableModelPtr->rowCount() == 0 )
    {
        on_newButton_clicked();
    }

    selectionChangedSlot();
    connect( &m_widgetHelpers, SIGNAL( dataChanged( QString, QString, QVariant, QVariant ) ), SLOT( updateTable() ) );
}

QVariant AddressDialog::selectedAddressId() const
{
    return m_selectedAddressId;
}

AddressDialog::~AddressDialog()
{
    delete ui;
}

void AddressDialog::on_newButton_clicked()
{
    int newRow = AddressBookMainWindow::addRowToModel( m_addressTableModelPtr );
    newRow = ui->addressTable->rowAt( newRow );
    ui->addressTable->selectRow( newRow );
    selectionChangedSlot();
}

void AddressDialog::on_deleteButton_clicked()
{
    const QVariant id = m_widgetHelpers.getTableId( AddressBookMainWindow::ADDRESS_TABLE_NAME );

    if( AddressBookMainWindow::deleteRowFromModels( m_addressTableModelPtr, &m_proxyModel, ui->addressTable ) )
    {
        QSqlQuery query; // Clean usage that address in another cases
        query.prepare( QString( "UPDATE %1 SET %2 = Q_NULLPTR WHERE %2 = :id" ).arg( AddressBookMainWindow::PERSONS_TABLE_NAME ).arg( AddressBookMainWindow::ADDRESS_FK_COL_NAME ) );
        query.bindValue( ":id", id );
        EXEC_SQL_QUERY( query );
        m_addressTableModelPtr->select();
        selectionChangedSlot();
    }
}

void AddressDialog::on_addressTable_doubleClicked( const QModelIndex &index )
{
    m_selectedAddressId = m_addressTableModelPtr->index( index.row(), Id ).data();
    accept();
}

void AddressDialog::selectionChangedSlot()
{
    const int row = AddressBookMainWindow::selectedRow( ui->addressTable );

    if( row == AddressBookMainWindow::NO_SELECTION )
    {
        m_selectedAddressId = QVariant();
    }
    else
    {
        m_selectedAddressId = m_addressTableModelPtr->index( row, Id ).data();
    }

    m_widgetHelpers.fillForm( m_selectedAddressId );
}

void AddressDialog::on_selectButton_clicked()
{
    accept();
}

void AddressDialog::on_closeButtojn_clicked()
{
    reject();
}

void AddressDialog::updateTable()
{
    if( m_addressTableModelPtr )
        m_addressTableModelPtr->select();
}
