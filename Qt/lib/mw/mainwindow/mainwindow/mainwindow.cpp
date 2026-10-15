/*********************************************************************************
**                                                                              **
**     Copyright (C) 2025                                                       **
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
#include "mainwindow.hpp"
//--------------------------------------------------------------------------------
#ifdef DEF_USE_SANITIZER
#include <sanitizer/asan_interface.h>

// ASan автоматически вызовет эту функцию до функции main()
extern "C" const char* __asan_default_options() {
    return "abort_on_error=1:detect_leaks=0";
    // return "abort_on_error=1";
}
#endif
//--------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    CreatorMenus(parent)
{
#ifdef DEF_GET_SCREENSHOT
    if (server.listen(QHostAddress::Any, 8080))
    {
        // qDebug() << "Server started on port 8080";
    }
    else
    {
        // qDebug() << "Failed to start server";
    }
#endif

#ifdef DEF_REMOTE_CONTROL
    if (m_remoteConsole.startServer())
    {
        // qDebug() << "Удаленная консоль успешно запущена.";
    }
    else
    {
        // qDebug() << "Не удалось запустить сервер. Порт занят?";
    }
#endif

#ifdef QT_DEBUG
#ifdef Q_OS_WIN
    qApp->setStyleSheet(
        "QPushButton, QToolButton, QLineEdit, QCheckBox, QGroupBox, QLabel, QSlider, QFrame, QSpinBox, QTextEdit, QComboBox {"
        // "   background-color: #FF9999;"
        // "   border: 1px solid #000000;"
        "   background-color: #26c85a;"
        "   color: white;"
        "}"
        );
#endif
#endif

#ifdef Q_OS_WIN
    installEventFilter(this);
#endif
}
//--------------------------------------------------------------------------------
MainWindow::~MainWindow()
{

}
//--------------------------------------------------------------------------------
#ifdef Q_OS_WIN
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == this)
    {
        if (event->type() == QEvent::Move ||
                event->type() == QEvent::Resize)
        {
            // Откладываем проверку, чтобы сработала после всех внутренних пересчётов Qt
            QTimer::singleShot(0, this, &MainWindow::ensureOnScreen);
        }
    }
    return QMainWindow::eventFilter(obj, event);
}
#endif
//--------------------------------------------------------------------------------
#ifdef Q_OS_WIN
void MainWindow::ensureOnScreen()
{
    const QRect winRect = frameGeometry();
    bool visible = false;
    for (QScreen *s : QGuiApplication::screens())
    {
        if (s->availableGeometry().intersects(winRect))
        {
            visible = true;
            break;
        }
    }
    if (!visible)
    {
        // Возвращаем в центр основного экрана
        QScreen *primary = QGuiApplication::primaryScreen();
        QRect sg = primary->availableGeometry();
        move(sg.center() - rect().center());
    }
}
#endif
//--------------------------------------------------------------------------------
