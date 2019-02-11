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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "treefilms.hpp"
#include "selectbox.hpp"
#include "qxmlputget.h"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
TreeFilms::TreeFilms(QWidget *parent) :
    QTreeWidget(parent),
    is_modified(false)
{
    init();
}
//--------------------------------------------------------------------------------
TreeFilms::~TreeFilms()
{
    qDebug() << currentItem()->text(0);
    if(is_modified)
    {
        int button = QMessageBox::critical(this, QObject::tr("save"), QObject::tr("save?"), QMessageBox::Yes, QMessageBox::No);
        if(button == QMessageBox::Yes)
        {
            save();
        }
    }
}
//--------------------------------------------------------------------------------
void TreeFilms::connect_log(void)
{
    if(topLevelWidget())
    {
        connect(this, SIGNAL(info(QString)),    topLevelWidget(), SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)),   topLevelWidget(), SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)),   topLevelWidget(), SIGNAL(error(QString)));
        connect(this, SIGNAL(trace(QString)),   topLevelWidget(), SIGNAL(trace(QString)));
    }
}
//--------------------------------------------------------------------------------
void TreeFilms::init(void)
{
    connect_log();

    film_filters = QStringList() << "*.mpg"
                                 << "*.mp4"
                                 << "*.avi"
                                 << "*.mov"
                                 << "*.mkv";
    picture_filters = QStringList() << "*.jpg"
                                    << "*.png";

    folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon), QIcon::Normal, QIcon::Off);
    folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),   QIcon::Normal, QIcon::On);

    filmIcon = QIcon(":/mainwindow/mplayer.png");

    setAcceptDrops(true);
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::InternalMove);

    read();

    //------
    QString current_film = parentWidget()->property("current_item").toString();
    qDebug() << "current_film =" << current_film;
    if(current_film.isEmpty() == false)
    {
        QList<QTreeWidgetItem *> items = findItems(current_film, Qt::MatchRecursive);
        foreach (QTreeWidgetItem *item, items)
        {
            if(item->data(0, ROLE_IS_FOLDER).toBool() == false)
                setCurrentItem(item);
        }
    }
    //------
    QTreeWidgetItem *tli = topLevelItem(0);
    if(tli)
    {
        QTreeWidgetItemIterator it(tli);
        int cnt = 0;
        while (*it)
        {
            if(*it != nullptr)
            {
                if((*it)->data(0, ROLE_IS_FOLDER).toBool() == false)
                    cnt++;
            }
            ++it;
        }
        qDebug() << "find" << cnt;
    }
    else
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(this);
        item->setData(0, ROLE_IS_FOLDER, true);
        item->setIcon(0, folderIcon);
        item->setText(0, "Фильмы");

        addTopLevelItem(item);
    }
    //------

    setContextMenuPolicy(Qt::CustomContextMenu);

    setHeaderLabel(QObject::tr("films"));

    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setSortingEnabled(true);
    sortByColumn(0, Qt::AscendingOrder);

    setMinimumSize(MAX_WIDTH, MAX_HEIGHT);

    expandItem(topLevelItem(0));

    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(slot_click_film(QTreeWidgetItem*, int)));
    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(slot_expand_item(QTreeWidgetItem*, int)));
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),       this, SLOT(popup(QPoint)));
    connect(this, SIGNAL(itemActivated(QTreeWidgetItem*, int)),     this, SLOT(expand_item(QTreeWidgetItem*, int)));
    connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),   this, SLOT(slot_currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
}
//--------------------------------------------------------------------------------
void TreeFilms::slot_expand_item(QTreeWidgetItem *item, int)
{
    if(item->isExpanded())
        collapseItem(item);
    else
        expandItem(item);
}
//--------------------------------------------------------------------------------
void TreeFilms::slot_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *)
{
    slot_click_film(current, 0);
}
//--------------------------------------------------------------------------------
void TreeFilms::expand_item(QTreeWidgetItem *item, int column)
{
    if(currentItem()->data(0, ROLE_IS_FOLDER).toBool() == false)
    {
        slot_click_film(item, column);
        execute_player(default_player,
                       QStringList() << currentItem()->data(0, ROLE_FILENAME).toString());
    }

    if(item->isExpanded())
        collapseItem(item);
    else
        expandItem(item);
}
//--------------------------------------------------------------------------------
void TreeFilms::read(void)
{
    doc = new QDomDocument("films");
    QFile file(FILMS_XML);
    if(!file.open(QIODevice::ReadOnly))
        return;
    if(!doc->setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root = doc->documentElement();

    QDomElement player = root.firstChildElement("player");
    if(player.text().isEmpty())
        default_player = "/usr/bin/kaffeine";
    else
        default_player = player.text();

    qDebug() << player.text();

    QDomElement child = root.firstChildElement("folder");
    file_count = 0;
    while(!child.isNull())
    {
        parseFolderElement(child);
        child = child.nextSiblingElement("folder");
    }
    QTimer::singleShot(0, this, SLOT(update_status_text()));
}
//--------------------------------------------------------------------------------
void TreeFilms::update_status_text(void)
{
    QString temp = QString(QObject::tr("find %1 films").arg(file_count));
    emit set_status_text(temp);
}
//--------------------------------------------------------------------------------
void TreeFilms::save(void)
{
    is_modified = false;
    qDebug() << "save()";
    xmlPut = new QXmlPut("Catalog");
    xmlPut->putString("player", default_player);
    for(int n=0; n<topLevelItemCount(); n++)
    {
        QTreeWidgetItem *root = topLevelItem(n);
        xmlPut->descend("folder");
        xmlPut->setAttributeString("name", root->text(0));
        scan_tree(root);
        xmlPut->rise();
    }
    xmlPut->save(FILMS_XML);
}
//--------------------------------------------------------------------------------
void TreeFilms::scan_tree(QTreeWidgetItem *root_item)
{
    for(int n=0; n<root_item->childCount(); n++)
    {
        if(root_item->child(n)->data(0, ROLE_IS_FOLDER).toBool())
        {
            xmlPut->descend("folder");
            xmlPut->setAttributeString("name", root_item->child(n)->text(0));
            scan_tree(root_item->child(n));
            xmlPut->rise();
        }
        else
        {
            xmlPut->descend("film");
            xmlPut->setAttributeString("name", root_item->child(n)->text(0));
            xmlPut->putString("filename", root_item->child(n)->data(0, ROLE_FILENAME).toString());
            xmlPut->putString("picture", root_item->child(n)->data(0, ROLE_PICTURE).toString());
            xmlPut->rise();
        }
    }
}
//--------------------------------------------------------------------------------
void TreeFilms::double_click(void)
{
    if(currentItem()->data(0, ROLE_IS_FOLDER).toBool() == false)
    {
        QString filename = currentItem()->data(0, ROLE_FILENAME).toString();
        if(!filename.isEmpty())
        {
            execute_player(default_player, QStringList() << filename);
        }
    }
}
//--------------------------------------------------------------------------------
void TreeFilms::slot_click_film(QTreeWidgetItem *, int)
{
    if(currentItem()->data(0, ROLE_IS_FOLDER).toBool())
    {
        emit click_film(FOLDER_FILE);
    }
    else
    {
        QString temp = currentItem()->text(0);
        if(temp.isEmpty() == false)
            parentWidget()->setProperty("current_item", temp);

        emit click_film(currentItem()->data(0, ROLE_PICTURE).toString());
    }
}
//--------------------------------------------------------------------------------
void TreeFilms::slot_double_clicked(QTreeWidgetItem *item, int column)
{
    if(item->data(0, ROLE_IS_FOLDER).toBool())
        return;

    QString filename = item->data(column, ROLE_FILENAME).toString();
    if(!filename.isEmpty())
    {
        execute_player(default_player,
                       QStringList() << filename);
    }
}
//--------------------------------------------------------------------------------
QTreeWidgetItem *TreeFilms::createItem(const QDomElement &element,
                                       QTreeWidgetItem *parentItem)
{
    QTreeWidgetItem *item;
    if(parentItem)
    {
        item = new QTreeWidgetItem(parentItem);
        //item->setFlags(item->flags() | Qt::ItemIsEditable);
        //item->setFlags(item->flags() & ~Qt::ItemIsDropEnabled);
    }
    else
    {
        item = new QTreeWidgetItem(this);
        //item->setFlags(item->flags() | Qt::ItemIsEditable);
        //item->setFlags(item->flags() & ~Qt::ItemIsDropEnabled);
    }
    domElementForItem.insert(item, element);
    return item;
}
//--------------------------------------------------------------------------------
void TreeFilms::parseFolderElement(const QDomElement &element,
                                   QTreeWidgetItem *parentItem)
{
    QTreeWidgetItem *item = createItem(element, parentItem);
    item->setIcon(0, folderIcon);
    item->setText(0, element.attribute("name"));
    item->setData(0, ROLE_IS_FOLDER, true);

    QDomElement child = element.firstChildElement();
    while(child.isNull() == false)
    {
        if(child.nodeName() == "folder")
        {
            parseFolderElement(child, item);
        }
        if(child.nodeName() == "film")
        {
            QTreeWidgetItem *childItem = createItem(child, item);
            childItem->setIcon(0, filmIcon);
            childItem->setText(0, child.attribute("name"));
            childItem->setData(0, ROLE_IS_FOLDER, false);
            childItem->setData(0, ROLE_NAME, child.attribute("name"));
            childItem->setData(0, ROLE_FILENAME, child.firstChildElement("filename").text());
            childItem->setData(0, ROLE_PICTURE, child.firstChildElement("picture").text());
            file_count++;
        }
        child = child.nextSiblingElement();
    }
}
//--------------------------------------------------------------------------------
void TreeFilms::add_folder(const QString &name)
{
    if(name.isEmpty()) return;

    is_modified = true;
    QTreeWidgetItem *item = new QTreeWidgetItem(currentItem());
    item->setData(0, ROLE_IS_FOLDER, true);
    item->setIcon(0, folderIcon);
    item->setText(0, name);
}
//--------------------------------------------------------------------------------
void TreeFilms::del_folder(const QString &name)
{
    if(name.isEmpty()) return;

    is_modified = true;
    qDebug() << "delete" << name;
}
//--------------------------------------------------------------------------------
void TreeFilms::add_film(const QString &path,
                         const QString &filename)
{
    QDir dir(path);
    QString pict_name;

    if(path.isEmpty()) return;
    if(filename.isEmpty()) return;

    QStringList picture_list = dir.entryList(picture_filters);
    QString full_name = QFileInfo(filename).fileName();

    QString file_name = QString("%1/%2")
            .arg(path)
            .arg(full_name);
    QFileInfo fi(full_name);
    pict_name = SPACE_FILE;
    if(picture_list.count() > 0)
    {
        if(picture_list.count() > 1)
        {
            SelectBox *selectBox = new SelectBox(this, "caption", path, file_name, picture_list);
            int btn = selectBox->exec();
            if(btn == QDialog::Accepted)
            {
                pict_name = selectBox->selected_pictures();
            }
        }
        else
        {
            pict_name = QString("%1/%2")
                    .arg(path)
                    .arg(picture_list[0]);
        }
    }

    //------
    QString filmname = full_name.left(full_name.length()-fi.suffix().length()-1);
    QString rus_name = QString("%1/%2.rus")
            .arg(path)
            .arg(filmname);
    QFile langfile(rus_name);
    if (langfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream data(&langfile);
        QString temp = data.readLine();
        if(temp.isEmpty() == false)
            filmname = temp;
        langfile.close();
    }
    //------

    QTreeWidgetItem *item = new QTreeWidgetItem(currentItem());
    item->setIcon(0, filmIcon);
    item->setText(0, filmname);
    item->setData(0, ROLE_IS_FOLDER, false);
    item->setData(0, ROLE_NAME, filmname);
    item->setData(0, ROLE_FILENAME, file_name);
    item->setData(0, ROLE_PICTURE, pict_name);

    file_count++;
    emit update_status_text();
}
//--------------------------------------------------------------------------------
void TreeFilms::del_film(const QString &name)
{
    if(name.isEmpty()) return;

    is_modified = true;

    qDebug() << "del_film" << name;
    file_count--;

    int index;
    QTreeWidgetItem *parent = currentItem()->parent();
    if (parent)
    {
        index = parent->indexOfChild(currentItem());
        delete parent->takeChild(index);
    }
    else
    {
        index = indexOfTopLevelItem(currentItem());
        delete takeTopLevelItem(index);
    }
    emit itemClicked(currentItem(), 0);

    emit update_status_text();
}
//--------------------------------------------------------------------------------
void TreeFilms::create_rus_name(const QString &old_file_name,
                                const QString &data)
{
    QFileInfo fi(old_file_name);
    QString rus_name = old_file_name.left(old_file_name.length()-fi.suffix().length()-1) + ".rus";
    qDebug() << rus_name;

    QFile file(rus_name);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(data.toLocal8Bit());
        file.close();
    }
}
//--------------------------------------------------------------------------------
void TreeFilms::rename(const QString &new_name)
{
    if(new_name.isEmpty()) return;

    is_modified = true;

    QTreeWidgetItem *item = currentItem();
    item->setText(0, new_name);
}
//--------------------------------------------------------------------------------
void TreeFilms::scan(QDir dir)
{
    if(!dir.exists()) return;

    is_modified = true;
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot); // | QDir::NoSymLinks);
    QStringList film_list = dir.entryList(film_filters);
    for(int i=0; i<film_list.count(); i++)
    {
        progressDialog->setValue(file_count);
        progressDialog->setLabelText(QString(QObject::tr("scan %1 of %2 files"))
                                     .arg(file_count)
                                     .arg(progressDialog->maximum()));
        QCoreApplication::processEvents();

        QString path = dir.absolutePath();
        QString filename = film_list[i];
        qDebug() << path << filename;
        add_film(path, filename);
    }

    dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot); // | QDir::NoSymLinks);
    QStringList dirList = dir.entryList();
    for (int i=0; i<dirList.size(); ++i)
    {
        QString newPath = QString("%1/%2").arg(dir.absolutePath()).arg(dirList.at(i));
        scan(QDir(newPath));
    }
}
//--------------------------------------------------------------------------------
void TreeFilms::clear_data(void)
{
    is_modified = true;
    clear();
    file_count = 0;
    emit update_status_text();
}
//--------------------------------------------------------------------------------
void TreeFilms::popup(QPoint)
{
    QMenu *popup_menu = new QMenu(this);

    QAction *add_folder_action = new QAction(qApp->style()->standardIcon(QStyle::SP_TrashIcon),
                                             QObject::tr("add folder"),
                                             this);
    QAction *del_folder_action = new QAction(qApp->style()->standardIcon(QStyle::SP_TrashIcon),
                                             QObject::tr("del folder"),
                                             this);
    QAction *add_film_action = new QAction(qApp->style()->standardIcon(QStyle::SP_TrashIcon),
                                           QObject::tr("add film"),
                                           this);
    QAction *del_film_action = new QAction(qApp->style()->standardIcon(QStyle::SP_TrashIcon),
                                           QObject::tr("del film"),
                                           this);
    QAction *rename_action = new QAction(qApp->style()->standardIcon(QStyle::SP_TrashIcon),
                                         QObject::tr("rename"),
                                         this);
    QAction *scan_action = new QAction(qApp->style()->standardIcon(QStyle::SP_TrashIcon),
                                       QObject::tr("scan"),
                                       this);
    QAction *clear_action = new QAction(qApp->style()->standardIcon(QStyle::SP_TrashIcon),
                                        QObject::tr("clear films"),
                                        this);

    popup_menu->addAction(scan_action);
    popup_menu->addSeparator();
    popup_menu->addAction(add_film_action);
    popup_menu->addAction(del_film_action);
    popup_menu->addSeparator();
    popup_menu->addAction(add_folder_action);
    popup_menu->addAction(del_folder_action);
    popup_menu->addSeparator();
    popup_menu->addAction(rename_action);
    popup_menu->addSeparator();
    popup_menu->addAction(clear_action);

    connect(add_folder_action,  SIGNAL(triggered()), this, SLOT(slot_add_folder()));
    connect(del_folder_action,  SIGNAL(triggered()), this, SLOT(slot_del_folder()));
    connect(add_film_action,    SIGNAL(triggered()), this, SLOT(slot_add_film()));
    connect(del_film_action,    SIGNAL(triggered()), this, SLOT(slot_del_film()));
    connect(rename_action,      SIGNAL(triggered()), this, SLOT(slot_rename_action()));
    connect(scan_action,        SIGNAL(triggered()), this, SLOT(slot_scan()));
    connect(clear_action,       SIGNAL(triggered()), this, SLOT(slot_clear_data()));

    popup_menu->exec(QCursor::pos());
}
//--------------------------------------------------------------------------------
void TreeFilms::slot_add_folder(void)
{
    bool ok;
    QString text = QInputDialog::getText(this,
                                         QObject::tr("slot_add_folder"),
                                         QObject::tr("Folder name:"),
                                         QLineEdit::Normal,
                                         QObject::tr("new folder"),
                                         &ok);
    if (ok && !text.isEmpty())
    {
        add_folder(text);
    }
}
//--------------------------------------------------------------------------------
void TreeFilms::slot_del_folder(void)
{
    QTreeWidgetItem *folder = currentItem();
    if(folder->data(0, ROLE_IS_FOLDER).toBool() == false)
    {
        QMessageBox::critical(this, "slot_del_folder", "Это не каталог");
        return;
    }
    if(folder->childCount())
    {
        int button = QMessageBox::question(this,
                                           QObject::tr("del folders"),
                                           QObject::tr("Dir not empty. Clean all subdir?"),
                                           QMessageBox::No,
                                           QMessageBox::Yes);
        if(button == QMessageBox::Yes)
        {
            qDebug() << "del" << folder->childCount();
            while(folder->childCount())
                folder->removeChild(folder->child(0));
        }
        return;
    }
    QString temp = QString(tr("del folder [%1]?")).arg(currentItem()->text(0));
    int res = QMessageBox::question(this, "slot_del_folder", temp, QMessageBox::No, QMessageBox::Yes);
    if(res != QMessageBox::Yes)
        return;

    del_folder(currentItem()->text(0));
}
//--------------------------------------------------------------------------------
void TreeFilms::slot_add_film(void)
{
    QString filters;

    filters.clear();
    filters.append("Films (");
    foreach (QString filter, film_filters)
    {
        filters.append(QString("%1 ").arg(filter));
    }
    filters.append(")");

    QFileDialog *dlg = new QFileDialog();
    dlg->setWindowTitle(QObject::tr("add film"));
    dlg->setNameFilter(filters);
    //dlg->setFilters(film_filters);
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    int button = dlg->exec();
    if(button != QFileDialog::Accepted) return;

    add_film(dlg->directory().absolutePath(),
             dlg->selectedFiles().at(0));
}
//--------------------------------------------------------------------------------
void TreeFilms::slot_del_film(void)
{
    QTreeWidgetItem *folder = currentItem();
    if(folder->data(0, ROLE_IS_FOLDER).toBool() == true)
    {
        QMessageBox::critical(this, "slot_del_film", "Это не фильм");
        return;
    }

    QString temp = QString(tr("del film [%1]?")).arg(currentItem()->text(0));
    int button = QMessageBox::question(this, "slot_del_film", temp, QMessageBox::No, QMessageBox::Yes);
    if(button != QMessageBox::Yes)
        return;

    qDebug() << selectedItems().count();
    del_film(currentItem()->text(0));
}
//--------------------------------------------------------------------------------
void TreeFilms::slot_rename_action(void)
{
    bool ok;
    QString text = QInputDialog::getText(this,
                                         QObject::tr("slot_rename_action"),
                                         QObject::tr("New name:"),
                                         QLineEdit::Normal,
                                         currentItem()->text(0),
                                         &ok);
    if (ok && !text.isEmpty())
    {
        create_rus_name(currentItem()->data(0, ROLE_FILENAME).toString(), text);
        rename(text);
    }
}
//--------------------------------------------------------------------------------
void TreeFilms::slot_scan(void)
{
    QFileDialog *dlg = new QFileDialog(this);
    dlg->setFileMode(QFileDialog::DirectoryOnly);
    dlg->setWindowTitle(QObject::tr("Select dir for scan"));
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    int button = dlg->exec();
    if(button != QFileDialog::Accepted) return;

    QIcon icon(ICON_PROGRAMM);
    progressDialog = new QProgressDialog(this);
    progressDialog->setWindowIcon(icon);
    progressDialog->setWindowTitle(QObject::tr("scan"));
    progressDialog->setMinimumWidth(320);
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->setCancelButtonText(QObject::tr("cancel"));
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    progressDialog->setShown(true);
#else
    progressDialog->setVisible(true);
#endif

    progressDialog->setLabelText(QObject::tr("prepare files..."));
    QDirIterator iterator(dlg->directory().absolutePath(),
                          film_filters,
                          QDir::Files | QDir::NoSymLinks,
                          QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);
    int cnt = 0;
    while(iterator.hasNext())
    {
        QCoreApplication::processEvents();
        cnt++;
        iterator.next();
    }
    qDebug() << "found" << cnt;

    //file_count = 0;
    progressDialog->setMaximum(cnt + file_count - 1);

    scan(QDir(dlg->directory().absolutePath()));
}
//--------------------------------------------------------------------------------
void TreeFilms::slot_clear_data(void)
{
    clear_data();
}
//--------------------------------------------------------------------------------
void TreeFilms::execute_player(const QString &player_name,
                               QStringList params)
{
    qDebug() << "execute_player!!!";
    if(player_name.isEmpty()) return;
    if(params.count() == 0) return;

    QProcess process;
    bool result = process.startDetached(player_name, params);
    if(result == false)
        QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Process cannot be started"));
}
//--------------------------------------------------------------------------------
int TreeFilms::get_file_count(void)
{
    return file_count;
}
//--------------------------------------------------------------------------------
#if 0
void TreeFilms::dragEnterEvent(QDragEnterEvent *event)
{
    foreach (QString filter, film_filters)
    {
        foreach(QUrl url, event->mimeData()->urls())
        {
            if (QFileInfo(url.toLocalFile()).suffix().toUpper() == filter.right(3).toUpper())
            {
                event->acceptProposedAction();
                return;
            }
        }
    }
}
#endif
//--------------------------------------------------------------------------------
void TreeFilms::dropEvent(QDropEvent *event)
{
    QTreeWidgetItem* item = itemFromIndex(indexAt(event->pos()));

    bool is_folder = item->data(0, ROLE_IS_FOLDER).toBool();
    if(is_folder)
    {
        QTreeWidget::dropEvent(event);
    }

#if 0
    foreach (QString filter, film_filters)
    {
        foreach(QUrl url, event->mimeData()->urls())
        {
            QString filename = url.toLocalFile();
            QString suffix = QFileInfo(filename).suffix().toUpper();
            if(suffix == filter.right(3).toUpper())
            {
                event->acceptProposedAction();
                QString path = QFileInfo(filename).absolutePath();
                add_film(path, filename);
                continue;
            }
        }
    }
#endif
}
//--------------------------------------------------------------------------------
