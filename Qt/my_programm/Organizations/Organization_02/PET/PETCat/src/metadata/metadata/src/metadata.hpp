/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
**                                                                              **
**     This program is free software: you can redistribute it and/or modify     **
**     it under the terms of the GNU General Public License as published by     **
**     the Free Software Foundation, either version 3 of the License, or        **
**     (at your option) any later version.                                      **
**                                                                              **
**     This program is distributed in the hope that it will be useful,          **
**     but WITHOUT ANY WARRANTY; without even the implied warranty of           **
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
**     GNU General Public License for more details.                             **
**                                                                              **
**     You should have received a copy of the GNU General Public License        **
**     along with this program.  If not, see http://www.gnu.org/licenses/.      **
**                                                                              **
**********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#ifndef METADATA_HPP
#define METADATA_HPP
//--------------------------------------------------------------------------------
#include <QSqlDatabase>
#include <QComboBox>
#include <QObject>
//--------------------------------------------------------------------------------
class QTreeWidgetItem;
class MyComboBox;
//--------------------------------------------------------------------------------
class MetaData : public QObject
{
    Q_OBJECT

public:
    explicit MetaData(QObject *parent = 0);
    ~MetaData();

    typedef struct METADATA
    {
        QString fullname;
        QString name;
    } METADATA_t;

    void set_root_item(QTreeWidgetItem *ri);
    bool find_data(QString name, QStringList *result);
    bool delete_data(QString name, QString fullname);

    bool load_data(MyComboBox *result);
    bool load_data(QTreeWidgetItem *result);

    bool check_exists_file(QString name, QString fullname);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void add_metadata_file(void);

public slots:
    bool view_database(void);

    bool f_add_metadata(void);
    bool f_del_metadata(QString name, QString fullname);

private slots:
    void item_clicked(QTreeWidgetItem *item, int);

private:
    QSqlDatabase db;
    QString driver_name;
    QString database_name;
    QString table_name;

    QTreeWidgetItem  *root_item;

    void init(void);

    bool open_database(void);
    bool create_new_database(void);
    bool read_database(QStringList *sl);
    bool read_database(QString name, QStringList *sl);
    bool delete_data_from_database(QString name, QString fullname);
    bool write_database(METADATA data);
    void close_database(void);

    bool read_database(QList<METADATA> *sl);

    bool view(const QString &query);
};
//--------------------------------------------------------------------------------
#endif // METADATA_HPP
