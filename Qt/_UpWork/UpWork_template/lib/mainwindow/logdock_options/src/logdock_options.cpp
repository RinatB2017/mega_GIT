/*********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#include "logdock_options.hpp"
#include "ui_logdock_options.h"
//--------------------------------------------------------------------------------
LogDock_options::LogDock_options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogDock_options)
{
    ui->setupUi(this);

    connect(ui->btn_ok,     SIGNAL(clicked(bool)),  this,   SLOT(accept()));
    connect(ui->btn_cancel, SIGNAL(clicked(bool)),  this,   SLOT(reject()));

    connect(ui->cb_readonly,    SIGNAL(toggled(bool)),  this,   SLOT(set_readonly(bool)));

    connect(ui->cb_EMERG,   SIGNAL(toggled(bool)),  this,   SLOT(set_LOG_EMERG(bool)));
    connect(ui->cb_ALERT,   SIGNAL(toggled(bool)),  this,   SLOT(set_LOG_ALERT(bool)));
    connect(ui->cb_CRIT,    SIGNAL(toggled(bool)),  this,   SLOT(set_LOG_CRIT(bool)));
    connect(ui->cb_ERR,     SIGNAL(toggled(bool)),  this,   SLOT(set_LOG_ERR(bool)));
    connect(ui->cb_WARN,    SIGNAL(toggled(bool)),  this,   SLOT(set_LOG_WARNING(bool)));
    connect(ui->cb_NOTICE,  SIGNAL(toggled(bool)),  this,   SLOT(set_LOG_NOTICE(bool)));
    connect(ui->cb_INFO,    SIGNAL(toggled(bool)),  this,   SLOT(set_LOG_INFO(bool)));
    connect(ui->cb_DEBUG,   SIGNAL(toggled(bool)),  this,   SLOT(set_LOG_DEBUG(bool)));

    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
LogDock_options::~LogDock_options()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void LogDock_options::set_readonly(bool value)
{
    m_readonly = value;
    ui->cb_readonly->setChecked(value);
}
//--------------------------------------------------------------------------------
bool LogDock_options::get_readonly(void)
{
    return m_readonly;
}
//--------------------------------------------------------------------------------
void LogDock_options::set_LOG_EMERG(bool value)
{
    m_LOG_EMERG = value;
    ui->cb_EMERG->setChecked(value);
}
//--------------------------------------------------------------------------------
void LogDock_options::set_LOG_ALERT(bool value)
{
    m_LOG_ALERT = value;
    ui->cb_ALERT->setChecked(value);
}
//--------------------------------------------------------------------------------
void LogDock_options::set_LOG_CRIT(bool value)
{
    m_LOG_CRIT = value;
    ui->cb_CRIT->setChecked(value);
}
//--------------------------------------------------------------------------------
void LogDock_options::set_LOG_ERR(bool value)
{
    m_LOG_ERR = value;
    ui->cb_ERR->setChecked(value);
}
//--------------------------------------------------------------------------------
void LogDock_options::set_LOG_WARNING(bool value)
{
    m_LOG_WARNING = value;
    ui->cb_WARN->setChecked(value);
}
//--------------------------------------------------------------------------------
void LogDock_options::set_LOG_NOTICE(bool value)
{
    m_LOG_NOTICE = value;
    ui->cb_NOTICE->setChecked(value);
}
//--------------------------------------------------------------------------------
void LogDock_options::set_LOG_INFO(bool value)
{
    m_LOG_INFO = value;
    ui->cb_INFO->setChecked(value);
}
//--------------------------------------------------------------------------------
void LogDock_options::set_LOG_DEBUG(bool value)
{
    m_LOG_DEBUG = value;
    ui->cb_DEBUG->setChecked(value);
}
//--------------------------------------------------------------------------------
bool LogDock_options::get_LOG_EMERG(void)
{
    return m_LOG_EMERG;
}
//--------------------------------------------------------------------------------
bool LogDock_options::get_LOG_ALERT(void)
{
    return m_LOG_ALERT;
}
//--------------------------------------------------------------------------------
bool LogDock_options::get_LOG_CRIT(void)
{
    return m_LOG_CRIT;
}
//--------------------------------------------------------------------------------
bool LogDock_options::get_LOG_ERR(void)
{
    return m_LOG_ERR;
}
//--------------------------------------------------------------------------------
bool LogDock_options::get_LOG_WARNING(void)
{
    return m_LOG_WARNING;
}
//--------------------------------------------------------------------------------
bool LogDock_options::get_LOG_NOTICE(void)
{
    return m_LOG_NOTICE;
}
//--------------------------------------------------------------------------------
bool LogDock_options::get_LOG_INFO(void)
{
    return m_LOG_INFO;
}
//--------------------------------------------------------------------------------
bool LogDock_options::get_LOG_DEBUG(void)
{
    return m_LOG_DEBUG;
}
//--------------------------------------------------------------------------------
