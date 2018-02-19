#pragma once

#include <QObject>
#include <QHash>
#include <QString>
#include <QVariant>
#include <QSqlRecord>
#include <QWidgetList>
#include <QDebug>
#include <QSqlError>

class QSqlQuery;
class QSqlTableModel;
class QSortFilterProxyModel;
class QTableView;

#define EXEC_SQL_QUERY( query ) WidgetHelpers::execSqlQuery( &query, Q_FUNC_INFO );

class WidgetHelpers : public QObject
{
	Q_OBJECT

	QHash <QString, QSqlRecord> m_tablesRecords;
	QWidgetList m_inputsList;
	QWidgetList m_additionalWidgetsToDisable;
	QString m_defaultTableName;

public:
	void setupForm( QWidget* fornPointer, const QString& defaultTableName );
	void fillForm(const QVariant &tableRecordId  // null value (just QVariant() ) mean that form should clear from old data and disable
				  , const QString& tableName = QString::null );
	QSqlRecord addRecord(const QString &tableName, const QVariant &recordId);
	QVariant getTableId(const QString &tableName) const;
	QVariant getFieldValue(const QString &tableName, const QString &columnName) const;
	void setAdditionalDisableWidgets( const QWidgetList &widgets );
	static void setWidgetValue(const QVariant &value, QObject * const inputBox);
	static QVariant getWidgetValue(const QObject* const sourceBox);
	static bool execSqlQuery( QSqlQuery* query, const QString funcInfo );
	static const char* TABLE_NAME_PROP;
	static const char* COLUMN_NAME_PROP;

private slots:
	void saveDataSlot();

signals:
	void dataChanged( QString tableName, QString columnName , QVariant id , QVariant value );

}
;
