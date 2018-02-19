#include <QSettings>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QKeyEvent>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <cstdlib>
#include "addressbookmainwindow.h"
#include "setupdialog.h"
#include "addressdialog.h"
#include "nationalitymodel.h"
#include "basecombomodel.h"

#include "ui_addressbookmainwindow.h"

#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
#   define USE_UUID
#   include <QUuid>
#else
#   include <QTime>
#endif

namespace
{
	enum Columns
	{
		Id,
	};

	const int TEST_ADDRESSES_COUNT = 8;
	const QString DB_PATH_SETTING_KEY( "DBPath" );
	const QString COUNTRY_TABLE_NAME( "country" );
	const QString ADDRESS_FK_COL_NAME( "addressid" );

	const QStringList FIRST_NAMES( QStringList() << "Matthew" << "Robert" << "Daniel" << "Nicholas" << "Andrey" << "James" << "Christian" << "Noah"
													<< "Lauren" << "Anna" << "Victoria" << "Destiny" << "Mackenzie" << "Nicole" << "Christian" << "Stephanie"  );

	const QStringList LAST_NAMES( QStringList() << "Parks"  << "Lyons" << "Wiggins" << "Norman" << "Wilkerson" << "Harper" << "Gray" << "Clark"
												<< "Campbell"  << "Ray" << "Nelson" << "Powell" << "Murphy" << "Woods" << "Gaines" << "Fleming"  );

	const QStringList ADDRESS_LINE1_TEMPLATES( QStringList() << "Lemoyer Blvd Philadelphia" << "Cardigan St. Arlington" << "Bourg St Irving" << "Freeton Blvd Raleigh"
											   << "Lowel Rd Sunnyvale"  << "Warden ave Algonquin" << "ingston St. Hamilton" << "Third St Orlando" );

	const QStringList ADDRESS_LINE3_TEMPLATES( QStringList() << "Raleigh" << "Aurora" << "Pittsburgh" << "Orlando" << "St. Petersburg" << "Glendale" << "Fremont" << "Tacoma" );

	int randomNumber( const unsigned int maximum )
	{
		unsigned int key = 0;

#ifdef USE_UUID
		key = qHash( QUuid::createUuid() );
#undef USE_UUID
#else
		key = qHash( QTime::currentTime() );
#endif
		srand( static_cast< unsigned int >( key ) );

		return ( rand() % maximum );
	}

	QString randomString( const QStringList& templatesList )
	{
		QString result;
		result = templatesList.at( randomNumber( static_cast< unsigned int >( templatesList.count() ) ) );
		return result;
	}

	bool createTable( const QString& tableName, const QStringList& columns )
	{
		QSqlQuery query;
		query.prepare(
					QString( "CREATE TABLE %1 "
				  "( id INTEGER PRIMARY KEY AUTOINCREMENT, %2 )" )
					   .arg( tableName )
					   .arg( columns.join( " ," ) )
					);
		bool result = EXEC_SQL_QUERY( query );

		return result;
	}

	QVariant randomComboPoint( QComboBox* combo ) // Please note. Return UserRole data of random item
	{
		QVariant result;

		if( combo && combo->count() > 0 )
		{
			QAbstractItemModel* comboModel = combo->model();

			if( comboModel )
			{
				result = comboModel->index( randomNumber( comboModel->rowCount() ), 0 ).data( Qt::UserRole );

				if( result == comboModel->index( 0, 0 ).data( Qt::UserRole ) )
				{
					result = randomComboPoint( combo );
				}
			}
		}

		return result;
	}

	QString pathToDb()
	{
		return QSettings().value( DB_PATH_SETTING_KEY ).toString();
	}
}

