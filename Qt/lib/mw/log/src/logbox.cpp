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
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
//--------------------------------------------------------------------------------
#include "log_options.hpp"
#include "findbox.hpp"
#include "defines.hpp"
#include "logbox.hpp"
//--------------------------------------------------------------------------------
//#include "LoggingCategories.hpp"
//--------------------------------------------------------------------------------
LogBox::LogBox(QWidget *parent) :
    QFrame(parent)
{
    init();
}
//--------------------------------------------------------------------------------
LogBox::LogBox(const QString &o_name,
               QWidget *parent,
               int min_width,
               int min_height) :
    QFrame(parent),
    o_name(o_name),
    flagNoCRLF(false),
    flagAddDateTime(false),
    flagColor(true),
    flagErrorAsMessage(false),
    flagTextIsWindows(false),
    flagAutoSave(false)
{
    init();

    if(min_width > 0)
        setMinimumWidth(min_width);
    if(min_height > 0)
        setMinimumHeight(min_height);
}
//--------------------------------------------------------------------------------
LogBox::~LogBox()
{
    Q_CHECK_PTR(logBox);
    Q_CHECK_PTR(progressBar);
    Q_CHECK_PTR(fb);

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

    create_widgets();

    if(o_name.isEmpty() == false)
    {
        load_settings();
    }

    logBox->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(logBox, SIGNAL(customContextMenuRequested(QPoint)),   this, SLOT(popup(QPoint)));

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
    QAction *options_action = new QAction(tr("options"), this);

    popup_menu->addSeparator();
    popup_menu->addAction(clear_action);
    popup_menu->addAction(save_to_action);
    popup_menu->addAction(options_action);

    connect(clear_action,   SIGNAL(triggered(bool)), this, SLOT(clear()));
    connect(save_to_action, SIGNAL(triggered(bool)), this, SLOT(save_to()));
    connect(options_action, SIGNAL(triggered(bool)), this, SLOT(changeOptions()));

    popup_menu->exec(QCursor::pos());
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
    if(!text.isEmpty() && flag_is_shows_info)
    {
        //---
        LOG_DATA log_data;
        log_data.date = QDate::currentDate();
        log_data.time = QTime::currentTime();
        log_data.level = L_INFO;
        log_data.color_text = Qt::blue;
        log_data.background_color = Qt::white;
        log_data.message = text;

        l_log_data.append(log_data);
        //---

        append_string(log_data);
    }
}
//--------------------------------------------------------------------------------
void LogBox::debugLog(const QString &text)
{
    if(!text.isEmpty() && flag_is_shows_debug)
    {
        //---
        LOG_DATA log_data;
        log_data.date = QDate::currentDate();
        log_data.time = QTime::currentTime();
        log_data.level = L_DEBUG;
        log_data.color_text = Qt::darkGreen;
        log_data.background_color = Qt::white;
        log_data.message = text;

        l_log_data.append(log_data);
        //---

        append_string(log_data);
    }
}
//--------------------------------------------------------------------------------
void LogBox::errorLog(const QString &text)
{
    if(!text.isEmpty() && flag_is_shows_error)
    {
        //---
        LOG_DATA log_data;
        log_data.date = QDate::currentDate();
        log_data.time = QTime::currentTime();
        log_data.level = L_ERROR;
        log_data.color_text = Qt::red;
        log_data.background_color = Qt::white;
        log_data.message = text;

        l_log_data.append(log_data);
        //---

        append_string(log_data);
    }
}
//--------------------------------------------------------------------------------
void LogBox::traceLog(const QString &text)
{
    if(!text.isEmpty() && flag_is_shows_trace)
    {
        //---
        LOG_DATA log_data;
        log_data.date = QDate::currentDate();
        log_data.time = QTime::currentTime();
        log_data.level = L_TRACE;
        log_data.color_text = Qt::gray;
        log_data.background_color = Qt::white;
        log_data.message = text;

        l_log_data.append(log_data);
        //---

        append_string(log_data);
    }
}
//--------------------------------------------------------------------------------
void LogBox::colorLog(const QString &text,
                      const QColor text_color,
                      const QColor background_color)
{
    if(!text.isEmpty())
    {
        //---
        LOG_DATA log_data;
        log_data.date = QDate::currentDate();
        log_data.time = QTime::currentTime();
        log_data.level = L_INFO;
        log_data.color_text = text_color;
        log_data.background_color = background_color;
        log_data.message = text;

        l_log_data.append(log_data);
        //---

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
    qApp->processEvents();
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
    qApp->processEvents();
}
//--------------------------------------------------------------------------------
void LogBox::save_to(void)
{
    QFileDialog *dlg;

    dlg = new QFileDialog;
    dlg->setAcceptMode(QFileDialog::AcceptSave);
    dlg->setNameFilter(tr("log files (*.log)"));
    dlg->setDefaultSuffix(tr("log"));
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    dlg->setDirectory(".");
    dlg->selectFile("без имени");
    dlg->setOption(QFileDialog::DontConfirmOverwrite, false);
    // dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        save_log(files.at(0));
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

#if 0
    if(flagTextIsWindows)
    {
        file.write(logBox->toPlainText().replace('\n', "\r\n").toLocal8Bit()); //.toAscii());
    }
    else
    {
        file.write(logBox->toPlainText().toLocal8Bit()); //.toAscii());
    }
#else
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
        temp.append(QString("%1|").arg(ld.background_color.red()));
        temp.append(QString("%1\n").arg(ld.message));

        file.write(temp.toLocal8Bit());
    }
#endif

    file.close();
}
//--------------------------------------------------------------------------------
void LogBox::changeOptions(void)
{
    Log_options *optionsBox = new Log_options();
    Q_CHECK_PTR(optionsBox);

    optionsBox->setObjectName("optionsBox");

    optionsBox->setProperty("flag_ReadOnly",        logBox->isReadOnly());
    optionsBox->setProperty("flag_AcceptRichText",  logBox->acceptRichText());
    optionsBox->setProperty("flag_NoCRLF",          flagNoCRLF);
    optionsBox->setProperty("flag_AddDateTime",     flagAddDateTime);
    optionsBox->setProperty("flag_Color",           flagColor);
    optionsBox->setProperty("flag_ErrorAsMessage",  flagErrorAsMessage);
    optionsBox->setProperty("flag_TextIsWindows",   flagTextIsWindows);
    optionsBox->setProperty("flag_AutoSave",        flagAutoSave);
    optionsBox->setProperty("file_AutoSave",        autosave_filename);

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

        logBox->setReadOnly(optionsBox->property("flag_ReadOnly").toBool());
        logBox->setAcceptRichText(optionsBox->property("flag_AcceptRichText").toBool());
        flagNoCRLF          = optionsBox->property("flag_NoCRLF").toBool();
        flagAddDateTime     = optionsBox->property("flag_AddDateTime").toBool();
        flagColor           = optionsBox->property("flag_Color").toBool();
        flagErrorAsMessage  = optionsBox->property("flag_ErrorAsMessage").toBool();
        flagTextIsWindows   = optionsBox->property("flag_TextIsWindows").toBool();
        flagAutoSave        = optionsBox->property("flag_AutoSave").toBool();
        autosave_filename   = optionsBox->property("file_AutoSave").toString();

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

}
//--------------------------------------------------------------------------------
void LogBox::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        updateText();
        break;

    default:
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

