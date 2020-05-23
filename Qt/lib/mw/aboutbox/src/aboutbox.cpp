/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#ifdef HAVE_QT5
#   include<QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
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
AboutBox::AboutBox(const QString &orgName,
                   const QString &programmName,
                   const QString &version,
                   const QString &email,
                   const QString &author,
                   const QString &telegram,
                   QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutBox)
{
    ui->setupUi(this);

    ui->lbl_orgName->setText(orgName);
    ui->lbl_programmName->setText(programmName);
    ui->lbl_version->setText(QString(tr("version %1")).arg(version));
    ui->lbl_email->setText(QString("<a href='mailto:%1'>%2</a>").arg(email).arg(author));
    ui->lbl_telegram->setText(QString("<a href='%5'>Telegram</a>").arg(telegram));

    ui->lbl_email->setToolTip(email);
    ui->lbl_telegram->setToolTip(telegram);

#if 0
    QString text;
    text = QString("<b>%1</b><br><br><b>%2</b><br><i>%3</i><br><br><i>%4<br><i>%5</i>")
            .arg(orgName)
            .arg(programmName)
            .arg(QString(tr("version %1")).arg(version))
            .arg(QString("<a href='mailto:tux4096@gmail.com'>%1</a>").arg(author))
            .arg(QString("<a href='%5'>Telegram</a>").arg(telegram));
#endif

    connect(ui->lbl_email,      &QLabel::linkActivated, this,   &AboutBox::send_mail);
    connect(ui->lbl_telegram,   &QLabel::linkActivated, this,   &AboutBox::go_telegramm);

#ifdef LOGO_GL
    GLWidget *glWidget = new GLWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(glWidget);

    ui->labelLogo->setFixedSize(312, 312);   //TODO про фотографию не забыть

    ui->labelLogo->clear();
    ui->labelLogo->setLayout(vbox);
#else
    setFixedSize(sizeHint());
#endif

    ui->btn_about_qt->setIcon(QPixmap(QLatin1String(ICON_ABOUT_QT)));
    ui->btn_about_qt->setToolTip(tr("About Qt"));
    ui->btn_about_qt->setStatusTip(tr("About Qt"));
    connect(ui->btn_about_qt,   &QToolButton::clicked,  qApp,   &QApplication::aboutQt);

    connect(ui->buttonBox,  &QDialogButtonBox::rejected, this, &AboutBox::close);

    ui->btn_env->setIcon(QIcon(qApp->style()->standardIcon(QStyle::SP_MessageBoxInformation)));
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
    te->setObjectName("te");
    te->setWindowModality(Qt::WindowModal);
    te->setMinimumSize(800, 600);
    te->setReadOnly(true);
    foreach (auto text, sl)
    {
        te->append(text);
    }
    te->show();
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
AboutBox::~AboutBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void AboutBox::changeEvent(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::LanguageChange:
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------