const QString AddressBookMainWindow::ADDRESS_TABLE_NAME( "address" );
const QString AddressBookMainWindow::COUNTRY_NAME_COLUMN( "countryname" );
const QString AddressBookMainWindow::COUNTRY_CODE_COLUMN( "countrycode" );
const QString AddressBookMainWindow::PERSONS_TABLE_NAME( "persons" );
const QString AddressBookMainWindow::ADDRESS_FK_COL_NAME( "addressid" );
const QString AddressBookMainWindow::COUNTRY_FK_COL_NAME( "countryid" );
const QString AddressBookMainWindow::FIRST_NAME_COL_NAME( "firstname" );
const QString AddressBookMainWindow::LAST_NAME_COL_NAME( "lastname" );
const QString AddressBookMainWindow::DOB_COL_NAME( "dob" );
const QString AddressBookMainWindow::ADDRESS_LINE1_COL_NAME( "addressline1" );
const QString AddressBookMainWindow::ADDRESS_LINE2_COL_NAME( "addressline2" );
const QString AddressBookMainWindow::ADDRESS_LINE3_COL_NAME( "addressline3" );


AddressBookMainWindow::AddressBookMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::AddressBookMainWindow)
{
	ui->setupUi( this );
	const QStringList driversNames = m_database.drivers();

	if( driversNames.indexOf( "QSQLITE" ) < 0 )
	{
		Q_ASSERT( !"Can't continue. Because your Qt library do not have SQLite driver." );
		::exit( 0 );
	}

	QString reason;
	QString dbPath( pathToDb() );
	m_database = QSqlDatabase::addDatabase( "QSQLITE" );

	if ( dbPath.isEmpty() || !QFileInfo( dbPath ).exists() )
		reason = "Database file not founded.";
	else
		openDb( dbPath );

	while( !m_database.isOpen() )
	{
		if( reason.isEmpty() )
		{
			if( !QFileInfo( dbPath ).exists() )
				reason = "No database file on last path";
			else if( !openDb( pathToDb() ) )
				reason = "Can't open that DB!";
		}
		else
		{
			if( createDb( reason ) )
				reason = QString::null;
			else
				reason = "Can't create new DB in that path!";
		}
	}

	qDebug() << "Path to DB:" << pathToDb();

	m_personsModelPtr = new QSqlTableModel( this, m_database ); // Model should create after database setup
	m_personsModelPtr->setTable( PERSONS_TABLE_NAME );
	m_personsModelPtr->select();
	m_personsModelPtr->setEditStrategy( QSqlTableModel::OnFieldChange );
	m_proxyModel.setSourceModel( m_personsModelPtr );
	ui->personsTable->setModel( &m_proxyModel );
	ColumnsNames columnsNames;
	columnsNames.insert( FIRST_NAME_COL_NAME, "First Name" );
	columnsNames.insert( LAST_NAME_COL_NAME, "Last Name" );
	columnsNames.insert( DOB_COL_NAME,  "Date of burth" );
	setColumnNamesAndHideAnother( ui->personsTable, columnsNames );

	QHeaderView* header = ui->personsTable->horizontalHeader();
	header->setStretchLastSection( true );
	ui->personsTable->setSortingEnabled( true );

	// Just for example what that possible. You can fill properties at QtDesigner too
	ui->countryLabel->setProperty( WidgetHelpers::COLUMN_NAME_PROP, COUNTRY_NAME_COLUMN );
	ui->countryLabel->setProperty( WidgetHelpers::TABLE_NAME_PROP, COUNTRY_TABLE_NAME );

	ui->countryCombo->setModel( NEW_COUNTRY_MODEL );
	ui->countryCombo->hide();  // Hide because that data should change at address form
	m_widgetHelpers.setupForm( this, PERSONS_TABLE_NAME );
	ui->nationalityCombo->setModel( new NationalityModel( this ) );
	m_widgetHelpers.setAdditionalDisableWidgets( QWidgetList() << ui->deleteRecordButton << ui->changeAddressButton << ui->writeEmail << ui->makeCall );
	connect( ui->personsTable->selectionModel(), SIGNAL( selectionChanged(QItemSelection,QItemSelection ) ), SLOT( updateForm() ) );

	connect( &m_widgetHelpers
			 , SIGNAL( dataChanged( QString, QString , QVariant , QVariant ) )
			 , SLOT( updateTable() ) );

	if( m_personsModelPtr->rowCount() > 0 )
		ui->personsTable->selectRow( 0 );
	else
		on_newRecordButton_clicked();

	updateForm();
	ui->nullButton->setHidden( true ); // Just fake button for null value in QRadioButton's group
	connect( ui->fillTestDataBtn, SIGNAL( clicked( bool ) ), SLOT( fillTestData() ) );
}

