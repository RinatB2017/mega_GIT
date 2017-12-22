#include "lockscreen.h"

LockScreen::LockScreen(QWidget *parent) :
    QFrame(parent)
{
    settings = new QSettings;

    QPushButton *btnLockIcon = new QPushButton(QIcon(":/img/lock.png"),"");
    btnLockIcon->setIconSize(QSize(200,200));
    editPass = new QLineEdit;
    editPass->setEchoMode(QLineEdit::Password);
    connect(editPass,SIGNAL(returnPressed()),this,SLOT(unlock()));

    QToolButton *btnUnlock = new QToolButton;
    btnUnlock->setText(tr("Unlock"));
    connect(btnUnlock,SIGNAL(clicked()),this,SLOT(unlock()));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(btnLockIcon);
    mainLayout->addWidget(editPass);
    mainLayout->addWidget(btnUnlock);
    mainLayout->setAlignment(btnUnlock,Qt::AlignCenter);
    setLayout(mainLayout);

    //---------------------------------

    QToolButton *btnInfo = new QToolButton;
    btnInfo->setText(tr("Informations"));
    btnInfo->setIcon(QIcon(":/img/information.png"));
    btnInfo->setObjectName("BtnInfo");
    btnInfo->setIconSize(QSize(20,20));
    lblText = new QLabel;

    connect(btnInfo,SIGNAL(clicked()),this,SLOT(showIndication()));

    QHBoxLayout *layInfo = new QHBoxLayout;
    layInfo->addWidget(btnInfo);
    layInfo->addWidget(lblText);

    widgetInfo = new QWidget(this);
    widgetInfo->setLayout(layInfo);
    widgetInfo->move(15,this->geometry().height()-(15+widgetInfo->geometry().height()));
    widgetInfo->resize(500,80);

    applyStyle();
}

void LockScreen::resizeEvent(QResizeEvent *event)
{
    widgetInfo->move(15,this->geometry().height()-(15+widgetInfo->geometry().height()));
    event->accept();
}

void LockScreen::showIndication()
{
    if(!settings->value("Account/Password").toString().isEmpty())
    {
        QMenu menu;
        menu.addAction(QIcon(":/img/information.png"),settings->value("Account/Indice").toString());
        menu.exec(QCursor::pos());
    }
}

void LockScreen::launch()
{
    if(settings->value("Account/Password").toString().isEmpty()||settings->value("Security/Lock/Locker").toBool()==false)
        editPass->setVisible(false);
    else if(!settings->value("Account/Password").toString().isEmpty()||settings->value("Security/Lock/Locker").toBool()==true)
        editPass->setVisible(true);

    editPass->clear();
    editPass->setFocus();

    lblText->setText(tr("This browser is locked<br />by <b>") + settings->value("Account/Pseudo").toString() + tr("</b>."));
}

void LockScreen::unlock()
{
    if(editPass->text()==settings->value("Account/Password").toString()||settings->value("Security/Lock/Locker").toBool()==false)
        emit canUnlock();
    else
    {
        qApp->beep();
        lblText->setText(tr("Incorrect Password"));
        editPass->selectAll();
        editPass->setFocus();
    }
}

// ========================================================================== POSSIBILITE DE VERROUILLER SANS MOT DE PASSE

void LockScreen::applyStyle()
{
    this->setStyleSheet("\
                        QFrame {\
                            border: none;\
                            background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 transparent, stop: 1.0 rgba(0,0,0,.5));\
                            }\
                        QLineEdit {\
                            border: none;\
                            background: white;\
                            color: black;\
                            padding: 6px 10px;\
                            font-size: 14px;\
                            font-family: segoe ui;\
                            max-width: 400px;\
                            }\
                        QToolButton {\
                            border: 2px solid white;\
                            background: transparent;\
                            color: white;\
                            padding: 4px 12px;\
                            border-radius: 16px;\
                            font-size: 14px;\
                            font-family: segoe ui;\
                            min-width: 100px;\
                            }\
                        QToolButton:hover {\
                            background: rgba(0,0,0,.2);\
                            }\
                        QPushButton {\
                            border: none;\
                            background: transparent;\
                            padding: 0px; margin: 0px;\
                            margin-top: -50px;\
                            }\
                        QLabel {\
                            background: transparent;\
                            color: white;\
                            font-size: 14px;\
                            font-family: segoe ui;\
                            }\
                        #BtnInfo {\
                            border-radius: 23px;\
                            min-width: 0px;\
                            padding: 10px 10px;\
                            margin-right: 15px;\
                            border: 3px solid white;\
                            }");
}
