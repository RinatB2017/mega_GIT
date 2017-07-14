/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#ifndef TREEFILMS_HPP
#define TREEFILMS_HPP
//--------------------------------------------------------------------------------
#include <QDomElement>
#include <QTreeWidget>
#include <QDir>
//--------------------------------------------------------------------------------
class QProgressDialog;
class QXmlPut;
//--------------------------------------------------------------------------------
class TreeFilms : public QTreeWidget
{
    Q_OBJECT

public:
    explicit TreeFilms(QWidget *parent = 0);
    ~TreeFilms();
    
signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void set_status_text(const QString &);

    void click_film(const QString &);

public slots:
    void double_click(void);
    void save(void);

private slots:
    void slot_click_film(QTreeWidgetItem*, int);
    void slot_double_clicked(QTreeWidgetItem*, int);
    void expand_item(QTreeWidgetItem *item, int column);
    void popup(QPoint);

    void update_status_text(void);

    void slot_add_folder(void);
    void slot_del_folder(void);
    void slot_add_film(void);
    void slot_del_film(void);
    void slot_rename_action(void);
    void slot_scan(void);
    void slot_clear_data(void);

    void slot_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *);
    void slot_expand_item(QTreeWidgetItem *item, int);

private:
    bool is_modified = false;

    QIcon folderIcon;
    QIcon filmIcon;

    QStringList film_filters;
    QStringList picture_filters;
    QProgressDialog *progressDialog;
    int file_count;

    QString default_player;

    QXmlPut *xmlPut = 0;

    QDomDocument *doc = 0;

    void parseFolderElement(const QDomElement &element,
                            QTreeWidgetItem *parentItem = 0);
    QTreeWidgetItem *createItem(const QDomElement &element,
                                QTreeWidgetItem *parentItem = 0);
    QHash<QTreeWidgetItem *, QDomElement> domElementForItem;

    void connect_log(void);

    void init(void);
    void read(void);

    void add_folder(const QString &name);
    void del_folder(const QString &name);
    void add_film(const QString &path,
                  const QString &filename);
    void del_film(const QString &name);
    void create_rus_name(const QString &old_file_name,
                         const QString &dat);
    void rename(const QString &new_name);
    void scan(QDir dir);
    void clear_data(void);

    void dropEvent(QDropEvent *event);

    void scan_tree(QTreeWidgetItem *root_item);

    void execute_player(const QString &player_name,
                        QStringList params);

    int get_file_count(void);
};
//--------------------------------------------------------------------------------
#endif // TREEFILMS_HPP