void AddressBookMainWindow::updateForm()
{
	const int row = AddressBookMainWindow::selectedRow( ui->personsTable );
	QVariant id;

	if( row != NO_SELECTION )
	{
		id = m_personsModelPtr->index( row, Id ).data();
	}

	m_widgetHelpers.fillForm( id );
	const QVariant addressId = m_widgetHelpers.getFieldValue( PERSONS_TABLE_NAME, ADDRESS_FK_COL_NAME );
	setAddress( addressId );
	m_widgetHelpers.fillForm( addressId, ADDRESS_TABLE_NAME );
	const QVariant countryId( m_widgetHelpers.getFieldValue( ADDRESS_TABLE_NAME, COUNTRY_FK_COL_NAME ) );
	m_widgetHelpers.fillForm( countryId, COUNTRY_TABLE_NAME );
}

bool AddressBookMainWindow::createDb( const QString& reason )
{
	bool result = false;
	SetupDialog dialog;
	dialog.setReasonText( reason );

	if( dialog.exec() == QDialog::Rejected )
		exit( 0 ); // User can't define path. Exit program

	const QFileInfo selectedFile( dialog.selectedFile() );
	const QString selectedFileName( selectedFile.absoluteFilePath() );
	const QDir folderCreator;
	folderCreator.mkpath( selectedFile.dir().absolutePath() );

	m_database.setDatabaseName( selectedFileName );

	if( m_database.open() )
	{
		QStringList columnsList;
		columnsList << FIRST_NAME_COL_NAME + " TEXT"
					<< LAST_NAME_COL_NAME + " TEXT"
//					<< "middlename TEXT"
					<< DOB_COL_NAME + " TEXT" // Date of birth
					<< ADDRESS_FK_COL_NAME + " INTEGER"
					<< "nationality TEXT"
					<< "sex TEXT"
					<< "email"
					<< "skype";

		result = createTable( PERSONS_TABLE_NAME, columnsList );
		columnsList.clear();

		columnsList << ADDRESS_LINE1_COL_NAME + " TEXT"
				<< ADDRESS_LINE2_COL_NAME + " TEXT"
				<< ADDRESS_LINE3_COL_NAME + " TEXT"
//				<< "postcode TEXT"
				<< COUNTRY_FK_COL_NAME + " INTEGER";

		result = result && createTable( ADDRESS_TABLE_NAME, columnsList );
		columnsList.clear();
		columnsList << COUNTRY_NAME_COLUMN << COUNTRY_CODE_COLUMN;
		result = result && createTable( COUNTRY_TABLE_NAME, columnsList );

		QSqlQuery query;
		query.prepare( QString( "INSERT INTO %1 ( %2, %3 ) VALUES ( ?, ? ) " ).arg( COUNTRY_TABLE_NAME ).arg( COUNTRY_NAME_COLUMN ).arg( COUNTRY_CODE_COLUMN ) );
		query.addBindValue( QVariantList() << "Russia" << "Ukraine" << "United States" << "Germany" << "Poland" << "China" << "Cambodia" );
		query.addBindValue( QVariantList() << "RU" << "UA" << "US" << "DE" << "PL" << "CN" << "KH" );
		result = result && query.execBatch();

		QSettings settings;
		settings.setValue( DB_PATH_SETTING_KEY, selectedFileName );
		settings.sync();
	}

	return result;
}

AddressBookMainWindow::~AddressBookMainWindow()
{
	delete ui;
}

void AddressBookMainWindow::on_newRecordButton_clicked()
{
	int newRow = AddressBookMainWindow::addRowToModel( m_personsModelPtr );
	newRow = ui->personsTable->rowAt( newRow );
	ui->personsTable->selectRow( newRow );
	updateForm();
}

void AddressBookMainWindow::on_deleteRecordButton_clicked()
{
	AddressBookMainWindow::deleteRowFromModels( m_personsModelPtr, &m_proxyModel, ui->personsTable );
	updateForm();
}

