#include "basecombomodel.h"
#include <QSqlQuery>
#include <QStringList>

namespace
{
	enum Columns // Depends with 'query.prepare( QString( "SELECT ... '
	{
		Id,
		Data,
	};
}

BaseComboModel::BaseComboModel( const QString& visualColumn, const QString& queryTail, QObject *parent ) :
	QSqlQueryModel( parent )
{
	QSqlQuery query;
	query.prepare( QString( "SELECT %1.id, %2 FROM %3" ).arg( queryTail.split( ' ' ).first() ).arg( visualColumn ).arg( queryTail ) );
	// I.e. query.prepare( "SELECT country.id, countryname || ' - ' || countrycode  FROM country" );
	query.exec();
	QSqlQueryModel::setQuery( query );
}

QVariant BaseComboModel::dataFromParent( QModelIndex index, int column ) const
{
	return QSqlQueryModel::data( QSqlQueryModel::index( index.row() - 1 // "- 1" because make first row empty
														, column ) );
}

int BaseComboModel::rowCount(const QModelIndex &parent) const
{
	return QSqlQueryModel::rowCount( parent ) + 1; // Add info about first empty row
}

QVariant BaseComboModel::data(const QModelIndex & item, int role /* = Qt::DisplayRole */) const
{
	QVariant result;

	if( item.row() == 0 ) // Make first row empty
	{
		switch( role )
		{
			case Qt::UserRole:
				result = 0;
				break;
			case Qt::DisplayRole:
				result = "(please select)";
				break;
			default:
				break;
		}
	}
	else
	{
		switch( role )
		{
			case Qt::UserRole:
				result = dataFromParent( item, Id );
				break;
			case Qt::DisplayRole:
				result = dataFromParent( item, Data );
				break;
			default:
				break;
		}
	}

	return result;
}
