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
#include "aboutbox.hpp"
#include "ui_aboutbox.h"
//--------------------------------------------------------------------------------
#ifdef LOGO_GL
#   include "glwidget.h"
#endif
//--------------------------------------------------------------------------------
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#   define ICON_ABOUT_QT   ":/trolltech/qmessagebox/images/qtlogo-64.png"
#else
#   define ICON_ABOUT_QT   ":/qt-project.org/qmessagebox/images/qtlogo-64.png"
#endif
//--------------------------------------------------------------------------------
AboutBox::AboutBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutBox)
{
    init();
}
//--------------------------------------------------------------------------------
AboutBox::~AboutBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void AboutBox::init(void)
{
    ui->setupUi(this);

    connect(ui->lbl_email,      &QLabel::linkActivated, this,   &AboutBox::send_mail);
    connect(ui->lbl_telegram,   &QLabel::linkActivated, this,   &AboutBox::go_telegramm);

#ifdef LOGO_GL
    GLWidget *glWidget = new GLWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(glWidget);

    ui->lbl_avatar->setFixedSize(312, 312);

    ui->lbl_avatar->clear();
    ui->lbl_avatar->setLayout(vbox);
#else
    QTimer::singleShot(0, [this]{
        setFixedSize(sizeHint());
    });
#endif

    ui->btn_about_qt->setIcon(QIcon(QLatin1String(ICON_ABOUT_QT)));
    ui->btn_about_qt->setToolTip(tr("About Qt"));
    ui->btn_about_qt->setStatusTip(tr("About Qt"));
    connect(ui->btn_about_qt,   &QToolButton::clicked,  qApp,   &QApplication::aboutQt);

    connect(ui->buttonBox,  &QDialogButtonBox::rejected, this, &AboutBox::close);

    ui->btn_env->setIcon(qApp->style()->standardIcon(QStyle::SP_MessageBoxInformation));
    ui->btn_env->setToolTip(tr("Get ENV"));
    ui->btn_env->setStatusTip(tr("Get ENV"));

    connect(ui->btn_env,    &QToolButton::clicked,  this,   &AboutBox::show_env);
    ui->btn_env->setVisible(false);
}
//--------------------------------------------------------------------------------
void AboutBox::show_env(void)
{
    QStringList sl = QProcessEnvironment::systemEnvironment().toStringList();
    QTextEdit *te = new QTextEdit();
    Q_ASSERT(te);
    te->setObjectName("te");
    te->setWindowModality(Qt::WindowModal);
    te->setMinimumSize(800, 600);
    te->setReadOnly(true);
    foreach (QString text, sl)
    {
        te->append(text);
    }
    te->show();
}
//--------------------------------------------------------------------------------
QString AboutBox::get_avatar(void)
{
    return avatar;
}
//--------------------------------------------------------------------------------
QString AboutBox::get_telegram_qr(void)
{
    return telegram_qr;
}
//--------------------------------------------------------------------------------
QString AboutBox::get_orgName(void)
{
    return orgName;
}
//--------------------------------------------------------------------------------
QString AboutBox::get_programmName(void)
{
    return programmName;
}
//--------------------------------------------------------------------------------
QString AboutBox::get_version(void)
{
    return version;
}
//--------------------------------------------------------------------------------
QString AboutBox::get_email(void)
{
    return email;
}
//--------------------------------------------------------------------------------
QString AboutBox::get_author(void)
{
    return author;
}
//--------------------------------------------------------------------------------
QString AboutBox::get_telegram(void)
{
    return telegram;
}
//--------------------------------------------------------------------------------
void AboutBox::set_avatar(const QString &value)
{
    avatar = value;
    ui->lbl_avatar->setPixmap(QPixmap(avatar));
}
//--------------------------------------------------------------------------------
void AboutBox::set_telegram_qr(const QString &value)
{
    telegram_qr = value;
    ui->lbl_telegram_qr->setPixmap(QPixmap(telegram_qr));
}
//--------------------------------------------------------------------------------
void AboutBox::set_orgName(const QString &value)
{
    orgName = value;
    ui->lbl_orgName->setText(orgName);
}
//--------------------------------------------------------------------------------
void AboutBox::set_programmName(const QString &value)
{
    programmName = value;
    ui->lbl_programmName->setText(programmName);
}
//--------------------------------------------------------------------------------
void AboutBox::set_version(const QString &value)
{
    version = value;
    ui->lbl_version->setText(QString("version %1").arg(version));
}
//--------------------------------------------------------------------------------
void AboutBox::set_email(const QString &value)
{
    email = value;
    ui->lbl_email->setText(QString("<a href='mailto:%1'>%2</a>").arg(email).arg(author));
    ui->lbl_email->setToolTip(email);
}
//--------------------------------------------------------------------------------
void AboutBox::set_author(const QString &value)
{
    author = value;
}
//--------------------------------------------------------------------------------
void AboutBox::set_telegram(const QString &value)
{
    telegram = value;
    ui->lbl_telegram->setText(QString("<a href='%1'>Telegram</a>").arg(telegram));
    ui->lbl_telegram->setToolTip(telegram);
}
//--------------------------------------------------------------------------------
void AboutBox::send_mail(QString link)
{
    QDesktopServices::openUrl(QUrl(link));
}
//--------------------------------------------------------------------------------
void AboutBox::go_telegramm(QString link)
{
    QDesktopServices::openUrl(QUrl(link));
}
//--------------------------------------------------------------------------------

