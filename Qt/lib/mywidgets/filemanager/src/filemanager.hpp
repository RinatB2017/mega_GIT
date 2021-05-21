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
    void run(void);

public:
    explicit FileManager(QWidget *parent = nullptr);
    virtual ~FileManager();

    void set_extension(QString value);
    bool isModified(void);
    QString get_data(void);

private slots:
    void choice_file(void);
    void s_run(void);
    void s_save(void);
    void s_save_as(void);
    void s_cancel(void);
    void need_cancel(bool state);

private:
    Ui::FileManager *ui;
    QFileSystemModel *model = nullptr;
    Highlighter *highlighter = nullptr;
    QString extension = "txt";
    QString filename;

    void init(void);
    void load_file(const QString &filename);
    void save_file(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // FILEMANAGER_HPP
