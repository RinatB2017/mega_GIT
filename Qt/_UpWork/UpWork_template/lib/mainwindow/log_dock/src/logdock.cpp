/*********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#include <QMetaMethod>
#include <QTextEdit>
#include <QAction>
#include <QMenu>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "logdock.hpp"
//#include "logdock_options.hpp"
//--------------------------------------------------------------------------------
#include "logbox.hpp"
//--------------------------------------------------------------------------------
LogDock::LogDock(const QString &title,
                 QWidget *parent) :
    QDockWidget(parent)
{
    setWindowTitle(title);
    setObjectName(title);

    le = new LogBox(this);
    if(!le) return;

    setWidget(le);
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void LogDock::infoLog(const QString &text)
{
    le->infoLog(text);
}
//--------------------------------------------------------------------------------
void LogDock::debugLog(const QString &text)
{
    le->debugLog(text);
}
//--------------------------------------------------------------------------------
void LogDock::errorLog(const QString &text)
{
    le->errorLog(text);
}
//--------------------------------------------------------------------------------
void LogDock::messageLog(const QString &text)
{
    le->messageLog(text);
}
//--------------------------------------------------------------------------------
void LogDock::syslog(int level,
                     const QString &src,
                     const QString &message)
{
    le->syslogLog(level, src, message);
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void LogDock::load_settings(void)
{
    le->load_settings();
}
//--------------------------------------------------------------------------------
void LogDock::save_settings(void)
{
    le->save_settings();
}
//--------------------------------------------------------------------------------
void LogDock::load_html(void)
{
    le->load_html();
}
//--------------------------------------------------------------------------------
void LogDock::save_html(void)
{
    le->save_html();
}
//--------------------------------------------------------------------------------
void  LogDock::set_font(QFont font)
{
#ifndef NO_LOG
    le->set_font(font);
#else
    Q_UNUSED(font)
#endif
}
//--------------------------------------------------------------------------------
QFont LogDock::get_font(void)
{
#ifndef NO_LOG
    return le->get_font();
#else
    return QFont();
#endif
}
//--------------------------------------------------------------------------------
