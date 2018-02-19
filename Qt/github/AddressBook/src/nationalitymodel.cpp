#include "nationalitymodel.h"

NationalityModel::NationalityModel(QObject *parent) :
	QAbstractListModel(parent)
{
	m_content << qMakePair( DataPair::first_type(), DataPair::second_type( "" ) )
			<< qMakePair( DataPair::first_type( "Russian" ), DataPair::second_type( "russian" ) )
			<< qMakePair( DataPair::first_type( "Belgian" ), DataPair::second_type( "belgian" ) )
			<< qMakePair( DataPair::first_type( "Norwegian" ), DataPair::second_type( "norwegian" ) )
			<< qMakePair( DataPair::first_type( "American" ), DataPair::second_type( "american" ) )
			<< qMakePair( DataPair::first_type( "German" ), DataPair::second_type( "german" ) );
}

QVariant NationalityModel::data( const QModelIndex &index, int role ) const
{
	const DataPair& data = m_content.at( index.row() );
	QVariant value;

	switch ( role )
	{
		case Qt::DisplayRole:
		{
			value = data.first;
		}
		break;

		case Qt::UserRole:
		{
			value = data.second;
		}
		break;

		default:
			break;
	}

	return value;
}

int NationalityModel::rowCount(const QModelIndex &/*parent*/) const
{
	return m_content.count();
}
