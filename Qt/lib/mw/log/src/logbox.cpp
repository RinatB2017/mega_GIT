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
#include "myfiledialog.hpp"
#include "log_options.hpp"
#include "findbox.hpp"
#include "defines.hpp"
#include "logbox.hpp"
//--------------------------------------------------------------------------------
//#include "LoggingCategories.hpp"
//--------------------------------------------------------------------------------
LogBox::LogBox(QWidget *parent) :
    MyWidget(parent),
    o_name("logbox")
{
    init();
}
//--------------------------------------------------------------------------------
LogBox::LogBox(const QString &o_name,
               QWidget *parent) :
    MyWidget(parent),
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

    if(logBox)
    {
        disconnect(logBox, &LogBox::customContextMenuRequested,    this,   &LogBox::popup);
        delete logBox;
    }
    if(progressBar)
    {
        delete progressBar;
    }
    if(fb)
    {
        disconnect(fb, &FindBox::find_prev,    this,   &LogBox::find_prev);
        disconnect(fb, &FindBox::find_next,    this,   &LogBox::find_next);
        delete fb;
    }
}
//--------------------------------------------------------------------------------
void LogBox::init(void)
{
    setObjectName("LogBox");

#ifdef NEED_CODEC
#ifdef Q_OS_WIN
    current_codec = QTextCodec::codecForName("windows-1251");   // костыль для винды
#else
    current_codec = QTextCodec::codecForLocale();
#endif
#ifdef QT_DEBUG
    qDebug() << QString("Current codec is %1").arg(current_codec->name().data()); //FIXME потом убрать
#endif
#endif

    create_widgets();

    logBox->setContextMenuPolicy(Qt::CustomContextMenu);
#ifndef NO_LOG_MENU
    connect(logBox, &LogBox::customContextMenuRequested,
            this,   &LogBox::popup);
#endif
}
//--------------------------------------------------------------------------------
void LogBox::popup(QPoint)
{
    QMenu *popup_menu = logBox->createStandardContextMenu();
    Q_ASSERT(popup_menu);
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
    Q_ASSERT(show_info);
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
    Q_ASSERT(show_debug);
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
    Q_ASSERT(show_error);
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
    Q_ASSERT(show_trace);
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
    Q_ASSERT(logBox);
    logBox->setObjectName("te_LogBox");
    logBox->setFont(font);
    logBox->setProperty(NO_SAVE, true);
#ifdef Q_OS_LINUX
    // белый цвет, если тема темная
    //logBox->setStyleSheet("background:white;");
#endif

    progressBar = new QProgressBar(this);
    progressBar->setVisible(false);

    hbox = new QHBoxLayout();
    vbox = new QVBoxLayout();
    mainbox = new QHBoxLayout();

#ifndef LOG_READ_ONLY
    hbox->setContentsMargins(0, 0, 0, 0);
#endif
    vbox->setContentsMargins(0, 0, 0, 0);
    mainbox->setContentsMargins(0, 0, 0, 0);

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

    fb = new FindBox(this);
    connect(fb, &FindBox::find_prev,    this,   &LogBox::find_prev);
    connect(fb, &FindBox::find_next,    this,   &LogBox::find_next);

    fb->hide();
    vbox->addWidget(fb);

    installEventFilter(this);

    mainbox->addLayout(vbox);

    setLayout(mainbox);
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
    QByteArray ba;
    ba.append(temp.toLocal8Bit());
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
                errorLog(QString("file %1 not writed!").arg(autosave_filename));
            }
        }
    }

    if(flagNoCRLF)
    {
        try {
            logBox->moveCursor(QTextCursor::End);
        } catch(...)
        {
            qDebug() << "error";
        }

#ifdef NEED_CODEC
        logBox->setTextColor(log_data.color_text);
        logBox->insertPlainText(current_codec->toUnicode(ba));
#else
        logBox->insertPlainText(temp);
#endif
    }
    else
    {
#ifdef NEED_CODEC
        // logBox->append(QString("Current codec is %1").arg(current_codec->name().data())); //FIXME потом убрать
        Q_ASSERT(current_codec);
        logBox->append(current_codec->toUnicode(ba));
#else
        logBox->append(temp);
#endif
    }
    try {
        logBox->moveCursor(QTextCursor::End);
    } catch(...)
    {
        qDebug() << "error";
    }
}
//--------------------------------------------------------------------------------
void LogBox::infoLog(const QString &text)
{
    LOG_DATA log_data;
    log_data.date = QDate::currentDate();
    log_data.time = QTime::currentTime();
    log_data.level = L_INFO;
    log_data.color_text = Qt::blue;
    QPalette palette = QApplication::palette();
    log_data.background_color = palette.color(QPalette::Base);
    log_data.message = text;

    l_full_log_data.append(log_data);
    if(!text.isEmpty() && flag_is_shows_info)
    {
        l_log_data.append(log_data);
        append_string(log_data);
    }

#ifdef FILENAME_LOG
    save_logfile(QDateTime::currentDateTime(),
                 L_INFO,
                 text);
#endif
}
//--------------------------------------------------------------------------------
void LogBox::debugLog(const QString &text)
{
    LOG_DATA log_data;
    log_data.date = QDate::currentDate();
    log_data.time = QTime::currentTime();
    log_data.level = L_DEBUG;
    log_data.color_text = Qt::darkGreen;
    QPalette palette = QApplication::palette();
    log_data.background_color = palette.color(QPalette::Base);
    log_data.message = text;

    l_full_log_data.append(log_data);
    if(!text.isEmpty() && flag_is_shows_debug)
    {
        l_log_data.append(log_data);
        append_string(log_data);
    }

#ifdef FILENAME_LOG
    save_logfile(QDateTime::currentDateTime(),
                 L_DEBUG,
                 text);
#endif
}
//--------------------------------------------------------------------------------
void LogBox::errorLog(const QString &text)
{
    LOG_DATA log_data;
    log_data.date = QDate::currentDate();
    log_data.time = QTime::currentTime();
    log_data.level = L_ERROR;
    log_data.color_text = Qt::red;
    QPalette palette = QApplication::palette();
    log_data.background_color = palette.color(QPalette::Base);
    log_data.message = text;

    l_full_log_data.append(log_data);
    if(!text.isEmpty() && flag_is_shows_error)
    {
        l_log_data.append(log_data);
        append_string(log_data);
#ifdef ERROR_AS_MESSAGEBOX
        messagebox_critical("Error", log_data.message);
#endif
    }

#ifdef FILENAME_LOG
    save_logfile(QDateTime::currentDateTime(),
                 L_ERROR,
                 text);
#endif
}
//--------------------------------------------------------------------------------
void LogBox::traceLog(const QString &text)
{
    LOG_DATA log_data;
    log_data.date = QDate::currentDate();
    log_data.time = QTime::currentTime();
    log_data.level = L_TRACE;
    log_data.color_text = Qt::gray;
    QPalette palette = QApplication::palette();
    log_data.background_color = palette.color(QPalette::Base);
    log_data.message = text;

    l_full_log_data.append(log_data);
    if(!text.isEmpty() && flag_is_shows_trace)
    {
        l_log_data.append(log_data);
        append_string(log_data);
    }

#ifdef FILENAME_LOG
    save_logfile(QDateTime::currentDateTime(),
                 L_TRACE,
                 text);
#endif
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

#ifdef FILENAME_LOG
    save_logfile(QDateTime::currentDateTime(),
                 L_INFO,
                 text);
#endif
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
                       const QString &message)
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

    try {
        logBox->moveCursor(QTextCursor::End);
    } catch(...)
    {
        qDebug() << "error";
    }
    //---
}
//--------------------------------------------------------------------------------
void LogBox::syslogLog(int level,
                       const QString &src,
                       const QString &message)
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

    try {
        logBox->moveCursor(QTextCursor::End);
    } catch(...)
    {
        qDebug() << "error";
    }
    //---
}
//--------------------------------------------------------------------------------
void LogBox::save_to(void)
{
    MyFileDialog *dlg = new MyFileDialog("log_box");
    dlg->setAcceptMode(MyFileDialog::AcceptSave);
    dlg->setNameFilter("log files (*.log)");
    dlg->setDefaultSuffix("log");
    dlg->setOption(MyFileDialog::DontUseNativeDialog, false);
    dlg->setDirectory(".");
    dlg->selectFile("без имени");
    dlg->setOption(MyFileDialog::DontConfirmOverwrite, false);
    // dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        save_log(files.at(0));
    }
    delete dlg;
}
//--------------------------------------------------------------------------------
void LogBox::save_full_log_to(void)
{
    MyFileDialog *dlg = new MyFileDialog("log_box");
    dlg->setAcceptMode(MyFileDialog::AcceptSave);
    dlg->setNameFilter("log files (*.log)");
    dlg->setDefaultSuffix("log");
    dlg->setOption(MyFileDialog::DontUseNativeDialog, false);
    dlg->setDirectory(".");
    dlg->selectFile("без имени");
    dlg->setOption(MyFileDialog::DontConfirmOverwrite, false);
    // dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        save_full_log(files.at(0));
    }
    delete dlg;
}
//--------------------------------------------------------------------------------
void LogBox::save_logfile(const QDateTime &dt,
                          int level,
                          const QString log)
{
    QString filename = QString("log/%1.log")
    .arg(APPNAME);

    QFile file(filename);
    if (!file.open(QIODevice::Append | QIODevice::Text))
    {
#ifdef QT_DEBUG
        qDebug() << filename << tr("not open");
#endif
        return;
    }

    QString temp;

    temp.append(QString("%1\t")
                    .arg(dt.toString("dd-MM-yyyy hh:mm:ss")));

    switch (level)
    {
    case L_INFO:    temp.append("INFO\t");  break;
    case L_DEBUG:   temp.append("DEBUG\t");  break;
    case L_ERROR:   temp.append("ERROR\t");  break;
    case L_TRACE:   temp.append("TRACE\t");  break;
        break;
    }
    temp.append(QString("%1\n")
                    .arg(log));

    file.write(temp.toLocal8Bit());
    file.close();
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
        emit error(QString("file %1 not writed!").arg(filename));
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
        emit error(QString("file %1 not writed!").arg(filename));
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
        current_codec = optionsBox->get_text_codec();
        QTextCodec::setCodecForLocale(current_codec);
        emit infoLog(QString("new codec is %1").arg(current_codec->name().data()));
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
    }
    delete optionsBox;
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
void LogBox::updateText(void)
{
    foreach (QAction *action, app_actions)
    {
        action->setText(tr(action->property(P_APP_ENG_TEXT).toString().toLocal8Bit()));
        action->setToolTip(tr(action->property(P_APP_ENG_TEXT).toString().toLocal8Bit()));
        action->setStatusTip(tr(action->property(P_APP_ENG_TEXT).toString().toLocal8Bit()));
    }
}
//--------------------------------------------------------------------------------
bool LogBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void LogBox::load_setting(void)
{
    beginGroup(get_full_objectName(this));

    flag_is_shows_info  = load_bool(P_FLAG_SHOW_INFO,   true);
    flag_is_shows_debug = load_bool(P_FLAG_SHOW_DEBUG,  true);
    flag_is_shows_error = load_bool(P_FLAG_SHOW_ERROR,  true);
    flag_is_shows_trace = load_bool(P_FLAG_SHOW_TRACE,  true);

#ifdef NO_LOG_DEBUG
    flag_is_shows_debug = false;
#endif
#ifdef NO_LOG_TRACE
    flag_is_shows_trace = false;
#endif
    //---

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
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    QFont::Weight font_weight;
#else
    int font_weight = 0;
#endif
    int font_size = 0;

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    font_weight = QFont::Normal;
#else
    font_weight = load_value(P_FONT_WEIGHT,   QFont::Normal).toInt();
#endif
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
void LogBox::save_setting(void)
{
#ifdef QT_DEBUG
    qDebug() << "LogBox::save_setting(void)";
#endif

    //    QString text = o_name;
    //    if(text.isEmpty())  text = objectName();
    //    if(text.isEmpty())  text = "LogBox";

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

    beginGroup(get_full_objectName(this));

    save_value(P_FLAG_READ_ONLY,          logBox->isReadOnly());
    save_value(P_FLAG_ACCEPT_RICH_TEXT,   logBox->acceptRichText());
    save_value(P_FLAG_NO_CRLF,            flagNoCRLF);
    save_value(P_FLAG_ADD_DATETIME,       flagAddDateTime);
    save_value(P_FLAG_COLOR,              flagColor);
    save_value(P_FLAG_ERROR_AS_MESSAGE,   flagErrorAsMessage);
    save_value(P_FLAG_TEXT_IS_WINDOWS,    flagTextIsWindows);
    save_value(P_FLAG_AUTOSIZE,           flagAutoSave);
    save_value(P_FILE_AUTOSIZE,           autosave_filename);

    save_bool(P_FLAG_SHOW_INFO,   flag_is_shows_info);
    save_bool(P_FLAG_SHOW_ERROR,  flag_is_shows_error);
#ifndef NO_LOG_DEBUG
    save_bool(P_FLAG_SHOW_DEBUG,  flag_is_shows_debug);
#endif
#ifndef NO_LOG_TRACE
    save_bool(P_FLAG_SHOW_TRACE,  flag_is_shows_trace);
#endif

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
