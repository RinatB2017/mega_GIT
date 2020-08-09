/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#ifndef LOGDOCK_OPTIONS_HPP
#define LOGDOCK_OPTIONS_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
namespace Ui {
    class LogDock_options;
}
//--------------------------------------------------------------------------------
class LogDock_options : public QDialog
{
    Q_OBJECT

    //TODO не забыть, что название property должны совпадать с первым полем Q_PROPERTY
    Q_PROPERTY(bool flag_ReadOnly       READ get_readonly       WRITE set_readonly)
    Q_PROPERTY(bool flag_Color          READ get_color          WRITE set_color)
    Q_PROPERTY(bool flag_NoCRLF         READ get_noCRLF         WRITE set_noCRLF)
    Q_PROPERTY(bool flag_AddDateTime    READ get_datetime       WRITE set_datetime)
    Q_PROPERTY(bool flag_ErrorAsMessage READ get_show_error     WRITE set_show_error)

    Q_PROPERTY(bool flag_LOG_EMERG      READ get_LOG_EMERG      WRITE set_LOG_EMERG)
    Q_PROPERTY(bool flag_LOG_ALERT      READ get_LOG_ALERT      WRITE set_LOG_ALERT)
    Q_PROPERTY(bool flag_LOG_CRIT       READ get_LOG_CRIT       WRITE set_LOG_CRIT)
    Q_PROPERTY(bool flag_LOG_ERR        READ get_LOG_ERR        WRITE set_LOG_ERR)
    Q_PROPERTY(bool flag_LOG_WARNING    READ get_LOG_WARNING    WRITE set_LOG_WARNING)
    Q_PROPERTY(bool flag_LOG_NOTICE     READ get_LOG_NOTICE     WRITE set_LOG_NOTICE)
    Q_PROPERTY(bool flag_LOG_INFO       READ get_LOG_INFO       WRITE set_LOG_INFO)
    Q_PROPERTY(bool flag_LOG_DEBUG      READ get_LOG_DEBUG      WRITE set_LOG_DEBUG)

public:
    LogDock_options(QWidget *parent = nullptr);
    ~LogDock_options();

    bool setProperty(const char *name, const QVariant &value);

private slots:
    void set_readonly(bool value);
    void set_color(bool value);
    void set_noCRLF(bool value);
    void set_datetime(bool value);
    void set_show_error(bool value);

    bool get_readonly(void);
    bool get_color(void);
    bool get_noCRLF(void);
    bool get_datetime(void);
    bool get_show_error(void);

    void set_LOG_EMERG(bool value);
    void set_LOG_ALERT(bool value);
    void set_LOG_CRIT(bool value);
    void set_LOG_ERR(bool value);
    void set_LOG_WARNING(bool value);
    void set_LOG_NOTICE(bool value);
    void set_LOG_INFO(bool value);
    void set_LOG_DEBUG(bool value);

    bool get_LOG_EMERG(void);
    bool get_LOG_ALERT(void);
    bool get_LOG_CRIT(void);
    bool get_LOG_ERR(void);
    bool get_LOG_WARNING(void);
    bool get_LOG_NOTICE(void);
    bool get_LOG_INFO(void);
    bool get_LOG_DEBUG(void);

private:
    Ui::LogDock_options *ui;
    QStringList codecs;

    //---
#if 0
    typedef struct OPTIONS
    {
        int index;
        //QCheckBox cb;
        void (LogDock_options::*func_get)(QCheckBox *cb);
        void (LogDock_options::*func_set)(QCheckBox *cb);
    } *options_t;
    QList<OPTIONS> options;

    void get(QCheckBox *cb) {}
    void set(QCheckBox *cb) {}
#endif
    //---

    void init(void);
    void findCodecs(void);
    void show_objectname(void);
};
//--------------------------------------------------------------------------------
#endif // LOGDOCK_OPTIONS_HPP
