#ifndef STARTERUSER_H
#define STARTERUSER_H

#include <QtWidgets>

#include "mainwindow.h"

/*
    This file is part of Dooscape.

    Dooscape is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Dooscape is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Dooscape.  If not, see <http://www.gnu.org/licenses/>.
*/

class TabWizard : public QFrame
{
    Q_OBJECT
    public:
        explicit TabWizard(QString title = 0, QWidget *parent = 0);

    signals:
        void goBackRequest();
        void goForwardRequest();

    public slots:
        void setBoxLayout(QLayout *layout);
        void setColor(QString color);
        void showBackButton(bool show);

    private:
        QLabel *lblTitle;
        QGridLayout *mainLayout;
        QPushButton *btnBack;
        QPushButton *btnForward;

};

class StarterUser : public QWidget
{
    Q_OBJECT
    public:
        explicit StarterUser(QWidget *parent = 0);

    signals:
        void finished();
        void editConfig();

    public slots:
        void start();
        void setMainWindow(MainWindow *window);

    private slots:
        void createWelcome();
        void createAccount();
        void createCustomize();
        void createFinish();

        void lookFields();
        void editColor();

        void finish();
        void showConfig();

        void saveSettings();

    private:
        QStackedLayout *layout;
        QSignalMapper *mapBack;
        QSignalMapper *mapNext;
        QColor color;

        TabWizard *tabWelcome;
        TabWizard *tabAccount;
        TabWizard *tabCustomize;
        TabWizard *tabFinish;

        QLineEdit *editPseudo;
        QLineEdit *editPass1;
        QLineEdit *editPass2;
        QLineEdit *editIndice;

};

#endif // STARTERUSER_H