void AddressBookMainWindow::updateTable()
{
	if( m_personsModelPtr )
		m_personsModelPtr->select();
}

void AddressBookMainWindow::on_changeAddressButton_clicked()
{
	AddressDialog dialog( m_widgetHelpers.getFieldValue( PERSONS_TABLE_NAME, ADDRESS_FK_COL_NAME ),	this );
	QVariant newAddressId;

	if( dialog.exec() == QDialog::Accepted )
		newAddressId = dialog.selectedAddressId();

	setAddress( newAddressId );

	QSqlQuery query;
	query.prepare( QString( "UPDATE %1 SET %2 = :value WHERE id = :id" ).arg( PERSONS_TABLE_NAME ).arg( ADDRESS_FK_COL_NAME ) );
	query.bindValue( ":value", newAddressId );
	query.bindValue( ":id", m_widgetHelpers.getTableId( PERSONS_TABLE_NAME ) );
	EXEC_SQL_QUERY( query );
}

void AddressBookMainWindow::setAddress( const QVariant& newAddressId )
{
	bool ok = false;
	QString addressLabelText;
	newAddressId.toInt( &ok );

	if( ok && newAddressId.isValid() )
	{
		QStringList addressTexts;
		const QSqlRecord record = m_widgetHelpers.addRecord( ADDRESS_TABLE_NAME, newAddressId );
		addressTexts << record.value( ADDRESS_LINE1_COL_NAME ).toString();
		addressTexts << record.value( ADDRESS_LINE2_COL_NAME ).toString();
		addressTexts << record.value( ADDRESS_LINE3_COL_NAME ).toString();
		addressLabelText = addressTexts.join( ", " );
	}

	ui->addressLabel->setText( addressLabelText );
}

bool AddressBookMainWindow::openDb( const QString& path )
{

	m_database.setDatabaseName( path );
	bool ok = m_database.open();

	if( !ok )
		qDebug() << m_database.lastError();

	return ok;
}

void AddressBookMainWindow::openExternalLink(const QLineEdit *edit)
{
	const QString prefix( edit->property( "prefix" ).toString() );

	if( !prefix.isEmpty()
			&& edit->text().length() > 3 ) // Think any address should have more than 3 characrers
	{
		QDesktopServices::openUrl( QUrl( prefix + edit->text() ) );
	}
}

void AddressBookMainWindow::on_writeEmail_clicked()
{
	openExternalLink( ui->email );
}

void AddressBookMainWindow::on_makeCall_clicked()
{
	openExternalLink( ui->skype );
}

void AddressBookMainWindow::on_deleteDbFileBtn_clicked()
{
	QSqlDatabase::database().close();
	const bool ok = QFile( pathToDb() ).remove();
	Q_ASSERT( ok ); Q_UNUSED( ok );
	::exit( 0 );
}

