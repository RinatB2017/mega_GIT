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
#include <QDesktopServices>
#include <QRect>
#include <QUrl>
//--------------------------------------------------------------------------------
#include "aboutbox.hpp"
#include "ui_aboutbox.h"
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
                   const QString &author,
                   QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutBox)
{
    ui->setupUi(this);

    QString text;
    text = QString(tr("<b>%1</b><br><br><b>%2</b><br><i>%3</i><br><br><i>%4</i>"))
            .arg(orgName)
            .arg(programmName)
            .arg(QString(tr("version %1")).arg(version))
            .arg(QString("<a href='mailto:tux4096@gmail.com'>%1</a>").arg(author));

    ui->lbl_email->setAlignment(Qt::AlignTop);
    ui->lbl_email->setText(text);
    ui->lbl_email->connect(ui->lbl_email, &QLabel::linkActivated, [](const QString &link)
    {
        QDesktopServices::openUrl(QUrl(link));
    });

#ifdef LOGO_GL
    GLWidget *glWidget = new GLWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(glWidget);

    ui->labelLogo->clear();
    ui->labelLogo->setLayout(vbox);
#endif

    setFixedSize(sizeHint());

    ui->btn_about_qt->setIcon(QPixmap(QLatin1String(ICON_ABOUT_QT)));
    ui->btn_about_qt->setToolTip(tr("About Qt"));
    ui->btn_about_qt->setStatusTip(tr("About Qt"));
    connect(ui->btn_about_qt, SIGNAL(clicked()),    qApp, SLOT(aboutQt()));

    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}
//--------------------------------------------------------------------------------
AboutBox::~AboutBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------

