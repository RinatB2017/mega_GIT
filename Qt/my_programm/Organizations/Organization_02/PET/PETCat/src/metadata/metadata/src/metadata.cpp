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
#include <QApplication>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
//--------------------------------------------------------------------------------
#include <QFileInfo>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QTableView>
#include <QAction>
//--------------------------------------------------------------------------------
#include "metadata.hpp"
//--------------------------------------------------------------------------------
#include "myfiledialog.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "mycombobox.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MetaData::MetaData(QObject *parent) :
    QObject(parent)
{
    init();
}
//--------------------------------------------------------------------------------
MetaData::~MetaData()
{
    bool ok = db.isOpen();
    if(ok)
    {
        db.close();
    }
}
//--------------------------------------------------------------------------------
void MetaData::init(void)
{
    database_name   = "metadata.db";
    driver_name     = "QSQLITE";
    table_name      = "main";
}
//--------------------------------------------------------------------------------
bool MetaData::open_database(void)
{
    QString temp_str;
    if(db.databaseName() != database_name)
    {
        if(QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        {
            db = QSqlDatabase::database();
        }
        else
        {
            db = QSqlDatabase::addDatabase(driver_name);
            db.setDatabaseName(database_name);
        }
    }
    if(db.isOpen())
    {
        return true;
    }
    bool ok = db.open();
    if(ok == false)
    {
        temp_str = QString(tr("База данных %1 не открыта! Ошибка: %2"))
                .arg(database_name)
                .arg(db.lastError().text());
        emit error(temp_str);
        MyWidget::messagebox_critical("Ошибка", temp_str);
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool MetaData::create_new_database(void)
{
    QString temp_str;
    if(!db.isValid())
    {
        emit error("db not valid");
        return false;
    }

    QSqlQuery sql;
    bool ok = sql.exec(QString("CREATE TABLE %1 (fullname TEXT, name TEXT)").arg(table_name));
    if(!ok)
    {
        temp_str = QString("create_new_database: Ошибка: %1").arg(sql.lastError().text());
        emit error(temp_str);
        MyWidget::messagebox_critical("Ошибка", temp_str);
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool MetaData::view_database(void)
{
    if(!db.isValid())
    {
        emit error("db not valid");
        return false;
    }

    bool ok = false;
    if(db.isOpen() == false)
    {
        ok = open_database();
        if(!ok)
        {
            return false;
        }
    }

    return view(QString("SELECT * FROM %1").arg(table_name));
}
//--------------------------------------------------------------------------------
bool MetaData::read_database(QStringList *sl)
{
    QString temp_str;
    if(sl == nullptr)
    {
        return false;
    }

    if(!db.isValid())
    {
        emit error("db not valid");
        return false;
    }

    QSqlQuery sql;
    bool ok = sql.exec(QString("SELECT * FROM %1").arg(table_name));
    if(!ok)
    {
        temp_str = QString("read_database: Ошибка: %1").arg(sql.lastError().text());
        emit error(temp_str);
        MyWidget::messagebox_critical("Ошибка", temp_str);
    }

    (*sl).clear();
    while (sql.next())
    {
        QFileInfo fi;
        fi.setFile(sql.value(0).toString());
        QString name = fi.baseName();
        if(name.isEmpty() == false)
        {
            (*sl).append(name);
        }
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool MetaData::read_database(QList<METADATA> *sl)
{
    QString temp_str;
    if(sl == nullptr)
    {
        return false;
    }

    if(!db.isValid())
    {
        emit error("db not valid");
        return false;
    }

    QSqlQuery sql;
    bool ok = sql.exec(QString("SELECT * FROM %1").arg(table_name));
    if(!ok)
    {
        temp_str = QString("read_database: Ошибка: %1").arg(sql.lastError().text());
        emit error(temp_str);
        MyWidget::messagebox_critical("Ошибка", temp_str);
    }

    (*sl).clear();
    while (sql.next())
    {
        METADATA m_data;
        m_data.fullname = sql.value(0).toString();
        m_data.name = sql.value(1).toString();

        //TODO
        QFileInfo fi(m_data.fullname);
        if(fi.exists() == false)
        {
            emit debug(QString("file %1 not exists").arg(m_data.fullname));
            ok = delete_data_from_database(m_data.name, m_data.fullname);
            if(ok == false)
            {
                emit error(QString("Не удалось удалить %1 из метаданных").arg(m_data.fullname));
            }
        }
        else
        {
            (*sl).append(m_data);
        }
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool MetaData::read_database(QString name, QStringList *sl)
{
    QString temp_str;
    if(sl == nullptr)
    {
        return false;
    }

    if(!db.isValid())
    {
        emit error("db not valid");
        return false;
    }

    QSqlQuery sql;
    bool ok = sql.exec(QString("SELECT fullname, name FROM %1 WHERE name like '%2'")
                       .arg(table_name)
                       .arg(name));
    if(!ok)
    {
        temp_str = QString("read_database: Ошибка: %1").arg(sql.lastError().text());
        emit error(temp_str);
        MyWidget::messagebox_critical("Ошибка", temp_str);
        return false;
    }

    (*sl).clear();
    while (sql.next())
    {
        QString name = sql.value(0).toString();
        if(name.isEmpty() == false)
        {
            (*sl).append(name);
        }
    }
    return true;
}
//--------------------------------------------------------------------------------
bool MetaData::delete_data_from_database(QString name, QString fullname)
{
    QString temp_str;
    if(!db.isValid())
    {
        emit error("db not valid");
        return false;
    }

    QSqlQuery sql;
    bool ok = sql.exec(QString("DELETE FROM %1 WHERE (name like '%2' and fullname like '%3')")
                       .arg(table_name)
                       .arg(name)
                       .arg(fullname));
    emit debug(QString("delete_data_from_database: [%1]").arg(sql.lastQuery()));
    if(!ok)
    {
        temp_str = QString("delete_data_from_database: Ошибка: %1").arg(sql.lastError().text());
        emit error(temp_str);
        MyWidget::messagebox_critical("Ошибка", temp_str);
    }
    emit debug("OK");
    return ok;
}
//--------------------------------------------------------------------------------
bool MetaData::write_database(METADATA data)
{
    QString temp_str;
    if(!db.isValid())
    {
        emit error("db not valid");
        return false;
    }

    emit debug(QString("write_database: table_name %1").arg(table_name));
    qDebug() << "table_name" << table_name;

    QSqlQuery query;
    query.prepare(QString("INSERT INTO %1 (fullname, name) VALUES (:fullname, :name)").arg(table_name));
    query.bindValue(":fullname",    data.fullname);
    query.bindValue(":name",        data.name);
    bool ok = query.exec();
    if(!ok)
    {
        temp_str = QString("query [%1] not exec!\n").arg(query.lastQuery());
        temp_str.append(QString("write_database: Ошибка: %1").arg(query.lastError().text()));
        emit error(temp_str);
        MyWidget::messagebox_critical("Ошибка", temp_str);

        ok = create_new_database();
    }
    return ok;
}
//--------------------------------------------------------------------------------
void MetaData::close_database(void)
{
    if(!db.isValid())
    {
        emit error("db not valid");
        return;
    }
    db.close();
}
//--------------------------------------------------------------------------------
bool MetaData::view(const QString &query)
{
    if(!db.isValid())
    {
        emit error("db not valid");
        return false;
    }

    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(QString(query));

    QTableView *table_view = 0;
    table_view = new QTableView();
    if(table_view == nullptr)
    {
        emit error("table_view == nullptr");
        return false;
    }

    table_view->setWindowTitle("view table");
    table_view->setMinimumSize(1024, 600);

    table_view->setModel(model);

    QHeaderView *header = new QHeaderView(Qt::Horizontal);
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
    table_view->setHorizontalHeader(header);

    table_view->show();
    return true;
}
//--------------------------------------------------------------------------------
bool MetaData::f_add_metadata(void)
{
    emit debug("f_add_metadata");
    //---
    QString temp_str;
    QString filename;
    QFileInfo file;

    MyFileDialog *dlg = new MyFileDialog("filedialog_states", "md_dlg");
    QStringList filters;
    filters << "Файлы данных (*.dat3)"
            << "Все файлы (*.*)";
    dlg->setNameFilters(filters);
    dlg->setDefaultSuffix(tr("dat3"));
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        filename = files.at(0);
    }
    dlg->deleteLater();
    if(filename.isEmpty())
    {
        emit debug("filename is empty");
        return false;
    }
    //---

    file.setFile(filename);

    if(root_item == nullptr)
    {
        emit error("root_item == nullptr");
        return false;
    }

    MetaData::METADATA data;
    data.fullname = filename;
    data.name = file.baseName();

    bool ok = false;

    ok = open_database();
    if(!ok)
    {
        return false;
    }

    ok = check_exists_file(file.baseName(), filename);
    if(ok)
    {
        temp_str = "Такой файл уже есть!";
        emit error(temp_str);
        MyWidget::messagebox_critical("Ошибка", temp_str);

        close_database();
        return false;
    }

    ok = write_database(data);
    if(!ok)
    {
        close_database();
        return false;
    }
    close_database();

    load_data(root_item);

    //---
    emit info(QString("Добавлен файл [%1]").arg(filename));
    //---
    return true;
}
//--------------------------------------------------------------------------------
bool MetaData::f_del_metadata(QString name, QString fullname)
{
    bool ok = false;
    ok = open_database();
    if(!ok)
    {
        return false;
    }
    ok = delete_data_from_database(name, fullname);
    if(!ok)
    {
        return false;
    }
    close_database();

    load_data(root_item);
    return true;
}
//--------------------------------------------------------------------------------
void MetaData::item_clicked(QTreeWidgetItem *item, int)
{
    emit debug(item->text(0));
}
//--------------------------------------------------------------------------------
void MetaData::set_root_item(QTreeWidgetItem *ri)
{
    if(ri != nullptr)
    {
        root_item = ri;
    }
}
//--------------------------------------------------------------------------------
bool MetaData::load_data(MyComboBox *result)
{
    bool ok = open_database();
    if(!ok)
    {
        return false;
    }

    QList<METADATA> sl;
    ok = read_database(&sl);
    if(!ok)
    {
        create_new_database();
        return false;
    }

    //---
    QString  r_text = (*result).currentText();
    QVariant r_data = (*result).currentData(Qt::UserRole);
    //---
    (*result).clear();
    foreach (METADATA m_data, sl)
    {
        (*result).addItem(QIcon(NAME_DATABASE_ICON),
                          m_data.name,          //text
                          ID_METADATA_DELETE);  //userdata
    }
    //---
    for(int n=0; n<(*result).count(); n++)
    {
        (*result).setCurrentIndex(n);
        if(((*result).currentText() == r_text) && ((*result).currentData() == r_data))
        {
            break;
        }
    }
    //---

    close_database();
    return true;
}
//--------------------------------------------------------------------------------
bool MetaData::load_data(QTreeWidgetItem *result)
{
    bool ok = open_database();
    if(!ok)
    {
        return false;
    }

    QList<METADATA> sl;
    ok = read_database(&sl);
    if(!ok)
    {
        create_new_database();
        return false;
    }
    //---
    while((*result).childCount() > 0)
    {
        (*result).removeChild((*result).child(0));
    }
    foreach (METADATA m_data, sl)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(*result);
        item->setText(0, m_data.name);
        item->setToolTip(0, m_data.fullname);
        item->setStatusTip(0, m_data.fullname);
        item->setData(0, Qt::UserRole, ID_METADATA_DELETE);
        item->setData(1, Qt::UserRole, m_data.name);
        item->setData(2, Qt::UserRole, m_data.fullname);
        item->setIcon(0, QIcon(NAME_DATABASE_ICON));
        (*result).addChild(item);
    }
    //---
    QTreeWidgetItem *a_item = new QTreeWidgetItem(*result);
    a_item->setText(0,          NAME_METADATA_APPEND);
    a_item->setToolTip(0,       NAME_METADATA_APPEND);
    a_item->setStatusTip(0,     NAME_METADATA_APPEND);
    a_item->setData(0, Qt::UserRole, ID_METADATA_APPEND);
    a_item->setIcon(0, qApp->style()->standardIcon(QStyle::SP_DirOpenIcon));
    (*result).addChild(a_item);
    //---
    close_database();
    return true;
}
//--------------------------------------------------------------------------------
bool MetaData::check_exists_file(QString name, QString fullname)
{
    QString temp_str;
    if(name.isEmpty())
    {
        return false;
    }
    if(fullname.isEmpty())
    {
        return false;
    }

    if(!db.isValid())
    {
        emit error("db not valid");
        return false;
    }

    QSqlQuery sql;
    bool ok = sql.exec(QString("SELECT fullname, name FROM %1 WHERE (name like '%2' and fullname like '%3')")
                       .arg(table_name)
                       .arg(name)
                       .arg(fullname));
    if(!ok)
    {
        temp_str = QString("check_exists_file: Ошибка: %1").arg(sql.lastError().text());
        emit error(temp_str);
        MyWidget::messagebox_critical("Ошибка", temp_str);
        return false;
    }

    ok = sql.next();
    if(ok)
    {
        return true;
    }

    return false;
}
//--------------------------------------------------------------------------------
bool MetaData::find_data(QString name, QStringList *result)
{
    bool ok = open_database();
    if(!ok)
    {
        return false;
    }

    ok = read_database(name, result);
    close_database();
    return ok;
}
//--------------------------------------------------------------------------------
bool MetaData::delete_data(QString name, QString fullname)
{
    bool ok = open_database();
    if(!ok)
    {
        return false;
    }

    ok = delete_data_from_database(name, fullname);
    close_database();
    return ok;
}
//--------------------------------------------------------------------------------
