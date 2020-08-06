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
#include "ui_findwindow_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "findwindow_mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef Q_OS_WIN
#   include "wingdi.h"
#   include "tchar.h"
#endif
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);
    createTestBar();
    updateText();
    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test", &MainBox::test });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    le_programm = new QLineEdit(this);
    le_programm->setObjectName("le_programm");
    connect(le_programm,    &QLineEdit::editingFinished,    this,   &MainBox::choice_test);
    testbar->addWidget(le_programm);

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok)
    {
        return;
    }
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
bool MainBox::test(void)
{
#ifdef Q_OS_LINUX
    emit error("Not for LINUX");
    return false;
#endif

#ifdef Q_OS_WIN
    RECT rc;
    // HWND hwnd = ::FindWindow(0, _T("Calculator"));
    HWND hwnd = ::FindWindow(0, reinterpret_cast<wchar_t *>(le_programm->text().data()));
    if (hwnd == NULL)
    {
        emit error("hwnd == NULL");
        return false;
    }
    GetClientRect(hwnd, &rc);

    //create
    HDC hdcScreen = GetDC(NULL);
    HDC hdc = CreateCompatibleDC(hdcScreen);
    HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen,
                                          rc.right - rc.left,
                                          rc.bottom - rc.top);
    SelectObject(hdc, hbmp);

    //Print to memory hdc
#if 1
    WINDOWINFO wi;
    GetWindowInfo(hwnd, &wi);

    BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hdcScreen, wi.rcClient.left, wi.rcClient.top, SRCCOPY);
#else
    // так не получится сфотографировать калькулятор
    bool ok = PrintWindow(hwnd, hdc, PW_CLIENTONLY);
    if(!ok)
    {
        emit error("PrintWindow return false");
    }
#endif

    //copy to clipboard
    OpenClipboard(NULL);
    EmptyClipboard();
    SetClipboardData(CF_BITMAP, hbmp);
    CloseClipboard();

    //release
    DeleteDC(hdc);
    DeleteObject(hbmp);
    ReleaseDC(NULL, hdcScreen);

    emit info(QString("%1 %2 %3 %4")
              .arg(rc.left)
              .arg(rc.top)
              .arg(rc.right)
              .arg(rc.bottom));
    emit info("OK");
#endif
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{
    le_programm->setText(load_string(PROGRAMM_NAME));
}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{
    save_string(PROGRAMM_NAME, le_programm->text());
}
//--------------------------------------------------------------------------------
