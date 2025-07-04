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
#ifndef LOGBOX_HPP
#define LOGBOX_HPP
//--------------------------------------------------------------------------------
#include <QProgressBar>
#include <QDateTime>
#include <QTextEdit>
#include <QColor>
#include <QFont>
#include <QDate>
#include <QTime>
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

//#define MAX_BLOCK_CNT 100000

#define P_FLAG_SHOW_INFO  "flag_show_info"
#define P_FLAG_SHOW_ERROR "flag_show_error"
#define P_FLAG_SHOW_DEBUG "flag_show_debug"
#define P_FLAG_SHOW_TRACE "flag_show_trace"
//--------------------------------------------------------------------------------
class FindBox;
//--------------------------------------------------------------------------------
class LogBox : public MyWidget
{
    Q_OBJECT

public:
    explicit LogBox(QWidget *parent);
    explicit LogBox(const QString &o_name,
                    QWidget *parent);
    virtual ~LogBox();

    void set_o_name(const QString &value);

    void set_flagNoCRLF(bool state);
    void set_flagAddDateTime(bool state);
    void set_flagColor(bool state);
    void set_flagErrorAsMessage(bool state);
    void set_flagTextIsWindows(bool state);
    void set_flagAutoSave(bool state);

public slots:
    void clear(void);
    void progress(int);
    void clearProgress(void);
    void setVisibleProgressBar(bool);
    void setColorLog(bool state);

    void  set_font(QFont font);
    QFont get_font(void);

    void infoLog(const  QString &text);
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
                   const QString &message);
    void syslogLog(int level,
                   const QString &src,
                   const QString &message);

    void update_log(void);

private slots:
    void changeOptions(void);
    void save_to(void);
    void save_full_log_to(void);

    void popup(QPoint);

    void find_prev(const QString &text);
    void find_next(const QString &text);

private:
    QTextEdit *logBox = nullptr;

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
    QTextCodec *current_codec = nullptr;
#endif

    QProgressBar *progressBar = nullptr;
    QHBoxLayout *hbox = nullptr;
    QVBoxLayout *vbox = nullptr;
    QHBoxLayout *mainbox = nullptr;

    FindBox *fb = nullptr;

    QString autosave_filename = "noname.log";

    QList<QAction *> app_actions;

    QList<LOG_DATA> l_log_data;
    QList<LOG_DATA> l_full_log_data;

    void init(void);
    void create_widgets(void);

    void slot_is_shows_info(bool state);
    void slot_is_shows_debug(bool state);
    void slot_is_shows_error(bool state);
    void slot_is_shows_trace(bool state);

    void save_logfile(const QDateTime &dt,
                      int level,
                      const QString log);
    void save_log(const QString &filename);
    void save_full_log(const QString &filename);

    void append_string(LOG_DATA log_data);
    QString syslog_to_str(int level);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);

protected:
    void changeEvent(QEvent *event);
    void keyPressEvent(QKeyEvent *event);
};
//--------------------------------------------------------------------------------
#endif // LOGBOX_HPP