void AddressBookMainWindow::fillTestData()
{
	QVariant lastInsertedAddressId;

	for( int i = 0; i < TEST_ADDRESSES_COUNT; ++i )
	{
		if( i % 2 == 0 )
		{
			QSqlQuery makeAddressQuery;
			makeAddressQuery.prepare( QString::fromLatin1( "INSERT INTO %1 ( addressline1, addressline2, addressline3, %2 ) VALUES( :addressline1, :addressline2, :addressline3, :countryid )" ).arg( ADDRESS_TABLE_NAME ).arg( COUNTRY_FK_COL_NAME ) );
			makeAddressQuery.bindValue( ":addressline1", QString::fromLatin1( "%1 %2" ).arg( randomNumber( 200 ) ).arg( randomString( ADDRESS_LINE1_TEMPLATES ) ) );
			makeAddressQuery.bindValue( ":addressline2", QString::fromLatin1( "Apt. # %1" ).arg( randomNumber( 200 ) ) );
			makeAddressQuery.bindValue( ":addressline3", randomString( ADDRESS_LINE3_TEMPLATES ) );
			QVariant countryId;
			countryId = randomComboPoint( ui->countryCombo ); // You can simple comment this if not want that combobox
			makeAddressQuery.bindValue( ":countryid", countryId );
			EXEC_SQL_QUERY( makeAddressQuery );
			lastInsertedAddressId = makeAddressQuery.lastInsertId();
		}

		QSqlQuery makePersonRecord;
		makePersonRecord.prepare(  QString::fromLatin1( "INSERT INTO %1 ( %2, firstname, lastname, dob, nationality, email, skype ) VALUES( :addressid, :firstname, :lastname, :dob, :nationality, :email, :skype ) "  ).arg( PERSONS_TABLE_NAME ).arg( ADDRESS_FK_COL_NAME ) );
		const QString firstName( randomString( FIRST_NAMES ) );
		makePersonRecord.bindValue( ":firstname", firstName );
		makePersonRecord.bindValue( ":lastname", randomString( LAST_NAMES ) );
		const int daysInManyYears = 365*30;
		makePersonRecord.bindValue( ":dob", QDate( 1960, 0, 0 ).addDays( randomNumber( daysInManyYears ) ) );
		makePersonRecord.bindValue( ":addressid", lastInsertedAddressId );
		makePersonRecord.bindValue( ":nationality", randomComboPoint( ui->nationalityCombo ) );
		makePersonRecord.bindValue( ":email", QString( "%1%2@fcorp.com" ).arg( firstName.toLower() ).arg( randomNumber( 1000 ) ) );
		makePersonRecord.bindValue( ":skype", QString( "%1_%2" ).arg( firstName ).arg( randomNumber( 1000 ) ) );
		EXEC_SQL_QUERY( makePersonRecord );
	}

	updateTable();
}

void AddressBookMainWindow::setColumnNamesAndHideAnother( QTableView* const table,  const ColumnsNames &columnsNames )
{
	QHeaderView* header = table->horizontalHeader();

	if( !header )
		return;

	QAbstractItemModel* tableModel = table->model();

	if( !tableModel )
		return;

	for( int i = 0; i < header->count(); ++i )
	{
		const QString& columnName = tableModel->headerData( i, Qt::Horizontal, Qt::DisplayRole ).toString();

		if( columnsNames.contains( columnName ) )
		{
			tableModel->setHeaderData( i, Qt::Horizontal, columnsNames.value( columnName ) );
		}
		else
		{
			header->hideSection( header->logicalIndex( i ) );
		}
	}

	header->setStretchLastSection( true );
}


int AddressBookMainWindow::addRowToModel( QSqlTableModel *sourceModel )
{
	int row = sourceModel->rowCount();
	bool ok = sourceModel->insertRow( row );
	ok = ok && sourceModel->setData( sourceModel->index( row, 1 /* any user data row */ ), QVariant( QString::null ) ); // Hack for save row in DB
	ok = ok && sourceModel->submit();

	return row;
}

bool AddressBookMainWindow::deleteRowFromModels( QSqlTableModel *sourceModel, QSortFilterProxyModel* proxyModel, QTableView* tableView )
{
	int r = selectedRow( tableView );
	bool ok = false;

	if( r != NO_SELECTION && tableView && sourceModel && proxyModel )
	{
		// Little hack that cover some Qt bugs. Column index here(0) have no difference
		tableView->hideRow( proxyModel->mapFromSource( sourceModel->index( r, 0 ) ).row() );
		ok = sourceModel->removeRow( r );
		sourceModel->submit();
	}

	return ok;
}

int AddressBookMainWindow::selectedRow( QTableView* tableView )
{
	int row = NO_SELECTION;

	if( tableView )
	{
		QItemSelectionModel* selectionModel = tableView->selectionModel();

		if( selectionModel )
		{
			QModelIndexList selected = selectionModel->selectedIndexes();

			// For see what kind of model class used for selection model
//			qDebug() << selected.first().model()->metaObject()->className();

			if( !selected.isEmpty() )
			{
				QModelIndex firstIndex = selected.first();
				const QSortFilterProxyModel* proxyModel = qobject_cast<  const QSortFilterProxyModel* >( firstIndex.model() );


				if( proxyModel )
					row = proxyModel->mapToSource( firstIndex ).row();
				else
					row = firstIndex.row();
			}
		}
	}

	return row;
}
