#ifndef COMMANDBAR_H
#define COMMANDBAR_H

#include <QtWidgets>

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

class CommandBar : public QFrame
{
    Q_OBJECT
    public:
        explicit CommandBar(QWidget *parent = 0);
        QLineEdit *textEdit;

    signals:
        void searchWeb();
        void searchWord();
        void goToWeb();
        void closeRequested();

    protected:
        void keyPressEvent(QKeyEvent *event);
        void resizeEvent(QResizeEvent *event);

    public slots:
        void setColor(QString color);
        void enableShadow(bool enable);
        void setFocus();
        void reset();

    private slots:
        void textEdited(QString text);
        void finishedEditing();

    private:
        QToolButton *btnSearchWeb;
        QToolButton *btnSearchWord;
        QToolButton *btnGoToWeb;
        QToolButton *btnExecCmd;
        QPushButton *btnClose;
        QTextEdit *helpMessage;
        QGraphicsDropShadowEffect *shadow;

};

#endif // COMMANDBAR_H
