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
#ifndef LOGBOX_HPP
#define LOGBOX_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include<QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "mysettings.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
typedef struct
{
    QDate date;
    QTime time;
    int level;
    QColor color_text;
    QColor background_color;
    QString message;
} LOG_DATA;

enum LEVELS
{
    L_INFO = 0,
    L_DEBUG,
    L_ERROR,
    L_TRACE
};
//--------------------------------------------------------------------------------
class FindBox;
//--------------------------------------------------------------------------------
class LogBox : public QFrame, MySettings
{
    Q_OBJECT

public:
    explicit LogBox(QWidget *parent);
    explicit LogBox(const QString &o_name,
                    QWidget *parent,
                    int min_width  = 1,
                    int min_height = 1);
    ~LogBox();

    void load_settings(void);
    void save_settings(void);

    void set_o_name(QString value);

    void updateText(void);

public slots:
    void clear(void);
    void progress(int);
    void clearProgress(void);
    void setVisibleProgressBar(bool);
    void setColorLog(bool state);

    void  set_font(QFont font);
    QFont get_font(void);

    void infoLog(const QString &text);
    void debugLog(const QString &text);
    void errorLog(const QString &text);
    void traceLog(const QString &text);

    void colorLog(const QString &text,
                  const QColor text_color = QColor(Qt::black),
                  const QColor background_color = QColor(Qt::white));

    void set_flag_is_shows_info(bool state);
    void set_flag_is_shows_debug(bool state);
    void set_flag_is_shows_error(bool state);
    void set_flag_is_shows_trace(bool state);

    void syslogLog(QDateTime dt,
                   int level,
                   int src,
                   QString message);
    void syslogLog(int level,
                   QString src,
                   QString message);

    void update_log(void);

private slots:
    void changeOptions(void);
    void save_to(void);

    void popup(QPoint);

    void find_prev(const QString &text);
    void find_next(const QString &text);

private:
    QPointer<QTextEdit> logBox;

    QString o_name;

    bool flagNoCRLF = false;
    bool flagAddDateTime = false;
    bool flagColor = true;
    bool flagErrorAsMessage = false;
    bool flagTextIsWindows = false;
    bool flagAutoSave = false;

    bool flag_is_shows_info  = true;
    bool flag_is_shows_debug = true;
    bool flag_is_shows_error = true;
    bool flag_is_shows_trace = true;

#ifdef NEED_CODEC
    QTextCodec *current_codec = nullptr; //TODO проверить надо
#endif

    QPointer<QProgressBar> progressBar;
    QPointer<QHBoxLayout> hbox;
    QPointer<QVBoxLayout> vbox;
    QPointer<QHBoxLayout> mainbox;

    QPointer<FindBox> fb;

    QString autosave_filename = "noname.log";

    QList<LOG_DATA> l_log_data;

    void init(void);
    void create_widgets(void);

    void save_log(const QString &);

    void append_string(LOG_DATA log_data);
    QString syslog_to_str(int level);

protected:
    void changeEvent(QEvent *event);
    void keyPressEvent(QKeyEvent *event);
};
//--------------------------------------------------------------------------------
#endif // LOGBOX_HPP
