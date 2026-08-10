#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtSql>
#include "database.h"
#include "sqlmodels.h"
#include <QSettings>
#include "createtabledialog.h"
#include "aboutdialog.h"
#include "logdialog.h"
#include "highlighter.h"
#include "selectobjectdialog.h"
#include "createindexdialog.h"
#include "preferencesdialog.h"
#include "advancedsearchdialog.h"
#include "importcsvdialog.h"
#include "createviewdialog.h"

namespace Ui {
    class SQLiteBrowser;
}

class QActionGroup;

class SQLiteBrowser : public QMainWindow
{
    Q_OBJECT
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void changeEvent(QEvent *);

protected slots:
    void slotLanguageChanged(QAction *action);

public:
    explicit SQLiteBrowser(QWidget *parent = nullptr);
    ~SQLiteBrowser();

private:
    Ui::SQLiteBrowser *ui;
    QActionGroup *tableActions;
    QActionGroup *indexActions;
    QActionGroup *databaseActions;
    QSqlDatabase db;
    SqlTableModel *model;
    LogDialog *myLog;
    Highlighter *highlighter;
    QStringList tableList;
    QStringList indexList;
    QStringList viewList;
    void loadLanguage(const QString& rLanguage);
    void createLanguageMenu(void);
    QTranslator m_translator;
    QTranslator m_translatorQt;
    QString m_currLang;
    QString m_langPath;

private slots:
    void on_cmdExecuteQuery_clicked(void);
    void on_cmdSaveChanges_clicked(void);
    void on_cmdDeleteRecord_clicked(void);
    void on_cmdNewRecord_clicked(void);
    void showTable(QString tableName);
    void showTable(void);
    void newFile(void);
    void open(void);
    bool save(void);
    bool saveAs(void);
    void about(void);
    void aboutQt(void);
    void closeFile(void);
    void openRecentFile(void);
    void newTable(void);
    void editTable(void);
    void dropTable(void);
    void dropIndex(void);
    void createIndex(void);
    void showPreferences(void);
    void preferencesChanged(void);
    void exportDatabaseAsSQL(void);
    void importDatabaseFromSQL(void);
    void exportTableToCSV(void);
    void importTableFromCSV(void);
    void compactDb(void);
    void searchAll(void);
    void advancedSearch(void);
    QString getTableCreateSQL(QString tableName);
    QString getTableInsertSQL(QString tableName);
    void reloadDbList(void);
    void newView(void);
    void modifyView(void);
    void deleteView(void);

private:
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    QString curFile;
    void updateRecentFileActions();
    enum { MaxRecentFiles = 5 };
    QAction *recentFileActs[MaxRecentFiles];
    QString getTable(void);
    QString getTableNView(void);
    QString getIndex(void);
    QString getView(void);
    void doSearch(QString searchQuery);
    void switchTranslator(QTranslator &translator, const QString &fileName);
};

#endif
