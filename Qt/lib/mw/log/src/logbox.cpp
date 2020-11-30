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
#include "mainwindow.hpp"
//--------------------------------------------------------------------------------
#include "myfiledialog.hpp"
#include "log_options.hpp"
#include "findbox.hpp"
#include "defines.hpp"
#include "logbox.hpp"
//--------------------------------------------------------------------------------
//#include "LoggingCategories.hpp"
//--------------------------------------------------------------------------------
LogBox::LogBox(QWidget *parent) :
    QFrame(parent),
    MySettings(),
    o_name("logbox")
{
    init();
}
//--------------------------------------------------------------------------------
LogBox::LogBox(const QString &o_name,
               QWidget *parent) :
    QFrame(parent),
    MySettings(),
    o_name(o_name)
{
    init();
}
//--------------------------------------------------------------------------------
LogBox::~LogBox()
{
    Q_ASSERT(logBox);
    Q_ASSERT(progressBar);
    Q_ASSERT(fb);

    MyWidget::set_param(P_LOG, FLAG_SHOW_INFO,   flag_is_shows_info);
    MyWidget::set_param(P_LOG, FLAG_SHOW_ERROR,  flag_is_shows_error);
#ifndef NO_LOG_DEBUG
    MyWidget::set_param(P_LOG, FLAG_SHOW_DEBUG,  flag_is_shows_debug);
#endif
#ifndef NO_LOG_TRACE
    MyWidget::set_param(P_LOG, FLAG_SHOW_TRACE,  flag_is_shows_trace);
#endif

    //qDebug() << "logbox is closed!";

    save_settings();

    if(logBox)
    {
        logBox->disconnect();
        logBox->deleteLater();
    }
    if(progressBar)
    {
        progressBar->disconnect();
        progressBar->deleteLater();
    }
    if(fb)
    {
        fb->disconnect();
        fb->deleteLater();
    }
}
//--------------------------------------------------------------------------------
void LogBox::init(void)
{
    setObjectName("LogBox");

#ifdef NEED_CODEC
    //TODO проверить надо
    current_codec = QTextCodec::codecForLocale();
#endif

    //FIXME говнокод
    QVariant v_flag_show_info   = true;
    QVariant v_flag_show_debug  = true;
    QVariant v_flag_show_error  = true;
    QVariant v_flag_show_trace  = true;

    MyWidget::get_param(P_LOG, FLAG_SHOW_INFO,   true,   &v_flag_show_info);
    MyWidget::get_param(P_LOG, FLAG_SHOW_DEBUG,  true,   &v_flag_show_debug);
    MyWidget::get_param(P_LOG, FLAG_SHOW_ERROR,  true,   &v_flag_show_error);
    MyWidget::get_param(P_LOG, FLAG_SHOW_TRACE,  true,   &v_flag_show_trace);

    flag_is_shows_info  = v_flag_show_info.toBool();
    flag_is_shows_debug = v_flag_show_debug.toBool();
    flag_is_shows_error = v_flag_show_error.toBool();
    flag_is_shows_trace = v_flag_show_trace.toBool();

#ifdef NO_LOG_DEBUG
    flag_is_shows_debug = false;
#endif
#ifdef NO_LOG_TRACE
    flag_is_shows_trace = false;
#endif
    //---

    create_widgets();

    if(o_name.isEmpty() == false)
    {
        load_settings();
    }

    logBox->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(logBox, &LogBox::customContextMenuRequested,    this,   &LogBox::popup);

    logBox->document()->setMaximumBlockCount(1000);
}
//--------------------------------------------------------------------------------
void LogBox::popup(QPoint)
{
    QMenu *popup_menu = logBox->createStandardContextMenu();
    if(popup_menu == nullptr)
    {
        //errorLog("cannot create standard context menu");
        popup_menu = new QMenu;
    }

    popup_menu->setStyleSheet("background:white;color:black;");

    QAction *clear_action   = new QAction(tr("clear"),   this);
    QAction *save_to_action = new QAction(tr("save to"), this);
    QAction *save_full_log_to_action = new QAction(tr("save full log to"), this);
    QAction *options_action = new QAction(tr("options"), this);

    popup_menu->addSeparator();
    popup_menu->addAction(clear_action);
    popup_menu->addAction(save_to_action);
    popup_menu->addAction(save_full_log_to_action);
    popup_menu->addAction(options_action);

    popup_menu->addSeparator();

#ifndef NO_LOG_INFO
    QAction *show_info  = new QAction(popup_menu);
    show_info->setProperty(P_APP_ENG_TEXT, "is_shows_info");
    show_info->setText(QObject::tr("is_shows_info"));
    show_info->setToolTip(QObject::tr("is_shows_info"));
    show_info->setStatusTip(QObject::tr("is_shows_info"));
    show_info->setCheckable(true);
    show_info->setChecked(flag_is_shows_info);
    connect(show_info,    &QAction::triggered,    this,   &LogBox::slot_is_shows_info);
    app_actions.append(show_info);
    popup_menu->addAction(show_info);
#endif

#ifndef NO_LOG_DEBUG
    QAction *show_debug = new QAction(popup_menu);
    show_debug->setProperty(P_APP_ENG_TEXT, "is_shows_debug");
    show_debug->setText(QObject::tr("is_shows_debug"));
    show_debug->setToolTip(QObject::tr("is_shows_debug"));
    show_debug->setStatusTip(QObject::tr("is_shows_debug"));
    show_debug->setCheckable(true);
    show_debug->setChecked(flag_is_shows_debug);
    connect(show_debug,    &QAction::triggered,    this,   &LogBox::slot_is_shows_debug);
    app_actions.append(show_debug);
    popup_menu->addAction(show_debug);
#endif

#ifndef NO_LOG_ERROR
    QAction *show_error = new QAction(popup_menu);
    show_error->setProperty(P_APP_ENG_TEXT, "is_shows_error");
    show_error->setText(QObject::tr("is_shows_error"));
    show_error->setToolTip(QObject::tr("is_shows_error"));
    show_error->setStatusTip(QObject::tr("is_shows_error"));
    show_error->setCheckable(true);
    show_error->setChecked(flag_is_shows_error);
    connect(show_error,    &QAction::triggered,    this,   &LogBox::slot_is_shows_error);
    app_actions.append(show_error);
    popup_menu->addAction(show_error);
#endif

#ifndef NO_LOG_TRACE
    QAction *show_trace = new QAction(popup_menu);
    show_trace->setProperty(P_APP_ENG_TEXT, "is_shows_trace");
    show_trace->setText(QObject::tr("is_shows_trace"));
    show_trace->setToolTip(QObject::tr("is_shows_trace"));
    show_trace->setStatusTip(QObject::tr("is_shows_trace"));
    show_trace->setCheckable(true);
    show_trace->setChecked(flag_is_shows_trace);
    connect(show_trace,    &QAction::triggered,    this,   &LogBox::slot_is_shows_trace);
    app_actions.append(show_trace);
    popup_menu->addAction(show_trace);
#endif

    connect(clear_action,               &QAction::triggered,    this, &LogBox::clear);
    connect(save_to_action,             &QAction::triggered,    this, &LogBox::save_to);
    connect(save_full_log_to_action,    &QAction::triggered,    this, &LogBox::save_full_log_to);
    connect(options_action,             &QAction::triggered,    this, &LogBox::changeOptions);

    popup_menu->exec(QCursor::pos());
}
//--------------------------------------------------------------------------------
void LogBox::slot_is_shows_info(bool state)
{
    flag_is_shows_info = state;
    update_log();
}
//--------------------------------------------------------------------------------
void LogBox::slot_is_shows_debug(bool state)
{
    flag_is_shows_debug = state;
    update_log();
}
//--------------------------------------------------------------------------------
void LogBox::slot_is_shows_error(bool state)
{
    flag_is_shows_error = state;
    update_log();
}
//--------------------------------------------------------------------------------
void LogBox::slot_is_shows_trace(bool state)
{
    flag_is_shows_trace = state;
    update_log();
}
//--------------------------------------------------------------------------------
void LogBox::set_font(QFont font)
{
    logBox->setFont(font);
}
//--------------------------------------------------------------------------------
QFont LogBox::get_font(void)
{
    return logBox->font();
}
//--------------------------------------------------------------------------------
void LogBox::create_widgets(void)
{
    QFont font("Liberation Mono", 10);
    logBox = new QTextEdit(this);
    logBox->setObjectName("te_LogBox");
    logBox->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    logBox->setFont(font);
    logBox->setProperty(NO_SAVE, true);
#ifdef Q_OS_LINUX
    //TODO белый цвет, если тема темная
    //logBox->setStyleSheet("background:white;");
#endif

    progressBar = new QProgressBar;
    progressBar->setVisible(false);

    hbox = new QHBoxLayout();
    vbox = new QVBoxLayout();
    mainbox = new QHBoxLayout();

#ifndef LOG_READ_ONLY
    hbox->setMargin(0);
#endif
    vbox->setMargin(0);
    mainbox->setMargin(0);

#ifndef LOG_READ_ONLY
    hbox->setSpacing(0);
#endif
    vbox->setSpacing(0);
    mainbox->setSpacing(0);

    progressBar->setRange(0, 100);

    logBox->setTextInteractionFlags(Qt::NoTextInteraction);

#ifndef LOG_READ_ONLY
    hbox = new QHBoxLayout();
    hbox->addStretch(1);
    hbox->addWidget(progressBar);
#endif

    vbox->addWidget(logBox);
#ifndef LOG_READ_ONLY
    vbox->addLayout(hbox);
#endif

#if 1
    fb = new FindBox(this);
    connect(fb, &FindBox::find_prev,    this,   &LogBox::find_prev);
    connect(fb, &FindBox::find_next,    this,   &LogBox::find_next);

    fb->hide();
    vbox->addWidget(fb);

    installEventFilter(this);
#endif

    mainbox->addLayout(vbox);

    setLayout(mainbox);
    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);

    //adjustSize();
}
//--------------------------------------------------------------------------------
void LogBox::find_prev(const QString &text)
{
    logBox->find(text, QTextDocument::FindBackward);
}
//--------------------------------------------------------------------------------
void LogBox::find_next(const QString &text)
{
    logBox->find(text);
}
//--------------------------------------------------------------------------------
void LogBox::setColorLog(bool state)
{
    flagColor = state;
}
//--------------------------------------------------------------------------------
void LogBox::set_flag_is_shows_info(bool state)
{
    flag_is_shows_info = state;
    update_log();
}
//--------------------------------------------------------------------------------
void LogBox::set_flag_is_shows_debug(bool state)
{
    flag_is_shows_debug = state;
    update_log();
}
//--------------------------------------------------------------------------------
void LogBox::set_flag_is_shows_error(bool state)
{
    flag_is_shows_error = state;
    update_log();
}
//--------------------------------------------------------------------------------
void LogBox::set_flag_is_shows_trace(bool state)
{
    flag_is_shows_trace = state;
    update_log();
}
//--------------------------------------------------------------------------------
void LogBox::append_string(LOG_DATA log_data)
{
    if(log_data.message.isEmpty())
    {
        return;
    }

    if(!flag_is_shows_info  && log_data.level == L_INFO)  return;
    if(!flag_is_shows_debug && log_data.level == L_DEBUG) return;
    if(!flag_is_shows_error && log_data.level == L_ERROR) return;
    if(!flag_is_shows_trace && log_data.level == L_TRACE) return;

    QString temp;

    QString level_str;
    switch (log_data.level) {
        case L_INFO:  level_str = "INFO";   break;
        case L_DEBUG: level_str = "DEBUG";  break;
        case L_ERROR: level_str = "ERROR";  break;
        case L_TRACE: level_str = "TRACE";  break;
    }
    if(flagAddDateTime)
    {
        temp = QString("%1\t%2\t%3")
                .arg(level_str)
                .arg(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"))
                .arg(log_data.message);
    }
    else
    {
        temp = log_data.message;
    }

    flagColor ? logBox->setTextColor(log_data.color_text) : logBox->setTextColor(QColor(Qt::black));
    flagColor ? logBox->setTextBackgroundColor(log_data.background_color) : logBox->setTextBackgroundColor(QColor(Qt::white));

#ifdef NEED_CODEC
    //TODO проверить надо
    QByteArray ba;
    ba.append(temp.toStdString().c_str());
#endif

    if(flagAutoSave)
    {
        if(!autosave_filename.isEmpty())
        {
            QFile file(autosave_filename);
            bool ok = file.open(QIODevice::WriteOnly | QIODevice::Append);
            if(ok)
            {
#ifdef NEED_CODEC
                file.write(ba);
#else
                QByteArray ba;
                ba.append(temp.toStdString().c_str());
                file.write(ba);
#endif
                file.write("\n");
                file.close();
            }
            else
            {
                errorLog(QString("file %1 not create").arg(autosave_filename));
            }
        }
    }

    if(flagNoCRLF)
    {
        logBox->moveCursor(QTextCursor::End);
#ifdef NEED_CODEC
        //TODO проверить надо
        logBox->insertPlainText(current_codec->toUnicode(ba));
#else
        logBox->insertPlainText(temp);
#endif
    }
    else
    {
#ifdef NEED_CODEC
        //TODO проверить надо
        logBox->append(current_codec->toUnicode(ba));
#else
        logBox->append(temp);
#endif
    }
    logBox->moveCursor(QTextCursor::End);
}
//--------------------------------------------------------------------------------
void LogBox::infoLog(const QString &text)
{
    LOG_DATA log_data;
    log_data.date = QDate::currentDate();
    log_data.time = QTime::currentTime();
    log_data.level = L_INFO;
    log_data.color_text = Qt::blue;
    log_data.background_color = Qt::white;
    log_data.message = text;

    l_full_log_data.append(log_data);
    if(!text.isEmpty() && flag_is_shows_info)
    {
        l_log_data.append(log_data);
        append_string(log_data);
    }
}
//--------------------------------------------------------------------------------
void LogBox::debugLog(const QString &text)
{
    LOG_DATA log_data;
    log_data.date = QDate::currentDate();
    log_data.time = QTime::currentTime();
    log_data.level = L_DEBUG;
    log_data.color_text = Qt::darkGreen;
    log_data.background_color = Qt::white;
    log_data.message = text;

    l_full_log_data.append(log_data);
    if(!text.isEmpty() && flag_is_shows_debug)
    {
        l_log_data.append(log_data);
        append_string(log_data);
    }
}
//--------------------------------------------------------------------------------
void LogBox::errorLog(const QString &text)
{
    LOG_DATA log_data;
    log_data.date = QDate::currentDate();
    log_data.time = QTime::currentTime();
    log_data.level = L_ERROR;
    log_data.color_text = Qt::red;
    log_data.background_color = Qt::white;
    log_data.message = text;

    l_full_log_data.append(log_data);
    if(!text.isEmpty() && flag_is_shows_error)
    {
        l_log_data.append(log_data);
        append_string(log_data);
    }
}
//--------------------------------------------------------------------------------
void LogBox::traceLog(const QString &text)
{
    LOG_DATA log_data;
    log_data.date = QDate::currentDate();
    log_data.time = QTime::currentTime();
    log_data.level = L_TRACE;
    log_data.color_text = Qt::gray;
    log_data.background_color = Qt::white;
    log_data.message = text;

    l_full_log_data.append(log_data);
    if(!text.isEmpty() && flag_is_shows_trace)
    {
        l_log_data.append(log_data);
        append_string(log_data);
    }
}
//--------------------------------------------------------------------------------
void LogBox::colorLog(const QString &text,
                      const QColor text_color,
                      const QColor background_color)
{
    LOG_DATA log_data;
    log_data.date = QDate::currentDate();
    log_data.time = QTime::currentTime();
    log_data.level = L_INFO;
    log_data.color_text = text_color;
    log_data.background_color = background_color;
    log_data.message = text;

    l_full_log_data.append(log_data);
    if(!text.isEmpty())
    {
        l_log_data.append(log_data);
        append_string(log_data);
    }
}
//--------------------------------------------------------------------------------
QString LogBox::syslog_to_str(int level)
{
    QString temp;
    switch(level)
    {
    case LOG_EMERG:     temp = "LOG_EMERG  ";  break;
    case LOG_ALERT:     temp = "LOG_ALERT  ";  break;
    case LOG_CRIT:      temp = "LOG_CRIT   ";  break;
    case LOG_ERR:       temp = "LOG_ERR    ";  break;
    case LOG_WARNING:   temp = "LOG_WARNING";  break;
    case LOG_NOTICE:    temp = "LOG_NOTICE ";  break;
    case LOG_INFO:      temp = "LOG_INFO   ";  break;
    case LOG_DEBUG:     temp = "LOG_DEBUG  ";  break;
    default:
        temp = QString("level %1").arg(level);
        break;
    }
    return temp;
}
//--------------------------------------------------------------------------------
void LogBox::syslogLog(QDateTime dt,
                       int level,
                       int src,
                       QString message)
{
    QString temp = QString("%1   %2   %3   %4")
            .arg(dt.toString("dd-MM-yy hh:mm:ss"))
            .arg(syslog_to_str(level))
            .arg(src)
            .arg(message);

    flagColor ? logBox->setTextColor(QColor("blue")) : logBox->setTextColor(QColor("black"));

    if(flagNoCRLF)
        logBox->insertPlainText(temp);
    else
        logBox->append(temp);

    logBox->moveCursor(QTextCursor::End);
    //---
    
}
//--------------------------------------------------------------------------------
void LogBox::syslogLog(int level,
                       QString src,
                       QString message)
{
    QString temp = QString("%1   %2   %3   %4")
            .arg(QDateTime::currentDateTime().toString("dd-MM-yy hh:mm:ss"))
            .arg(syslog_to_str(level))
            .arg(src)
            .arg(message);

    flagColor ? logBox->setTextColor(QColor("blue")) : logBox->setTextColor(QColor("black"));

    if(flagNoCRLF)
        logBox->insertPlainText(temp);
    else
        logBox->append(temp);

    logBox->moveCursor(QTextCursor::End);
    //---
    
}
//--------------------------------------------------------------------------------
void LogBox::save_to(void)
{
    MyFileDialog *dlg = new MyFileDialog("log_box", "log_box");
    dlg->setAcceptMode(MyFileDialog::AcceptSave);
    dlg->setNameFilter("log files (*.log)");
    dlg->setDefaultSuffix("log");
    dlg->setOption(MyFileDialog::DontUseNativeDialog, true);
    dlg->setDirectory(".");
    dlg->selectFile("без имени");
    dlg->setOption(MyFileDialog::DontConfirmOverwrite, false);
    // dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        save_log(files.at(0));
    }
    dlg->deleteLater();
}
//--------------------------------------------------------------------------------
void LogBox::save_full_log_to(void)
{
    MyFileDialog *dlg = new MyFileDialog("log_box", "log_box");
    dlg->setAcceptMode(MyFileDialog::AcceptSave);
    dlg->setNameFilter("log files (*.log)");
    dlg->setDefaultSuffix("log");
    dlg->setOption(MyFileDialog::DontUseNativeDialog, true);
    dlg->setDirectory(".");
    dlg->selectFile("без имени");
    dlg->setOption(MyFileDialog::DontConfirmOverwrite, false);
    // dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        save_full_log(files.at(0));
    }
    dlg->deleteLater();
}
//--------------------------------------------------------------------------------
void LogBox::save_log(const QString &filename)
{
#ifdef QT_DEBUG
    qDebug() << filename;
#endif
    if(filename.isEmpty()) return;

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
#ifdef QT_DEBUG
        qDebug() << filename << tr("not open");
#endif
        return;
    }

    QString temp;
    foreach (LOG_DATA ld, l_log_data)
    {
        temp.clear();

        temp.append(QString("%1|").arg(ld.date.toString("dd-MM-yyyy")));
        temp.append(QString("%1|").arg(ld.time.toString("hh:mm:ss")));
        temp.append(QString("%1|").arg(ld.level));
        temp.append(QString("#%1%2%3|")
                    .arg(static_cast<uchar>(ld.color_text.red()),   2, 16, QChar('0'))
                    .arg(static_cast<uchar>(ld.color_text.green()), 2, 16, QChar('0'))
                    .arg(static_cast<uchar>(ld.color_text.blue()),  2, 16, QChar('0')));
        temp.append(QString("#%1%2%3|")
                    .arg(static_cast<uchar>(ld.background_color.red()),   2, 16, QChar('0'))
                    .arg(static_cast<uchar>(ld.background_color.green()), 2, 16, QChar('0'))
                    .arg(static_cast<uchar>(ld.background_color.blue()),  2, 16, QChar('0')));
        temp.append(QString("%1\n").arg(ld.message));

        file.write(temp.toLocal8Bit());
    }

    file.close();
}
//--------------------------------------------------------------------------------
void LogBox::save_full_log(const QString &filename)
{
#ifdef QT_DEBUG
    qDebug() << filename;
#endif
    if(filename.isEmpty()) return;

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
#ifdef QT_DEBUG
        qDebug() << filename << tr("not open");
#endif
        return;
    }

    QString temp;
    foreach (LOG_DATA ld, l_full_log_data)
    {
        temp.clear();

        temp.append(QString("%1|").arg(ld.date.toString("dd-MM-yyyy")));
        temp.append(QString("%1|").arg(ld.time.toString("hh:mm:ss")));
        temp.append(QString("%1|").arg(ld.level));
        temp.append(QString("#%1%2%3|")
                    .arg(static_cast<uchar>(ld.color_text.red()),   2, 16, QChar('0'))
                    .arg(static_cast<uchar>(ld.color_text.green()), 2, 16, QChar('0'))
                    .arg(static_cast<uchar>(ld.color_text.blue()),  2, 16, QChar('0')));
        temp.append(QString("#%1%2%3|")
                    .arg(static_cast<uchar>(ld.background_color.red()),   2, 16, QChar('0'))
                    .arg(static_cast<uchar>(ld.background_color.green()), 2, 16, QChar('0'))
                    .arg(static_cast<uchar>(ld.background_color.blue()),  2, 16, QChar('0')));
        temp.append(QString("%1\n").arg(ld.message));

        file.write(temp.toLocal8Bit());
    }

    file.close();
}
//--------------------------------------------------------------------------------
void LogBox::changeOptions(void)
{
    Log_options *optionsBox = new Log_options();
    Q_ASSERT(optionsBox);

    optionsBox->setObjectName("optionsBox");

    optionsBox->setProperty(P_FLAG_READ_ONLY,         logBox->isReadOnly());
    optionsBox->setProperty(P_FLAG_ACCEPT_RICH_TEXT,  logBox->acceptRichText());
    optionsBox->setProperty(P_FLAG_NO_CRLF,           flagNoCRLF);
    optionsBox->setProperty(P_FLAG_ADD_DATETIME,      flagAddDateTime);
    optionsBox->setProperty(P_FLAG_COLOR,             flagColor);
    optionsBox->setProperty(P_FLAG_ERROR_AS_MESSAGE,  flagErrorAsMessage);
    optionsBox->setProperty(P_FLAG_TEXT_IS_WINDOWS,   flagTextIsWindows);
    optionsBox->setProperty(P_FLAG_AUTOSIZE,          flagAutoSave);
    optionsBox->setProperty(P_FILE_AUTOSIZE,          autosave_filename);

    qDebug() << autosave_filename;

    int res = optionsBox->exec();
    if(res == QDialog::Accepted)
    {
#ifdef NEED_CODEC
        //TODO проверить надо
        current_codec = optionsBox->get_text_codec();
        QTextCodec::setCodecForLocale(current_codec);
        emit debugLog(QString("new codec is %1").arg(current_codec->name().data()));
#endif

        logBox->setReadOnly(optionsBox->property(P_FLAG_READ_ONLY).toBool());
        logBox->setAcceptRichText(optionsBox->property(P_FLAG_ACCEPT_RICH_TEXT).toBool());
        flagNoCRLF          = optionsBox->property(P_FLAG_NO_CRLF).toBool();
        flagAddDateTime     = optionsBox->property(P_FLAG_ADD_DATETIME).toBool();
        flagColor           = optionsBox->property(P_FLAG_COLOR).toBool();
        flagErrorAsMessage  = optionsBox->property(P_FLAG_ERROR_AS_MESSAGE).toBool();
        flagTextIsWindows   = optionsBox->property(P_FLAG_TEXT_IS_WINDOWS).toBool();
        flagAutoSave        = optionsBox->property(P_FLAG_AUTOSIZE).toBool();
        autosave_filename   = optionsBox->property(P_FILE_AUTOSIZE).toString();

        qDebug() << autosave_filename;

        update_log();

        save_settings();
    }
    optionsBox->deleteLater();
}
//--------------------------------------------------------------------------------
void LogBox::update_log(void)
{
    logBox->clear();

    foreach (LOG_DATA ld, l_log_data)
    {
        append_string(ld);
    }
}
//--------------------------------------------------------------------------------
void LogBox::clearProgress()
{
    progress(0);
}
//--------------------------------------------------------------------------------
void LogBox::clear()
{
    logBox->clear();
    l_log_data.clear();
}
//--------------------------------------------------------------------------------
void LogBox::progress(int value)
{
    progressBar->setValue(value);
}
//--------------------------------------------------------------------------------
void LogBox::setVisibleProgressBar(bool state)
{
    progressBar->setVisible(state);
}
//--------------------------------------------------------------------------------
void LogBox::updateText(void)
{
    foreach (auto action, app_actions)
    {
        action->setText(tr(action->property(P_APP_ENG_TEXT).toString().toLatin1()));
        action->setToolTip(tr(action->property(P_APP_ENG_TEXT).toString().toLatin1()));
        action->setStatusTip(tr(action->property(P_APP_ENG_TEXT).toString().toLatin1()));
    }
}
//--------------------------------------------------------------------------------
void LogBox::changeEvent(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::LanguageChange:
        updateText();
        break;

    default:
        QWidget::changeEvent(event);
        break;
    }
}
//--------------------------------------------------------------------------------
void LogBox::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);

    if (event->key() == Qt::Key_F && event->modifiers() == Qt::ControlModifier)
    {
        fb->show();
    }
}
//--------------------------------------------------------------------------------
void LogBox::load_settings(void)
{
#ifdef QT_DEBUG
    qDebug() << "LogBox::load_settings(void)";
#endif

    QString text = o_name;
    if(text.isEmpty())  text = "noname";

    beginGroup(text);
    logBox->setReadOnly(load_value(P_FLAG_READ_ONLY, true).toBool());
    logBox->setAcceptRichText(load_value(P_FLAG_ACCEPT_RICH_TEXT, true).toBool());
    flagNoCRLF          = load_value(P_FLAG_NO_CRLF, false).toBool();
    flagAddDateTime     = load_value(P_FLAG_ADD_DATETIME, false).toBool();
    flagColor           = load_value(P_FLAG_COLOR, true).toBool();
    flagErrorAsMessage  = load_value(P_FLAG_ERROR_AS_MESSAGE, false).toBool();
    flagTextIsWindows   = load_value(P_FLAG_TEXT_IS_WINDOWS, false).toBool();
    flagAutoSave        = load_value(P_FLAG_AUTOSIZE, false).toBool();
    autosave_filename   = load_value(P_FILE_AUTOSIZE, "noname.log").toString();

#ifdef QT_DEBUG
    qDebug() << "logbox: load settings";
    qDebug() << "flagNoCRLF" << flagNoCRLF;
    qDebug() << "flagAddDateTime" << flagAddDateTime;
    qDebug() << "flagColor" << flagColor;
    qDebug() << "flagErrorAsMessage" << flagErrorAsMessage;
    qDebug() << "flagTextIsWindows" << flagTextIsWindows;
    qDebug() << "AutoSave" << flagAutoSave;
    qDebug() << "FileAutoSave" << autosave_filename;
#endif

    QFont font;
    QString font_name;
    int font_weight = 0;
    int font_size = 0;

    font_weight = load_value(P_FONT_WEIGHT,   QFont::Normal).toInt();
    font_size   = load_value(P_FONT_SIZE,     9).toInt();
    font_name   = load_value(P_FONT_NAME,     "Liberation Mono").toString();

    if(font_size > 72) font_size = 72;
    if(font_size < 6)  font_size = 6;

    font.setFamily(font_name);
    font.setWeight(font_weight);
    font.setPointSize(font_size);

    logBox->setFont(font);

    endGroup();
}
//--------------------------------------------------------------------------------
void LogBox::save_settings(void)
{
#ifdef QT_DEBUG
    qDebug() << "LogBox::save_settings(void)";
#endif

    QString text = o_name;
    //if(text.isEmpty())  text = objectName();
    if(text.isEmpty())  text = "RS-232";

#ifdef QT_DEBUG
    qDebug() << "logbox: save settings";
    qDebug() << "flagNoCRLF" << flagNoCRLF;
    qDebug() << "flagAddDateTime" << flagAddDateTime;
    qDebug() << "flagColor" << flagColor;
    qDebug() << "flagErrorAsMessage" << flagErrorAsMessage;
    qDebug() << "flagTextIsWindows" << flagTextIsWindows;
    qDebug() << "AutoSave" << flagAutoSave;
    qDebug() << "FileAutoSave" << autosave_filename;
#endif

    beginGroup(text);
    save_value(P_FLAG_READ_ONLY,          logBox->isReadOnly());
    save_value(P_FLAG_ACCEPT_RICH_TEXT,   logBox->acceptRichText());
    save_value(P_FLAG_NO_CRLF,            flagNoCRLF);
    save_value(P_FLAG_ADD_DATETIME,       flagAddDateTime);
    save_value(P_FLAG_COLOR,              flagColor);
    save_value(P_FLAG_ERROR_AS_MESSAGE,   flagErrorAsMessage);
    save_value(P_FLAG_TEXT_IS_WINDOWS,    flagTextIsWindows);
    save_value(P_FLAG_AUTOSIZE,           flagAutoSave);
    save_value(P_FILE_AUTOSIZE,           autosave_filename);

#ifndef NO_LOG
    QFont font = get_font();
    save_value(P_FONT_WEIGHT,  font.weight());
    save_value(P_FONT_SIZE,    font.pointSize());
    save_value(P_FONT_NAME,    font.family());
#endif

    endGroup();
}
//--------------------------------------------------------------------------------
void LogBox::set_o_name(const QString &value)
{
    o_name = value;
}
//--------------------------------------------------------------------------------
void LogBox::set_flagNoCRLF(bool state)
{
    flagNoCRLF = state;
}
//--------------------------------------------------------------------------------
void LogBox::set_flagAddDateTime(bool state)
{
    flagAddDateTime = state;
}
//--------------------------------------------------------------------------------
void LogBox::set_flagColor(bool state)
{
    flagColor = state;
}
//--------------------------------------------------------------------------------
void LogBox::set_flagErrorAsMessage(bool state)
{
    flagErrorAsMessage = state;
}
//--------------------------------------------------------------------------------
void LogBox::set_flagTextIsWindows(bool state)
{
    flagTextIsWindows = state;
}
//--------------------------------------------------------------------------------
void LogBox::set_flagAutoSave(bool state)
{
    flagAutoSave = state;
}
//--------------------------------------------------------------------------------
