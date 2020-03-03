/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#include "filemanager.hpp"
#include "ui_filemanager.h"

#include "myfiledialog.hpp"
#include "highlighter.hpp"
//--------------------------------------------------------------------------------
FileManager::FileManager(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::FileManager)
{
    init();
}
//--------------------------------------------------------------------------------
FileManager::~FileManager()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void FileManager::init(void)
{
    ui->setupUi(this);

    highlighter = new Highlighter(ui->te_filemanager->document());

    ui->btn_run->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_save->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogSaveButton));
    ui->btn_save_as->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogSaveButton));

    ui->btn_save->setToolTip("File save");
    ui->btn_save_as->setToolTip("File save as...");

    //TODO сначала модель присваивается виджету для просмотра
    model = new QFileSystemModel;
    model->setNameFilters(QStringList() << QString("*.%1").arg(extension));
    model->setNameFilterDisables(false);

    ui->tv_filemanager->setModel(model);

    ui->tv_filemanager->setColumnHidden(1, true);
    ui->tv_filemanager->setColumnHidden(2, true);
    ui->tv_filemanager->setColumnHidden(3, true);

    //TODO и только потом переходим на нужный каталог
    QModelIndex idx = model->setRootPath(QDir::currentPath());
    ui->tv_filemanager->setRootIndex(idx);

    ui->splitter->setCollapsible(0, false);
    ui->splitter->setCollapsible(1, false);

    QFont font("Courier", 10);
    ui->te_filemanager->setFont(font);
    ui->te_filemanager->setFont(font);

    // ширина TAB в символах
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    int fontWidth_html = QFontMetrics(ui->te_filemanager->currentCharFormat().font()).averageCharWidth();
    ui->te_filemanager->setTabStopWidth(3 * fontWidth_html);
#else
    ui->te_filemanager->setTabStopDistance(QFontMetricsF(ui->te_filemanager->font()).horizontalAdvance(' ') * 3);
#endif

    connect(ui->tv_filemanager, &QTreeView::doubleClicked,  this,   &FileManager::choice_file);
    connect(ui->btn_run,        &QToolButton::clicked,      this,   &FileManager::s_run);
    connect(ui->btn_run,        &QToolButton::clicked,      this,   &FileManager::run);

    connect(ui->btn_save,       &QToolButton::clicked,      this,   &FileManager::s_save);
    connect(ui->btn_save_as,    &QToolButton::clicked,      this,   &FileManager::s_save_as);

    //TODO
//    emit info(metaObject()->className());
//    emit info(objectName());
//    emit info(parent()->objectName());
//    emit info(metaObject()->classInfo(0).name());

//    QObject *object = new QPushButton;
//    const QMetaObject *metaobject = metaObject();
//    for (int i=0; i<metaobject->propertyCount(); ++i) {
//        QMetaProperty metaproperty = metaobject->property(i);
//        const char *name = metaproperty.name();
//        emit info(name);
//        const char *tname = metaproperty.typeName();
//        emit info(tname);
//        QVariant value = object->property(name);
//        qDebug()<<qPrintable(QString(name) + " " + QString(value.typeName()) + " " +
//                             QString(metaproperty.isDesignable()? "shown in Designer": "not show in Designer"));
//    }
}
//--------------------------------------------------------------------------------
void FileManager::choice_file(void)
{
    QString filename = model->filePath(ui->tv_filemanager->currentIndex());
    if(filename.right(extension.count()).toLower() == extension)
    {
        emit debug(filename);
        if(ui->te_filemanager->document()->isModified())
        {
            qDebug() << "HTML not saved";
            int btn = messagebox_question("Ошибка: файл был изменён", "Сохранить?");
            if(btn == QMessageBox::Yes)
            {
                save_file();
            }
            else
            {
                load_file(filename);
                //ui->tv_filemanager->setCurrentIndex()
            }
        }
        else
        {
            load_file(filename);
        }
    }
}
//--------------------------------------------------------------------------------
void FileManager::load_file(const QString &filename)
{
    if(filename.isEmpty())
    {
        emit error("load_file: filename is empty");
        return;
    }

    this->filename = filename;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    ui->te_filemanager->setPlainText(file.readAll());

    file.close();
}
//--------------------------------------------------------------------------------
void FileManager::save_file(void)
{
    if(filename.isEmpty())
    {
        emit error("Filename is empty");
        return;
    }

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        emit error("File not written");
        return;
    }
    qint64 bytes = file.write(ui->te_filemanager->toPlainText().toLocal8Bit(),
                              ui->te_filemanager->toPlainText().length());
    if(bytes != ui->te_filemanager->toPlainText().length())
    {
        emit error("Bad write!");
    }
    file.close();

    ui->te_filemanager->document()->setModified(false);
}
//--------------------------------------------------------------------------------
void FileManager::s_run(void)
{
    emit trace(Q_FUNC_INFO);

    const QString temp = ui->te_filemanager->toPlainText();
    if(temp.isEmpty())
    {
        emit error("no data");
        return;
    }
    emit set_data(temp);
    //ui->webEngineView->setHtml(temp);
}
//--------------------------------------------------------------------------------
void FileManager::s_save(void)
{
    save_file();
}
//--------------------------------------------------------------------------------
void FileManager::s_save_as(void)
{
    if(ui->te_filemanager->document()->isEmpty())
    {
        emit error("Nothing save...");
        return;
    }

    MyFileDialog *dlg = new MyFileDialog("filemanage", "filemanage", this);
    dlg->setNameFilter(QString("%1 files (*.%1)").arg(extension));
    dlg->selectFile("noname");
    dlg->setDefaultSuffix(extension);
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    int btn = dlg->exec();
    if(btn == MyFileDialog::Accepted)
    {
        QStringList files = dlg->selectedFiles();
        filename = files.at(0);
        save_file();
    }
}
//--------------------------------------------------------------------------------
void FileManager::set_extension(QString value)
{
    if(value.isEmpty() == false)
    {
        extension = value;
        model->setNameFilters(QStringList() << QString("*.%1").arg(extension));
    }
}
//--------------------------------------------------------------------------------
QString FileManager::get_data(void)
{
    return ui->te_filemanager->toPlainText();
}
//--------------------------------------------------------------------------------
bool FileManager::isModified(void)
{
    return ui->te_filemanager->document()->isModified();
}
//--------------------------------------------------------------------------------
void FileManager::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool FileManager::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void FileManager::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void FileManager::save_setting(void)
{

}
//--------------------------------------------------------------------------------
