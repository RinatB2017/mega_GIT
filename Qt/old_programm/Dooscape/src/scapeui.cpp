#include "scapeui.h"

ScapeUI::ScapeUI(QObject *parent) :
    QObject(parent)
{
}

HideBlock::HideBlock(QString title, QWidget *parent) :
    QFrame(parent)
{
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    this->setStyleSheet("QFrame {margin-bottom: 30px;}");

    btnTitle = new QToolButton;
    btnTitle->setText(title);
    btnTitle->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    btnTitle->setStyleSheet("QToolButton {border: 1px solid rgba(255,255,255,.5); background: rgba(255,255,255,.1);} QToolButton:hover {border: 1px solid white; background: rgba(255,255,255,.3);}");
    btnTitle->setIcon(QIcon(":/img/down.png"));
    btnTitle->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    mainLayout->addWidget(btnTitle);
    connect(btnTitle,SIGNAL(clicked()),this,SLOT(showBlock()));

    block = new QFrame;
    block->hide();
    block->setStyleSheet("QFrame {border: 1px solid rgba(255,255,255,.5); border-top: none;} QLabel {border: none; margin: 0px;} QToolButton {border: 1px solid rgba(255,255,255,.5); border-radius: 5px;}");
    mainLayout->addWidget(block);
}

void HideBlock::setBlockLayout(QLayout *layout)
{
    block->setLayout(layout);
    blockHeight = block->geometry().height();
}

void HideBlock::showBlock()
{
    if(block->isVisible())
    {
        btnTitle->setIcon(QIcon(":/img/down.png"));

        QPropertyAnimation *animation = new QPropertyAnimation(block,"maximumHeight");
        animation->setDuration(350);
        animation->setEasingCurve(QEasingCurve::OutCirc);
        animation->setStartValue(blockHeight);
        animation->setEndValue(0);
        animation->start();

        connect(animation,SIGNAL(finished()),block,SLOT(hide()));
    }
    else
    {
        block->show();
        btnTitle->setIcon(QIcon(":/img/up.png"));

        QPropertyAnimation *animation = new QPropertyAnimation(block,"maximumHeight");
        animation->setDuration(500);
        animation->setEasingCurve(QEasingCurve::InCirc);
        animation->setStartValue(0);
        animation->setEndValue(blockHeight+10000);
        animation->start();
    }
}
