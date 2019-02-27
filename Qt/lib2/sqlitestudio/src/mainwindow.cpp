#include <QMessageBox>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"

SQLiteBrowser::SQLiteBrowser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SQLiteBrowser)
{
    ui->setupUi(this);
    //
    setAcceptDrops(true);
    //Highlighter
    highlighter =  new Highlighter(ui->txtQuery->document());
    //
    myLog = new LogDialog(this);
    connect (ui->actionOpen_Database, SIGNAL(triggered()), SLOT(open()));
    connect (ui->actionSave_Database, SIGNAL(triggered()), SLOT(save()));
    connect (ui->actionAbout, SIGNAL(triggered()), SLOT(about()));
    connect (ui->actionCreate_Table, SIGNAL(triggered()), SLOT(newTable()));
    connect (ui->actionModify_Table, SIGNAL(triggered()), SLOT(editTable()));
    connect (ui->actionDelete_Table, SIGNAL(triggered()), SLOT(dropTable()));
    connect (ui->actionDelete_Index, SIGNAL(triggered()), SLOT(dropIndex()));
    connect (ui->actionCreate_Index, SIGNAL(triggered()), SLOT(createIndex()));
    connect (ui->actionPreferences, SIGNAL(triggered()), SLOT(showPreferences()));
    connect (ui->actionDatabse_As_SQL_File, SIGNAL(triggered()), SLOT(exportDatabaseAsSQL()));
    connect (ui->actionDatabase_From_SQL_File, SIGNAL(triggered()), SLOT(importDatabaseFromSQL()));
    connect (ui->actionTable_as_CSV_File, SIGNAL(triggered()), SLOT(exportTableToCSV()));
    connect (ui->actionTable_From_CSV_File, SIGNAL(triggered()), SLOT(importTableFromCSV()));
    connect (ui->actionCompact_Database, SIGNAL(triggered()), SLOT(compactDb()));
    connect (ui->cmdSearch, SIGNAL(clicked()), SLOT(searchAll()));
    connect (ui->cmdAdvancedSearch, SIGNAL(clicked()), SLOT(advancedSearch()));
    connect (ui->cboTables, SIGNAL(currentIndexChanged(QString)), SLOT(showTable(QString)));
    connect (ui->cmdRefresh, SIGNAL(clicked()), SLOT(showTable()));
    connect (ui->actionSQL_Log, SIGNAL(toggled(bool)), myLog, SLOT(setVisible(bool)));
    connect (ui->actionRefresh, SIGNAL(triggered()), SLOT(reloadDbList()));
    connect (myLog, SIGNAL(closed(bool)), ui->actionSQL_Log, SLOT(setChecked(bool)));
    connect (ui->actionCreate_View, SIGNAL(triggered()), SLOT(newView()));
    connect (ui->actionModify_View, SIGNAL(triggered()), SLOT(modifyView()));
    connect (ui->actionDelete_View, SIGNAL(triggered()), SLOT(deleteView()));
    connect (ui->actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
    //
    QAction *aboutQtAct = new QAction(tr("About Qt"), this);
    connect (aboutQtAct, SIGNAL(triggered()), this, SLOT(aboutQt()));
    ui->menuHelp->addAction(aboutQtAct);
    //
    tableActions = new QActionGroup(this);
    indexActions = new QActionGroup(this);
    databaseActions = new QActionGroup(this);
    indexActions->addAction(ui->actionCreate_Index);
    indexActions->addAction(ui->actionDelete_Index);
    tableActions->addAction(ui->actionCreate_Table);
    tableActions->addAction(ui->actionModify_Table);
    tableActions->addAction(ui->actionDelete_Table);
    tableActions->addAction(ui->actionTable_as_CSV_File);
    tableActions->addAction(ui->actionCreate_View);
    tableActions->addAction(ui->actionModify_View);
    tableActions->addAction(ui->actionDelete_View);
    tableActions->addAction(ui->actionTable_From_CSV_File);
    databaseActions->addAction(ui->actionSave_Database);
    //databaseActions->addAction(ui->actionDatabase_From_SQL_File);
    databaseActions->addAction(ui->actionRevert_Database);
    databaseActions->addAction(ui->actionDatabse_As_SQL_File);
    databaseActions->addAction(ui->actionClose_Database);
    databaseActions->addAction(ui->actionCompact_Database);
    databaseActions->addAction(ui->actionRefresh);
    ui->actionSave_Database->setVisible(false);
    ui->actionRevert_Database->setVisible(false);
    //
    model = new SqlTableModel(this, db);
    //
    closeFile();
    connect (ui->actionClose_Database, SIGNAL(triggered()), SLOT(closeFile()));
    connect (ui->actionNew_Database, SIGNAL(triggered()), SLOT(newFile()));
    ui->menuFile->removeAction(ui->actionExit);
    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect (recentFileActs[i], SIGNAL(triggered()), SLOT(openRecentFile()));
        ui->menuFile->addAction(recentFileActs[i]);
    }
    ui->menuFile->addSeparator();
    ui->menuFile->addAction(ui->actionExit);
    updateRecentFileActions();
    //restore appearance settings
    QSettings sett(qApp->organizationName(), qApp->applicationName()) ;
    ui->splitter->restoreState(sett.value("QueryBrowserSplitter").toByteArray());
    preferencesChanged();
    createLanguageMenu();
}
//-------------------------------------------------------------------------------------------
SQLiteBrowser::~SQLiteBrowser()
{
    //Save Appearance settings
    QSettings sett(qApp->organizationName(), qApp->applicationName());
    sett.setValue("QueryBrowserSplitter", ui->splitter->saveState());
    delete ui;
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::newFile(void)
{
    saveAs();
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::open(void)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"));
    if (!fileName.isEmpty())
        loadFile(fileName);
}
//-------------------------------------------------------------------------------------------
bool SQLiteBrowser::save(void)
{
    return true;
}
//-------------------------------------------------------------------------------------------
bool SQLiteBrowser::saveAs(void)
{
    QString fileName  = QFileDialog::getSaveFileName(this, tr("New Database"));
    if (!fileName.isEmpty())
        loadFile(fileName);
    return !fileName.isEmpty();
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::about(void)
{
    AboutDialog ab;
    ab.setWindowTitle(tr("About %1").arg(qApp->applicationName()));
    ab.exec();
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::aboutQt(void)
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::loadFile(const QString &fileName)
{
    if (fileName.isNull())
        open();
    closeFile();
    db = QSqlDatabase::addDatabase("QSQLITE", SESSION_NAME + QDateTime::currentDateTime().toString());
    db.setDatabaseName(fileName);
    if (!db.open())
    {
        QMessageBox::critical(this, tr("Error"), tr("Database Error.\n%1").arg(db.lastError().text()));
        return;
    }
    QSqlQuery q("SELECT * FROM sqlite_master WHERE 1=2", db);
    myLog->refreshLog("", "SELECT * FROM sqlite_master WHERE 1=2");
    if (!q.exec())
    {
        //Database Open Error
        QMessageBox::critical(this, tr("Error"), tr("Database Error.\n%1 is probably not an sqlite database").arg(fileName));
        return;
    }
    //Database Open
    curFile = fileName;
    ui->statusBar->showMessage(tr("Opened file: %1").arg(fileName), 5000);
    QSettings settings(qApp->organizationName(), qApp->applicationName());
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);

    while (files.size() > MaxRecentFiles)
        files.removeLast();

    settings.setValue("recentFileList", files);
    updateRecentFileActions();
    model = new SqlTableModel(this, db);
    this->setWindowTitle(tr("%1 - %2").arg(qApp->applicationName()).arg(strippedName(fileName)));
    databaseActions->setEnabled(true);
    tableActions->setEnabled(true);
    indexActions->setEnabled(true);
    ui->tableTab->setEnabled(true);
    ui->databaseTab->setEnabled(true);
    ui->queryTab->setEnabled(true);
    reloadDbList();
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::closeFile(void)
{
    curFile = "";
    databaseActions->setEnabled(false);
    tableActions->setEnabled(false);
    indexActions->setEnabled(false);
    ui->lstStructure->clear();
    ui->cboTables->clear();
    this->setWindowTitle(qApp->applicationName());
    //
    ui->txtQuery->setText("");
    ui->txtQueryError->setText("");
    ui->txtSearch->setText("");
    ui->tableTab->setEnabled(false);
    ui->databaseTab->setEnabled(false);
    ui->queryTab->setEnabled(false);
    //Clear models
    SqlQueryModel *dummyModel = new SqlQueryModel(this);
    ui->tblData->setModel(dummyModel);
    ui->tblQueryResult->setModel(dummyModel);
    //db.removeDatabase(SESSION_NAME);
    //
    tableList.clear();
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::reloadDbList(void)
{
    ui->lstStructure->clear();
    ui->cboTables->clear();
    tableList.clear();
    indexList.clear();
    viewList.clear();
    //Show Tables
    QSqlQuery tableQuery("SELECT name, sql FROM sqlite_master WHERE type='table';", db);
    myLog->refreshLog("", "SELECT name, sql FROM sqlite_master WHERE type='table';");
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(ui->lstStructure);
    rootItem->setText(0, curFile);
    rootItem->setIcon(0, QIcon(":/images/database.png"));
    if (tableQuery.exec())
    {
        while (tableQuery.next())
        {
            QTreeWidgetItem *tableItem = new QTreeWidgetItem(rootItem);
            tableItem->setText(0, tableQuery.record().value("name").toString());
            tableItem->setIcon(0, QIcon(":/images/table.png"));
            tableItem->setText(1, "Table");
            tableItem->setText(3, tableQuery.record().value("sql").toString());
            tableList.insert(tableList.count(), tableItem->text(0));
            ui->cboTables->insertItem(ui->cboTables->count(), tableItem->icon(0), tableItem->text(0));
            QSqlQuery columnQuery(QString("PRAGMA TABLE_INFO(%1);").arg(tableItem->text(0)), db);
            myLog->refreshLog("", QString("PRAGMA TABLE_INFO(%1);").arg(tableItem->text(0)));
            if (columnQuery.exec())
            {
                while (columnQuery.next())
                {
                    QTreeWidgetItem *childItem = new QTreeWidgetItem(tableItem);
                    childItem->setText(0, columnQuery.record().value("name").toString());
                    childItem->setText(1, "Field");
                    childItem->setIcon(0, QIcon(":/images/tick.png"));
                    childItem->setText(2, columnQuery.record().value("type").toString());
                }
            }
        }
    }
    //Show Indexes
    QSqlQuery indexQuery("SELECT name, sql FROM sqlite_master WHERE type='index';", db);
    myLog->refreshLog("", ("SELECT name, sql FROM sqlite_master WHERE type='index';"));
    if (indexQuery.exec())
    {
        while (indexQuery.next())
        {
            QTreeWidgetItem *indexItem = new QTreeWidgetItem(rootItem);
            indexItem->setText(0, indexQuery.record().value("name").toString());
            indexItem->setText(1, "Index");
            indexItem->setIcon(0, QIcon(":/images/table_key.png"));
            indexItem->setText(3, indexQuery.record().value("sql").toString());
            indexList.insert(indexList.count(), indexItem->text(0));
        }
    }
    //show views
    QSqlQuery viewQuery("SELECT name, sql FROM sqlite_master WHERE type='view';", db);
    myLog->refreshLog("", ("SELECT name, sql FROM sqlite_master WHERE type='view';"));
    if (viewQuery.exec())
    {
        while (viewQuery.next())
        {
            QTreeWidgetItem *viewItem = new QTreeWidgetItem(rootItem);
            viewItem->setText(0, viewQuery.record().value("name").toString());
            viewItem->setText(1, "View");
            viewItem->setIcon(0, QIcon(":/images/table_gear.png"));
            viewItem->setText(3, viewQuery.record().value("sql").toString());
            viewList.insert(viewList.count(), viewItem->text(0));
            ui->cboTables->insertItem(ui->cboTables->count(), viewItem->icon(0), viewItem->text(0));
        }
    }
    rootItem->setExpanded(true);
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::openRecentFile(void)
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        loadFile(action->data().toString());
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::updateRecentFileActions(void)
{
    QSettings settings(qApp->organizationName(), qApp->applicationName());
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i)
    {
        QString fNum = QString::number(i + 1);
        if (i < 9)
            fNum.prepend(" ");
        QString text = tr("%1 %2").arg(fNum).arg(strippedName(files[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setStatusTip(files[i]);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);
}
//-------------------------------------------------------------------------------------------
QString SQLiteBrowser::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::on_cmdNewRecord_clicked(void)
{
    model->insertRows(model->rowCount(), 1);
    ui->tblData->resizeRowsToContents();
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::on_cmdDeleteRecord_clicked(void)
{
    if(model)
    {
        model->removeRows(ui->tblData->currentIndex().row(), 1);
        ui->tblData->resizeRowsToContents();

    }
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::on_cmdSaveChanges_clicked(void)
{
    ui->tblData->selectRow(ui->tblData->currentIndex().row());
    if (!model->submitAll())
    {
        int ret = QMessageBox::question(this, qApp->applicationName(),
                                        tr("There is a pending transaction in progress. That cannot be commited now."\
                                           "\nError: %1\n"\
                                           "Perform rollback?").arg(model->lastError().text()),
                                        QMessageBox::Yes, QMessageBox::No);
        if(ret == QMessageBox::Yes)
            model->revertAll();
        model->setPendingTransaction(false);
        return;
    }
    model->setPendingTransaction(false);
    ui->tblData->resizeRowsToContents();
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::on_cmdExecuteQuery_clicked(void)
{
    SqlQueryModel *dummyModel = new SqlQueryModel(this);
    ui->tblQueryResult->setModel(dummyModel);
    ui->txtQueryError->setText("");
    QString query = ui->txtQuery->toPlainText();
    myLog->refreshLog("user", query);
    if (!query.isEmpty())
    {
        SqlQueryModel *queryModel = new SqlQueryModel(this);
        queryModel->setQuery(query, db);
        if (!queryModel->lastError().isValid())
        {
            //No Error
            ui->tblQueryResult->setModel(queryModel);
            ui->tblQueryResult->resizeColumnsToContents();
            ui->tblQueryResult->resizeRowsToContents();
        }
        else
        {
            //Error occurred
            ui->txtQueryError->setText(queryModel->lastError().text());
        }
    }
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::newTable(void)
{
    CreateTableDialog createNewTable;
    createNewTable.db = db;
    if (createNewTable.exec())
    {
        reloadDbList();
    }
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::editTable(void)
{
    QString tableName = getTable();
    if (tableName != "")
    {
        CreateTableDialog changeTable;
        changeTable.db = db;
        changeTable.startAlterTable(tableName);
        if (changeTable.exec())
            reloadDbList();
    }
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::dropTable(void)
{
    QString tableName = getTable();
    if (tableName != "")
    {
        if (QMessageBox::question(this, tr("Confirm Drop"), tr("Are you sure you want to delete this table?\nCAUTION!!!You will lose all data."), QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
            return;
        QString dropSQL = QString("DROP TABLE '%1'").arg(tableName);
        db.exec(dropSQL);
        if (!db.lastError().isValid())
        {
            reloadDbList();
        }
        else
        {
            QMessageBox::critical(this, tr("Error"), tr("Drop Table Error:\n%1").arg(db.lastError().text()));
        }
    }
}
//-------------------------------------------------------------------------------------------
QString SQLiteBrowser::getTableNView(void)
{
    SelectObjectDialog selectTable(this, tr("Select an object"), tr("Select a table or view"));
    QStringList allItems;
    allItems  << tableList << viewList;
    selectTable.setItems(allItems);
    QString tableName = "";
    if (selectTable.exec())
    {
        tableName = selectTable.returnValue();
    }
    return tableName;
}
//-------------------------------------------------------------------------------------------
QString SQLiteBrowser::getTable(void)
{
    SelectObjectDialog selectTable(this, tr("Select a table"), tr("Select a table"));
    selectTable.setItems(tableList);
    QString tableName = "";
    if (selectTable.exec())
    {
        tableName = selectTable.returnValue();
    }
    return tableName;
}
//-------------------------------------------------------------------------------------------
QString SQLiteBrowser::getIndex(void)
{
    SelectObjectDialog selectIndex(this, tr("Select an Index"), tr("Select an Index"));
    selectIndex.setItems(indexList);
    QString indexName = "";
    if (selectIndex.exec())
    {
        indexName = selectIndex.returnValue();
    }
    return indexName;
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::dropIndex(void)
{
    QString indexName = getIndex();
    if (indexName != "")
    {
        if (QMessageBox::question(this, tr("Confirm Drop"), tr("Are you sure you want to delete this index?\nCAUTION!!!This action cannot be undone."), QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
            return;
        QString dropSQL = QString("DROP index '%1'").arg(indexName);
        db.exec(dropSQL);
        myLog->refreshLog("", dropSQL);
        if (!db.lastError().isValid())
        {
            reloadDbList();
        }
        else
        {
            QMessageBox::critical(this, tr("Error"), tr("Drop Table Error:\n%1").arg(db.lastError().text()));
        }
    }
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::createIndex(void)
{
    QString tableName = getTable();
    if (tableName != "")
    {
        CreateIndexDialog *newIndex = new CreateIndexDialog(this);
        connect (newIndex, SIGNAL(emitLog(QString,QString)), myLog, SLOT(refreshLog(QString,QString)));
        newIndex->db = db;
        newIndex->setTableName(tableName);
        if (newIndex->exec())
            reloadDbList();
    }
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::showPreferences(void)
{
    PreferencesDialog pref;
    if (pref.exec())
        preferencesChanged();
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::preferencesChanged(void)
{
    //Preferences not yet implemented
    //PreferencesDialog pref;
    //ui->txtQuery->setFont(pref.queryEditorFont);
    //ui->txtQuery->setFontPointSize(pref.queryEditorFontSize);
    //ui->lstStructure->setFont(QFont(pref.databaseExplorerFont.toString(), pref.databaseExplorerFontSize));
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::dragEnterEvent(QDragEnterEvent *event)
{
    if( event->mimeData()->hasFormat("text/uri-list") )
        event->acceptProposedAction();
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();

    if( urls.isEmpty() )
        return;

    QString fileName = urls.first().toLocalFile();

    if( !fileName.isEmpty())
        loadFile(fileName);
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::exportDatabaseAsSQL(void)
{
    QString exportFileName = QFileDialog::getSaveFileName();
    if (exportFileName.size() > 0) {
        QFile f(exportFileName);
        if (!f.open(QIODevice::WriteOnly))
        {
            QMessageBox::warning(this, qApp->applicationName(), tr("Could not create export file.\n%1").arg(f.errorString()));
        }
        else
        {
            QTextStream out(&f);
            QString sql ="";// "BEGIN TRANSACTION;\n";
            QSqlQuery qu("SELECT name FROM sqlite_master WHERE type='table'", db);
            while (qu.next())
            {
                QString tableName = qu.value(0).toString();
                sql += "DROP TABLE IF EXISTS `" + tableName + "`;\n";
                sql += getTableCreateSQL(tableName);
                sql += "\n";
                sql += getTableInsertSQL(tableName);
            }
            //sql += "COMMIT;";
            out << sql;
            f.close();
            QMessageBox::information(this, qApp->applicationName(), tr("Export completed"));
        }
    }
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::importDatabaseFromSQL(void)
{
    if (curFile.isEmpty())
    {
        newFile();
    }
    QString fileName = QFileDialog::getOpenFileName(this);
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("SQL Import"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }
    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    ui->txtQuery->setText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    db.exec(ui->txtQuery->toPlainText());
    ui->txtQuery->setText("");
    if (db.lastError().isValid())
        QMessageBox::warning(this, tr("SQL Import"), tr("Error:\n%1").arg(db.lastError().text()));
    QMessageBox::information(this, tr("SQL Import"), tr("SQL File imported successfully"));
    reloadDbList();
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::exportTableToCSV(void)
{
    QString tableName = getTableNView();
    QString exportFileName = QFileDialog::getSaveFileName();
    char quote = '"';
    char deliminiter = ',';
    QString endline = "\n";
    if (!exportFileName.isEmpty())
    {
        QFile f(exportFileName);
        if (f.open(QIODevice::WriteOnly))
        {
            QTextStream out(&f);
            //Have header as first line
            QString header = "";
            QSqlQuery headerQu("PRAGMA TABLE_INFO(`" + tableName + "`)", db);
            while (headerQu.next())
            {
                header +=  quote + headerQu.record().value("name").toString() + quote + deliminiter;
            }
            header = header.remove(header.size() - sizeof(deliminiter), sizeof(deliminiter));
            header += endline;
            out << header;
            //append all rows.
            QString rowText = "";
            QSqlQuery qu("SELECT * FROM `" + tableName + "`", db);
            while (qu.next())
            {
                QSqlRecord rec = qu.record();
                rowText = "";
                for (int i = 0; i < rec.count(); ++i)
                {
                    rowText += quote + rec.value(i).toString() + quote + deliminiter;
                }
                rowText = rowText.remove(rowText.size() - sizeof(deliminiter), sizeof(deliminiter));
                rowText += endline;
                out << rowText;
            }
            f.close();
            QMessageBox::information(this, tr("CSV Export"), tr("Export succeeded."));
        }
        else
        {
            QMessageBox::warning(this, tr("CSV Export"), tr("Export error.\n%1").arg(f.errorString()));
        }
    }
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::importTableFromCSV(void)
{
    QString wFile = QFileDialog::getOpenFileName(this, tr("Select a CSV File"), QDir::currentPath(), tr("CSV Files (*csv)"));

    if (QFile::exists(wFile))
    {
        ImportCSVDialog csv(this, db, wFile);
        if (csv.exec())
        {
            reloadDbList();
            QMessageBox::information(this, qApp->applicationName(), tr("Import completed" ));
        }
    }
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::compactDb(void)
{
    //Not Implemented
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::searchAll(void)
{
    if (ui->txtSearch->text().isEmpty())
    {
        QMessageBox::warning(this, qApp->applicationName(), tr("Please enter search text."));
        return;
    }
    QString search = ui->txtSearch->text();
    QString searchQuery = "SELECT * FROM `" + ui->cboTables->currentText() + "` WHERE ";
    QSqlQuery qu("PRAGMA TABLE_INFO(" + ui->cboTables->currentText() + ")", db);
    myLog->refreshLog("", "PRAGMA TABLE_INFO(" + ui->cboTables->currentText() + ")");
    while (qu.next())
    {
        QString colName = qu.record().value("name").toString();
        searchQuery.append("`" + colName + QString("` LIKE '\%%1\%' OR ").arg(search));
    }
    searchQuery = searchQuery.remove(searchQuery.size() - 4, 4);
    doSearch(searchQuery);
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::advancedSearch(void)
{
    AdvancedSearchDialog adv(this, db, ui->cboTables->currentText());
    if (adv.exec())
    {
        QString searchQuery = adv.searchQuery;
        doSearch(searchQuery);
    }
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::doSearch(QString searchQuery)
{
    myLog->refreshLog("", searchQuery);
    SqlQueryModel *queryModel = new SqlQueryModel(this);
    queryModel->setQuery(searchQuery, db);
    if (!queryModel->lastError().isValid())
    {
        //No Error
        ui->tblData->setModel(queryModel);
        ui->tblData->resizeRowsToContents();
        ui->cmdDeleteRecord->setEnabled(false);
        ui->cmdNewRecord->setEnabled(false);
        ui->cmdSaveChanges->setEnabled(false);
    }
    else
    {
        //Error occurred
        QMessageBox::critical(this, tr("Search Error"), queryModel->lastError().text());
    }

}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::showTable(QString tableName)
{
    model->setSchema("main");
    model->setTable(tableName);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    ui->tblData->setModel(model);
    ui->cmdDeleteRecord->setEnabled(true);
    ui->cmdNewRecord->setEnabled(true);
    ui->tblData->resizeRowsToContents();
    ui->cmdDeleteRecord->setEnabled(true);
    ui->cmdNewRecord->setEnabled(true);
    ui->cmdSaveChanges->setEnabled(true);
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::showTable(void)
{
    showTable(ui->cboTables->currentText());
}
//-------------------------------------------------------------------------------------------
QString SQLiteBrowser::getTableCreateSQL(QString tableName)
{
    QSqlQuery qu("SELECT sql FROM sqlite_master WHERE type='table' AND name = '" + tableName + "'", db);
    if (!qu.lastError().isValid())
    {
        if (qu.next())
        {
            QString sql = qu.value(0).toString();
            return sql + ";";
        }
        else
        {
            QMessageBox::warning(this, tr("Export Error"), tr("Error:\n%1").arg(qu.lastError().text()));
            return "";
        }
    }
    else
    {
        QMessageBox::warning(this, tr("Export Error"), qu.lastError().text());
        return "";
    }
}
//-------------------------------------------------------------------------------------------
QString SQLiteBrowser::getTableInsertSQL(QString tableName)
{
    QString sql = "";
    QSqlQuery qu("SELECT * FROM `" + tableName + "`", db);
    QSqlQuery qu2("PRAGMA TABLE_INFO(`" + tableName + "`);", db);
    QStringList types;
    while (qu2.next())
    {
        types.insert(types.count(), qu2.record().value("type").toString());
    }
    if (qu.lastError().isValid())
        return "";
    while (qu.next())
    {
        QSqlRecord rec = qu.record();
        sql += "INSERT INTO " + tableName + " VALUES (";
        for (int i = 0; i < rec.count(); ++i)
        {
            QString colData = rec.value(i).toString();
            colData = colData.replace("'", "''");
            colData = "'" + colData + "'";
            QString type = types.at(i);
            if (type.toLower().contains("int"))
                colData = rec.value(i).toString();
            sql += colData + ", ";
        }
        sql = sql.remove(sql.size() - 2, 2);
        sql += ");\n";
    }
    return sql;
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::createLanguageMenu(void)
{
    QActionGroup *langGroup = new QActionGroup(ui->menuLanguage);
    langGroup->setExclusive(true);
    connect (langGroup, SIGNAL(triggered(QAction*)), this, SLOT(slotLanguageChanged(QAction*)));
    QString defaultLocale = QLocale::system().name();   //e.g. "de_DE"
    defaultLocale.truncate(defaultLocale.indexOf('_')); //e.g. "de"
    m_langPath = QApplication::applicationDirPath();
    m_langPath.append("/languages");
    QDir dir(m_langPath);
    QStringList fileNames = dir.entryList(QStringList("sqlitebrowser_*.qm"));

    for (int i = 0; i < fileNames.size(); ++i)
    {
        //get locale extracted by filename
        QString locale;
        locale = fileNames[i];                      //SqliteBrowser_de_qm
        locale.truncate(locale.lastIndexOf('.'));   //SqliteBrowser_de
        locale.remove(0, locale.indexOf('_') + 1);  //de

        QString lang = QLocale::languageToString(QLocale(locale).language());
        QIcon ico(QString("%1/%2.png").arg(m_langPath).arg(locale));
        QAction *action = new QAction(lang, this);
        action->setCheckable(true);
        action->setData(locale);

        ui->menuLanguage->addAction(action);
        langGroup->addAction(action);

        if (defaultLocale == locale)
        {
            action->setChecked(true);
        }
    }
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::slotLanguageChanged(QAction *action)
{
    if (action != 0)
    {
        loadLanguage(action->data().toString());
    }
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::loadLanguage(const QString &rLanguage)
{
    if (m_currLang != rLanguage)
    {
        m_currLang = rLanguage;
        QLocale locale =QLocale(m_currLang);
        QLocale::setDefault(locale);
        QString languageName = QLocale::languageToString(locale.language());
        switchTranslator(m_translator, QString(qApp->applicationDirPath() + "/languages/sqlitebrowser_%1.qm").arg(rLanguage));
        switchTranslator(m_translatorQt, QString("qt_%1.qm").arg(rLanguage));
        ui->statusBar->showMessage(tr("Current language changed to %1").arg(languageName), 5000);
    }
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::switchTranslator(QTranslator &translator,
                                     const QString &fileName)
{
    qApp->removeTranslator(&translator);
    if (translator.load(fileName))
    {
        qApp->installTranslator(&translator);
    }
    else
    {
        // qDebug() << "Error";
    }
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::changeEvent(QEvent *event)
{
    if (event != 0)
    {
        switch (event->type())
        {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        case QEvent::LocaleChange:
        {
            QString locale = QLocale::system().name();
            locale.truncate(locale.lastIndexOf('_'));
            loadLanguage(locale);
        }
            break;
        default:
            break;
        }
    }
    QMainWindow::changeEvent(event);
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::newView(void)
{
    CreateViewDialog createView(this);
    createView.db = db;
    if (createView.exec())
    {
        reloadDbList();
    }
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::modifyView(void)
{
    QString viewName = getView();
    CreateViewDialog editView(this);
    editView.db = db;
    editView.setViewName(viewName);
    if (editView.exec()) {
        reloadDbList();
    }
}
//-------------------------------------------------------------------------------------------
void SQLiteBrowser::deleteView(void)
{
    QString viewName = getView();
    if (viewName == "")
        return;
    if (QMessageBox::question(this,
                              tr("Confirm Delete"),
                              tr("Are you sure you want to remove the <b>%1</b> view?").arg(viewName),
                              QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        db.exec(QString("DROP VIEW %1").arg(viewName));
        if (!db.lastError().isValid())
        {
            reloadDbList();
        }
        else
        {
            QMessageBox::critical(this, tr("Error"), tr("There was an error while deleting the view.\n%1").arg(viewName));
        }
    }
}
//-------------------------------------------------------------------------------------------
QString SQLiteBrowser::getView(void)
{
    SelectObjectDialog selectView(this, tr("Select a View"), tr("Select a View"));
    selectView.setItems(viewList);
    QString viewName = "";
    if (selectView.exec())
    {
        viewName = selectView.returnValue();
    }
    return viewName;
}
//-------------------------------------------------------------------------------------------
