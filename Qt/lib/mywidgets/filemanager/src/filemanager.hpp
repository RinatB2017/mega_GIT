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
#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP
//--------------------------------------------------------------------------------
#include <QFileSystemModel>
#include <QUrl>
//--------------------------------------------------------------------------------
#include "myfiledialog.hpp"
#include "highlighter.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class FileManager;
}
//--------------------------------------------------------------------------------
class FileManager : public MyWidget
{
    Q_OBJECT

signals:
    void set_data(const QString& html, const QUrl& baseUrl = QUrl());
    void run();

public:
    explicit FileManager(QWidget *parent = nullptr);
    virtual ~FileManager();

    void set_extension(QString value);
    bool isModified();
    QString get_data();

private slots:
    void choice_file();
    void s_run();
    void s_save();
    void s_save_as();
    void s_cancel();
    void need_cancel(bool state);

private:
    Ui::FileManager *ui;
    QFileSystemModel *model = nullptr;
    Highlighter *highlighter = nullptr;
    QString extension = "txt";
    QString filename;

    void init();
    void load_file(const QString &filename);
    void save_file();

    void updateText();
    bool programm_is_exit();
    void load_setting();
    void save_setting();
};
//--------------------------------------------------------------------------------
#endif // FILEMANAGER_HPP
