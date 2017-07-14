/*********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#include <QRect>
//--------------------------------------------------------------------------------
#include "aboutbox.hpp"
#include "ui_aboutbox.h"
//--------------------------------------------------------------------------------
#ifdef LOGO_GL
    #include "glwidget.h"
#endif
//--------------------------------------------------------------------------------
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    #define ICON_ABOUT_QT   ":/trolltech/qmessagebox/images/qtlogo-64.png"
#else
    #define ICON_ABOUT_QT   ":/qt-project.org/qmessagebox/images/qtlogo-64.png"
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
    text = QString(tr("<center>%1<br><br><b>%2</b><br><i>%3</i><br><br><i>%4</i>"))
            .arg(orgName)
            .arg(programmName)
            .arg(QString(tr("version %1")).arg(version))
            .arg(author);
    ui->teAbout->setText(text);

#ifdef LOGO_GL
    GLWidget *glWidget = new GLWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(glWidget);

    ui->labelLogo->clear();
    ui->labelLogo->setLayout(vbox);
#endif

    QRect rect = geometry();
    setFixedSize(rect.width(), rect.height());

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

