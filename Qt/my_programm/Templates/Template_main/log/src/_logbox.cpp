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
#include <QApplication>
#include <QProgressBar>
#include <QPushButton>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateTime>
#include <QCheckBox>
#include <QTextEdit>
#include <QSettings>
#include <QWidget>
#include <QLocale>
#include <QEvent>
#include <QFrame>
#include <QFile>
#include <QFont>
#include <QStyle>
#include <QMenu>
#include <QMap>
//--------------------------------------------------------------------------------
#include <QDebug>
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
//--------------------------------------------------------------------------------
#include "myfiledialog.hpp"
#include "mywidget.hpp"
#include "log_options.hpp"
#include "defines.hpp"
#include "logbox.hpp"
//--------------------------------------------------------------------------------
LogBox::LogBox(QWidget *parent,
               unsigned int min_width,
               unsigned int min_height) :
    QFrame(parent),
    flagNoCRLF(false),
    flagAddDateTime(false),
    flagColor(true),
    flagAutoLoad(false),
    flagAutoSave(false),
    flagErrorAsMessage(false),
    flagTextIsWindows(false)
{
    init();

    if(min_width != 0)
        setMinimumWidth(min_width);
    if(min_height != 0)
        setMinimumHeight(min_height);
}
//--------------------------------------------------------------------------------
LogBox::~LogBox()
{
    //save_settings();
    if(flagAutoSave)
    {
        save_html();
    }

    delete logBox;

    delete progressBar;
    delete hbox;
    delete vbox;
    delete mainbox;
}
//--------------------------------------------------------------------------------
void LogBox::init(void)
{
    create_widgets();
    load_settings();
    if(flagAutoLoad)
    {
        load_html();
    }

    logBox->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(logBox, SIGNAL(customContextMenuRequested(QPoint)),   this, SLOT(popup(QPoint)));
}
//--------------------------------------------------------------------------------
void LogBox::popup(QPoint)
{
    QMenu *popup_menu = logBox->createStandardContextMenu();
    popup_menu->setStyleSheet("background:white;color:black;");

    QAction *clear_action   = new QAction(tr("clear"),   this);
    QAction *save_to_action = new QAction(tr("save to"), this);
    //QAction *options_action = new QAction(tr("options"), this);

    popup_menu->addSeparator();
    popup_menu->addAction(clear_action);
    popup_menu->addAction(save_to_action);
    //popup_menu->addAction(options_action);

    connect(clear_action,   SIGNAL(triggered(bool)), this, SLOT(clear()));
    connect(save_to_action, SIGNAL(triggered(bool)), this, SLOT(save_to()));
    //connect(options_action, SIGNAL(triggered(bool)), this, SLOT(changeOptions()));

    popup_menu->exec(QCursor::pos());
}
//--------------------------------------------------------------------------------
#ifndef NO_LOG
void LogBox::set_font(QFont font)
{
    //qDebug() << font;
    logBox->setFont(font);
}
#endif
//--------------------------------------------------------------------------------
#ifndef NO_LOG
QFont LogBox::get_font(void)
{
    return logBox->font();
}
#endif
//--------------------------------------------------------------------------------
void LogBox::create_widgets(void)
{
    QFont font("Courier", 10);
    logBox = new QTextEdit(this);
    logBox->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    logBox->setFont(font);
#ifdef Q_OS_LINUX
    //TODO белый цвет, если тема темная
    logBox->setStyleSheet("background:white;");
#endif

    progressBar = new QProgressBar;
    progressBar->setVisible(false);

    hbox = new QHBoxLayout;
    vbox = new QVBoxLayout;
    mainbox = new QHBoxLayout(this);

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
    hbox = new QHBoxLayout;
    hbox->addStretch(1);
    hbox->addWidget(progressBar);
#endif

    vbox->addWidget(logBox);
#ifndef LOG_READ_ONLY
    vbox->addLayout(hbox);
#endif

    mainbox->addLayout(vbox);

    setLayout(mainbox);
    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);

    adjustSize();
}
//--------------------------------------------------------------------------------
void LogBox::setColorLog(bool state)
{
    flagColor = state;
}
//--------------------------------------------------------------------------------
void LogBox::infoLog(const QString &text)
{
    //TODO
    //if(busy)    return;
    //busy = true;

    QString temp;

    //TODO
#if 0
    QString o_name = sender()->metaObject()->className();
    qDebug() << o_name;
#endif
    //---

    if(text.isEmpty()) return;
    QCoreApplication::processEvents();
    if(flagAddDateTime)
    {
        temp = QString("%1 INFO  %2")
                .arg(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"))
                .arg(text);
    }
    else
    {
        temp = text;
    }

    if(flagAutoSave)
    {
        QFile logfile("log.txt");
        if (logfile.open(QIODevice::Append | QIODevice::Text))
        {
            logfile.write(QString("%1\n").arg(temp).toLocal8Bit());
            logfile.close();
        }
    }

    flagColor ? logBox->setTextColor(QColor("blue")) : logBox->setTextColor(QColor("black"));
    flagNoCRLF ? logBox->insertPlainText(temp) : logBox->append(temp);
    logBox->moveCursor(QTextCursor::End);
    //---
    QCoreApplication::processEvents();

    //TODO
    //busy = false;
}
//--------------------------------------------------------------------------------
void LogBox::debugLog(const QString &text)
{
    //TODO
    //if(busy)    return;
    //busy = true;

    QString temp;

#ifndef QT_DEBUG
    return;
#endif

    if(text.isEmpty()) return;
    QCoreApplication::processEvents();
    if(flagAddDateTime)
    {
        temp = QString("%1 DEBUG %2")
                .arg(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"))
                .arg(text);
    }
    else
    {
        temp = text;
    }

#if 0
    if(flagAutoSave)
    {
        QFile logfile("log.txt");
        if (logfile.open(QIODevice::Append | QIODevice::Text))
        {
            logfile.write(QString("%1\n").arg(temp).toLocal8Bit());
            logfile.close();
        }
    }
#endif

    flagColor ? logBox->setTextColor(QColor("green")) : logBox->setTextColor(QColor("black"));
    flagNoCRLF ? logBox->insertPlainText(temp) : logBox->append(temp);
    logBox->moveCursor(QTextCursor::End);
    //---
    QCoreApplication::processEvents();

    //TODO
    //busy = false;
}
//--------------------------------------------------------------------------------
void LogBox::errorLog(const QString &text)
{
    //TODO
    //if(busy)    return;
    //busy = true;

    QString temp;

    if(text.isEmpty()) return;
    QCoreApplication::processEvents();
    if(flagAddDateTime)
    {
        temp = QString("%1 ERROR %2")
                .arg(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"))
                .arg(text);
    }
    else
    {
        temp = text;
    }

    if(flagErrorAsMessage)
    {
        MyWidget::messagebox_critical("Ошибка", temp);
    }

    if(flagAutoSave)
    {
        QFile logfile("log.txt");
        if (logfile.open(QIODevice::Append | QIODevice::Text))
        {
            logfile.write(QString("%1\n").arg(temp).toLocal8Bit());
            logfile.close();
        }
    }

    flagColor ? logBox->setTextColor(QColor("red")) : logBox->setTextColor(QColor("black"));
    flagNoCRLF ? logBox->insertPlainText(temp) : logBox->append(temp);
    logBox->moveCursor(QTextCursor::End);
    //---
    QCoreApplication::processEvents();

    //TODO
    //busy = false;
}
//--------------------------------------------------------------------------------
void LogBox::messageLog(const QString &text)
{
    //TODO
    //if(busy)    return;
    //busy = true;

    QString temp;

    if(text.isEmpty()) return;
    QCoreApplication::processEvents();
    if(flagAddDateTime)
    {
        temp = QString("%1 MESSAGE %2")
                .arg(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"))
                .arg(text);
    }
    else
    {
        temp = text;
    }

    if(flagAutoSave)
    {
        QFile logfile("log.txt");
        if (logfile.open(QIODevice::Append | QIODevice::Text))
        {
            logfile.write(QString("%1\n").arg(temp).toLocal8Bit());
            logfile.close();
        }
    }

    flagColor ? logBox->setTextColor(QColor("gray")) : logBox->setTextColor(QColor("black"));
    flagNoCRLF ? logBox->insertPlainText(temp) : logBox->append(temp);
    logBox->moveCursor(QTextCursor::End);
    //---
    QCoreApplication::processEvents();

    //TODO
    //busy = false;
}
//--------------------------------------------------------------------------------
void LogBox::save_to(void)
{
    MyFileDialog *dlg = new MyFileDialog("filedialog_states", "lb_dlg");
    dlg->setAcceptMode(MyFileDialog::AcceptSave);
    dlg->setNameFilter("Фвйлы лога (*.log)");
    dlg->setDefaultSuffix(tr("log"));
    dlg->selectFile("без имени");
    dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        save_log(files.at(0));
    }
    delete dlg;
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
    if(flagTextIsWindows)
    {
#if 0
        QString str=logBox->toPlainText();
        QStringList strList=str.split('\n');
        foreach (QString temp, strList)
        {
            temp.append("\r\n");
            file.write(temp.toLocal8Bit());
        }
#else
        file.write(logBox->toPlainText().replace('\n', "\r\n").toLocal8Bit()); //.toAscii());
#endif
    }
    else
    {
        file.write(logBox->toPlainText().toLocal8Bit()); //.toAscii());
        //file.write(logBox->toHtml().toLocal8Bit());    //.toAscii());
    }

    file.close();
}
//--------------------------------------------------------------------------------
void LogBox::changeOptions(void)
{
    Log_options *optionsBox = new Log_options();
    optionsBox->set_flag_ReadOnly(logBox->isReadOnly());
    optionsBox->set_flag_AcceptRichText(logBox->acceptRichText());
    optionsBox->set_flag_NoCRLF(flagNoCRLF);
    optionsBox->set_flag_AddDateTime(flagAddDateTime);
    optionsBox->set_flag_Color(flagColor);
    optionsBox->set_flag_AutoLoad(flagAutoLoad);
    optionsBox->set_flag_AutoSave(flagAutoSave);
    optionsBox->set_flag_ErrorAsMessage(flagErrorAsMessage);
    optionsBox->set_flag_TextIsWindows(flagTextIsWindows);

    int res = optionsBox->exec();
    if(res == QDialog::Accepted)
    {
        logBox->setReadOnly(optionsBox->get_flag_ReadOnly());
        logBox->setAcceptRichText(optionsBox->get_flag_AcceptRichText());
        flagNoCRLF          = optionsBox->get_flag_NoCRLF();
        flagAddDateTime     = optionsBox->get_flag_AddDateTime();
        flagColor           = optionsBox->get_flag_Color();
        flagAutoLoad        = optionsBox->get_flag_AutoLoad();
        flagAutoSave        = optionsBox->get_flag_AutoSave();
        flagErrorAsMessage  = optionsBox->get_flag_ErrorAsMessage();
        flagTextIsWindows   = optionsBox->get_flag_TextIsWindows();
        save_settings();
    }
    delete optionsBox;
}
//--------------------------------------------------------------------------------
void LogBox::clearProgress()
{
    progress(0);
}
//--------------------------------------------------------------------------------
void LogBox::append(const QString &data)
{
    if(data.isEmpty()) return;
    logBox->append(data);
    logBox->moveCursor(QTextCursor::End);
}
//--------------------------------------------------------------------------------
void LogBox::bappend(const QByteArray &data)
{
    if(data.isEmpty()) return;
    logBox->insertPlainText(data);
    logBox->moveCursor(QTextCursor::End);
}
//--------------------------------------------------------------------------------
void LogBox::clear()
{
    //logs.clear();
    logBox->clear();
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
void LogBox::load_html(void)
{
    QString filename = "log.html";
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
#ifdef QT_DEBUG
        qDebug() << filename << tr("not open");
#endif
        return;
    }

    QByteArray ba;
    ba.append(file.readAll());
    logBox->clear();
    logBox->append(ba);

    file.close();
}
//--------------------------------------------------------------------------------
void LogBox::save_html(void)
{
    QString filename = "log.html";
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
#ifdef QT_DEBUG
        qDebug() << filename << tr("not open");
#endif
        return;
    }

    file.write(logBox->toHtml().toLocal8Bit());
    file.close();
}
//--------------------------------------------------------------------------------
void LogBox::load_settings(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(!mw) return;

#ifdef QT_DEBUG
    qDebug() << "LogBox::load_settings(void)";
#endif

    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);

    settings->beginGroup("LogEdit");
    logBox->setReadOnly(settings->value("readOnly", true).toBool());
    logBox->setAcceptRichText(settings->value("acceptRichText", true).toBool());
    flagNoCRLF      = settings->value("no_CRLF", false).toBool();
    flagAddDateTime = settings->value("addDateTime", false).toBool();
    flagColor       = settings->value("color", true).toBool();
    flagAutoLoad    = settings->value("auto_load", false).toBool();
    flagAutoSave    = settings->value("auto_save", false).toBool();
    flagErrorAsMessage = settings->value("ErrorAsMessage", false).toBool();
    flagTextIsWindows  = settings->value("TextIsWindows", false).toBool();

    QFont font;
    QString font_name;
    int font_weight = 0;
    int font_size = 0;

    font_weight = settings->value("FontWeight",   QFont::Normal).toInt();
    font_size   = settings->value("FontSize",     10).toInt();
    font_name   = settings->value("FontName",     "Courier").toString();

    if(font_size > 72) font_size = 72;
    if(font_size < 6)  font_size = 6;

    font.setFamily(font_name);
    font.setWeight(font_weight);
    font.setPointSize(font_size);

    logBox->setFont(font);

    settings->endGroup();
    delete settings;
}
//--------------------------------------------------------------------------------
void LogBox::save_settings(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(!mw) return;

#ifdef QT_DEBUG
    qDebug() << "LogBox::save_settings(void)";
#endif

    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);
    settings->beginGroup("LogEdit");
    settings->setValue("readOnly",      (bool)logBox->isReadOnly());
    settings->setValue("acceptRichText",(bool)logBox->acceptRichText());
    settings->setValue("no_CRLF",       (bool)flagNoCRLF);
    settings->setValue("addDateTime",   (bool)flagAddDateTime);
    settings->setValue("color",         (bool)flagColor);
    settings->setValue("auto_load",     (bool)flagAutoLoad);
    settings->setValue("auto_save",     (bool)flagAutoSave);
    settings->setValue("ErrorAsMessage",(bool)flagErrorAsMessage);
    settings->setValue("TextIsWindows", (bool)flagTextIsWindows);

#ifndef NO_LOG
    QFont font = get_font();
    settings->setValue("FontWeight",  font.weight());
    settings->setValue("FontSize",    font.pointSize());
    settings->setValue("FontName",    font.family());
#endif

    settings->endGroup();
    delete settings;
}
//--------------------------------------------------------------------------------
