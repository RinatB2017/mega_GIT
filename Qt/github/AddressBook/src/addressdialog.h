#pragma once
#include <QDialog>
#include <QSortFilterProxyModel>
#include "widgethelpers.h"

#define NEW_COUNTRY_MODEL new BaseComboModel( QString( "%1 || ' - ' || %2 " ).arg( AddressBookMainWindow::COUNTRY_NAME_COLUMN ).arg( AddressBookMainWindow::COUNTRY_CODE_COLUMN ), "country",  this )

class QSqlTableModel;

namespace Ui {
class AddressDialog;
}

class AddressDialog : public QDialog
{
	Q_OBJECT

	Ui::AddressDialog *ui;
	WidgetHelpers m_widgetHelpers;
	QSqlTableModel* m_addressTableModelPtr;
	QSortFilterProxyModel m_proxyModel;
	QVariant m_selectedAddressId;

public:

	enum Columns // Depend with createTable( ADDRESS_TABLE_NAME, columnsList );
	{
		Id,
	};

	explicit AddressDialog( QVariant id, QWidget *parent = 0 );
	QVariant selectedAddressId() const;
	~AddressDialog();

protected slots:
	void selectionChangedSlot();
	void on_newButton_clicked();
	void on_deleteButton_clicked();
	void on_addressTable_doubleClicked(const QModelIndex &index);
	void updateTable();

private slots:
	void on_selectButton_clicked();
	void on_closeButtojn_clicked();
};
