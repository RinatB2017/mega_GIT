#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "registryitemcreationdialog.h"

#include <QSettings>
#include <QDebug>
#include <QTreeWidgetItem>

#include <algorithm>

static const auto REG_PATH_SEPARATOR = "\\";

MainWidget::MainWidget( QWidget* parent ) :
    QWidget( parent ),
    ui( new Ui::MainWidget ) 
    {
    ui->setupUi( this );

    auto* root = new QTreeWidgetItem( QStringList() << "Computer" );
    ui->treeWidget->addTopLevelItem( root );

    const auto REGISTRY_GROUPS = {
        "HKEY_CLASSES_ROOT",
        "HKEY_CURRENT_USER",
        "HKEY_LOCAL_MACHINE",
        "HKEY_USERS"
    };
    for( const auto& g : REGISTRY_GROUPS ) {
        QSettings settings( g, QSettings::NativeFormat );
        auto* item = new QTreeWidgetItem( QStringList() << g );
        root->addChild( item );
        addRegistryGroup( item, &settings );
    }

    ui->treeWidget->expandItem( root );

    connect(
        ui->treeWidget,
        SIGNAL( itemExpanded( QTreeWidgetItem* ) ),
        SLOT( onRegistryItemExpanded( QTreeWidgetItem* ) )
    );
    connect(
        ui->treeWidget,
        SIGNAL( itemClicked( QTreeWidgetItem*, int ) ),
        SLOT( onRegistryItemClicked( QTreeWidgetItem*, int ) )
    );

    connect( ui->bnCreate, SIGNAL( clicked( bool ) ), SLOT( onCreate() ) );
    connect( ui->bnRemove, SIGNAL( clicked( bool ) ), SLOT( onRemove() ) );
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::onRegistryItemExpanded( QTreeWidgetItem* item )
{
    QSettings settings( findPathForItem( item ), QSettings::NativeFormat );
    addRegistryGroup( item, &settings );
}

void MainWidget::onRegistryItemClicked( QTreeWidgetItem* item, int column )
{
    Q_UNUSED( column )

    if( item && item->parent() ) {
        QSettings settings( findPathForItem( item ), QSettings::NativeFormat );
        ui->tableWidget->setRowCount( 0 );
        ui->tableWidget->clearContents();
        for( const auto& key : settings.childKeys() )
        {
            auto row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow( row );

            auto item = new QTableWidgetItem( key );
            item->setFlags( item->flags() & ~Qt::ItemIsEditable );
            ui->tableWidget->setItem( row, 0, item );

            auto val = settings.value( key ).toString();
            item = new QTableWidgetItem( val );
            item->setFlags( item->flags() & ~Qt::ItemIsEditable );
            ui->tableWidget->setItem( row, 1, item );
        }
    }
}

void MainWidget::onCreate()
{
    RegistryItemCreationDialog dlg( this );
    if( dlg.exec() == QDialog::Accepted )
    {
        auto item = ui->treeWidget->currentItem();
        QSettings settings( findPathForItem( item ), QSettings::NativeFormat );
        settings.setValue( dlg.getName(), dlg.getValue() );
        settings.sync();
        onRegistryItemClicked( item, 0 );
    }
}

void MainWidget::onRemove()
{
    auto item = ui->treeWidget->currentItem();
    auto path = findPathForItem( item );
    QSettings settings( findPathForItem( item ), QSettings::NativeFormat );
    settings.remove( ui->tableWidget->item( ui->tableWidget->currentRow(), 0 )->text() );
    settings.sync();
    onRegistryItemClicked( item, 0 );
}

void MainWidget::addRegistryGroup( QTreeWidgetItem* root, QSettings* settings )
{
    static int depth = 0;
    static const int MAX_DEPTH = 2;

    ++depth;

    if( 0 < root->childCount() )
    {
        for( int i = 0; i < root->childCount(); ++i )
        {
            auto child = root->child( i );
            if( 0 < child->childCount() )
            {
                break;
            }

            settings->beginGroup( child->text( 0 ) );
            addRegistryGroup( child, settings );
            settings->endGroup();
        }
    } else if( depth <= MAX_DEPTH )
    {
        for( const auto& g : settings->childGroups() )
        {
            auto* item = new QTreeWidgetItem( QStringList() << g );
            root->addChild( item );

            settings->beginGroup( g );
            addRegistryGroup( item, settings );
            settings->endGroup();
        }
    }

    --depth;
}

QString MainWidget::findPathForItem( QTreeWidgetItem* item ) const
{
    QStringList reversePath;
    for( ; item != nullptr; item = item->parent() )
    {
        reversePath << item->text( 0 );
    }

    reversePath.pop_back();
    std::reverse( reversePath.begin(), reversePath.end() );

    return reversePath.join( REG_PATH_SEPARATOR );
}
