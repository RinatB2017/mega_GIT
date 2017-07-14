/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#ifndef LOGBOX_H
#define LOGBOX_H
//--------------------------------------------------------------------------------
#include <QFrame>
#include <QList>
//--------------------------------------------------------------------------------
class QProgressBar;
class QPushButton;
class QToolButton;
class QHBoxLayout;
class QVBoxLayout;
class QTextEdit;
class QCheckBox;
class QString;
//--------------------------------------------------------------------------------
class LogBox : public QFrame
{
    Q_OBJECT

public:
    explicit LogBox(QWidget *parent = 0,
                    unsigned int min_width  = 1,
                    unsigned int min_height = 1);
    ~LogBox();

    void load_settings(void);
    void save_settings(void);

    void updateText(void);

public slots:
    void append(const QString &);
    void bappend(const QByteArray &);
    void clear(void);
    void progress(int);
    void clearProgress(void);
    void setVisibleProgressBar(bool);
    void setColorLog(bool state);
#ifndef NO_LOG
    void  set_font(QFont font);
    QFont get_font(void);
#endif

    void load_html(void);
    void save_html(void);

    void infoLog(const QString &text);
    void debugLog(const QString &text);
    void errorLog(const QString &text);
    void traceLog(const QString &text);

    void syslogLog(int level,
                   QString src,
                   QString message);

private slots:
    void changeOptions(void);
    void save_to(void);

    void popup(QPoint);

private:
    QTextEdit *logBox = 0;

    bool flagNoCRLF = false;
    bool flagAddDateTime = false;
    bool flagColor = false;
    bool flagAutoLoad = false;
    bool flagAutoSave = false;
    bool flagErrorAsMessage = false;
    bool flagTextIsWindows = false;

    QProgressBar *progressBar = 0;
    QHBoxLayout *hbox = 0;
    QVBoxLayout *vbox = 0;
    QHBoxLayout *mainbox = 0;

    bool old_state_btn_show_Info = false;
    bool old_state_btn_show_Debug = false;
    bool old_state_btn_show_Error = false;
    bool old_state_btn_show_Message = false;

    void init(void);
    void create_widgets(void);

    void save_log(const QString &);

    QString syslog_to_str(int level);

protected:
    void changeEvent(QEvent *event);

};
//--------------------------------------------------------------------------------
#endif // LOGBOX_H
