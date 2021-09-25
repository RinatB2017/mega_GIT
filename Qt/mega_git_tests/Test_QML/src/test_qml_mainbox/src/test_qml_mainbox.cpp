/*********************************************************************************
**                                                                              **
**     Copyright (C) 2021                                                       **
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
#include "test_qml_mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MainBox_GUI(parent, splash)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{

}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
#ifdef QT_DEBUG
    createTestBar();
#endif

    QTimer::singleShot(0, [this]{
        add_menu();
    });

    connect(this,   &MainBox_GUI::btn_show_qml,
            this,   &MainBox::show_qml);
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;

    auto cmd_it = std::find_if(
        commands.begin(),
        commands.end(),
        [cmd](CMD command){ return command.cmd == cmd; }
    );
    if (cmd_it != commands.end())
    {
        typedef bool (MainBox::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (this->*x)();
        }
        else
        {
            emit error("no func");
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test", &MainBox::test });

    testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    cb_test->setProperty(NO_SAVE, true);
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));
}
//--------------------------------------------------------------------------------
void MainBox::add_menu(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    if(mw)
    {
        QMenu *menu = new QMenu(this);
        menu->setTitle("QML");

        QAction *a_new      = new QAction(this);
        QAction *a_load     = new QAction(this);
        QAction *a_save     = new QAction(this);
        QAction *a_save_as  = new QAction(this);

        a_new->setText("New");
        a_load->setText("Load");
        a_save->setText("Save");
        a_save_as->setText("Save As...");

        a_new->setIcon(qApp->style()->standardIcon(QStyle::SP_FileDialogNewFolder));
        a_load->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogOpenButton));
        a_save->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogSaveButton));
        a_save_as->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogSaveButton));

        connect(a_new,      &QAction::triggered,    this,   &MainBox::new_qml);
        connect(a_load,     &QAction::triggered,    this,   &MainBox::load_qml);
        connect(a_save,     &QAction::triggered,    this,   &MainBox::save_qml);
        connect(a_save_as,  &QAction::triggered,    this,   &MainBox::save_qml_as);

        menu->addAction(a_new);
        menu->addAction(a_load);
        menu->addAction(a_save);
        menu->addAction(a_save_as);

        mw->add_menu(1, menu);
    }
}
//--------------------------------------------------------------------------------
void MainBox::new_qml(void)
{
    if(QFile::copy(":/qml/default.qml", filename) == false)
    {
        messagebox_critical("ERROR", QString("file %1 not created").arg(filename));
    }
}
//--------------------------------------------------------------------------------
void MainBox::load_qml(void)
{
    MyFileDialog *dlg = new MyFileDialog("qml_options", "qml_options", this);
    dlg->setNameFilter("QML files (*.qml)");
    dlg->setDefaultSuffix("qml");
    int btn = dlg->exec();
    if(btn == MyFileDialog::Accepted)
    {
        QStringList files = dlg->selectedFiles();
        QString t_filename = files.at(0);

        if(!t_filename.isEmpty())
        {
            filename = t_filename;
            QFile file(filename);
            if(file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                set_qml_text(file.readAll());
                file.close();
            }
        }
    }
    delete dlg;
}
//--------------------------------------------------------------------------------
void MainBox::save_qml(void)
{
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(get_qml_text().toLocal8Bit());
        file.close();
    }
}
//--------------------------------------------------------------------------------
void MainBox::save_qml_as(void)
{
    MyFileDialog *dlg = new MyFileDialog("qml_options", "qml_options", this);
    dlg->setNameFilter("QML files (*.qml)");
    dlg->setDefaultSuffix("qml");
    dlg->setOption(MyFileDialog::DontConfirmOverwrite, false);
    int btn = dlg->exec();
    if(btn == MyFileDialog::Accepted)
    {
        QStringList files = dlg->selectedFiles();
        QString t_filename = files.at(0);

        if(!t_filename.isEmpty())
        {
            filename = t_filename;

            QFile file(filename);
            if(file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                file.write(get_qml_text().toLocal8Bit());
                file.close();
            }
        }
    }
    delete dlg;
}
//--------------------------------------------------------------------------------
void MainBox::show_qml(void)
{
    QString qml_text = get_qml_text();
    if(qml_text.isEmpty())
    {
        emit error("No data");
        return;
    }

    QFile file(filename);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(qml_text.toLocal8Bit());
        file.close();

        QQuickView *view = new QQuickView();
#if 0
        QString contentPath = QApplication::applicationDirPath();
        //emit debug(contentPath);
        view->setSource(QUrl::fromLocalFile(contentPath + "/" + filename));
#else
        view->setSource(QUrl::fromLocalFile(filename));
#endif
        view->show();
    }
}
//--------------------------------------------------------------------------------
bool MainBox::test(void)
{
    emit info("Test");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