#ifndef SAVE_INI
    QSettings *settings = new QSettings(ORGNAME, APPNAME);
#else
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
#endif
    Q_CHECK_PTR(settings);

    settings->beginGroup(text);
    logBox->setReadOnly(settings->value("readOnly", true).toBool());
    logBox->setAcceptRichText(settings->value("acceptRichText", true).toBool());
    flagNoCRLF          = settings->value("no_CRLF", false).toBool();
    flagAddDateTime     = settings->value("addDateTime", false).toBool();
    flagColor           = settings->value("color", true).toBool();
    flagErrorAsMessage  = settings->value("ErrorAsMessage", false).toBool();
    flagTextIsWindows   = settings->value("TextIsWindows", false).toBool();
    flagAutoSave        = settings->value("AutoSave", false).toBool();
    autosave_filename   = settings->value("FileAutoSave", "noname.log").toString();

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

    font_weight = settings->value("FontWeight",   QFont::Normal).toInt();
    font_size   = settings->value("FontSize",     9).toInt();
    font_name   = settings->value("FontName",     "Liberation Mono").toString();

    if(font_size > 72) font_size = 72;
    if(font_size < 6)  font_size = 6;

    font.setFamily(font_name);
    font.setWeight(font_weight);
    font.setPointSize(font_size);

    logBox->setFont(font);

    settings->endGroup();
    settings->deleteLater();
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

#ifndef SAVE_INI
    QSettings *settings = new QSettings(ORGNAME, APPNAME);
#else
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
#endif
    Q_CHECK_PTR(settings);

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

    settings->beginGroup(text);
    settings->setValue("readOnly",      logBox->isReadOnly());
    settings->setValue("acceptRichText",logBox->acceptRichText());
    settings->setValue("no_CRLF",       flagNoCRLF);
    settings->setValue("addDateTime",   flagAddDateTime);
    settings->setValue("color",         flagColor);
    settings->setValue("ErrorAsMessage",flagErrorAsMessage);
    settings->setValue("TextIsWindows", flagTextIsWindows);
    settings->setValue("AutoSave",      flagAutoSave);
    settings->setValue("FileAutoSave",  autosave_filename);

#ifndef NO_LOG
    QFont font = get_font();
    settings->setValue("FontWeight",  font.weight());
    settings->setValue("FontSize",    font.pointSize());
    settings->setValue("FontName",    font.family());
#endif

    settings->endGroup();
    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void LogBox::set_o_name(QString value)
{
    o_name = value;
}
//--------------------------------------------------------------------------------
