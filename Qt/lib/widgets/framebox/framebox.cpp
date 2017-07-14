//--------------------------------------------------------------------------------
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTextStream>
#include <QString>
#include <QLabel>
#include <QFile>
//--------------------------------------------------------------------------------
#include "framebox.hpp"
//--------------------------------------------------------------------------------
FrameBox::FrameBox(QWidget *parent) :
    QFrame(parent)
{
    vbox = new QVBoxLayout();

    topbox = new QHBoxLayout();
    middlebox = new QHBoxLayout();
    bottombox = new QHBoxLayout();

    workbox = new QVBoxLayout();
    workbox->addStretch();

    spaceframe = new QFrame();
    spaceframe2 = new QFrame();

    p1.load(QLatin1String(":images/dialog-close.png"));
    p2.load(QLatin1String(":images/dialog-close.png"));
    p3.load(QLatin1String(":images/dialog-close.png"));
    p4.load(QLatin1String(":images/dialog-close.png"));

    label1 = new QLabel();
    label2 = new QLabel();
    label3 = new QLabel();
    label4 = new QLabel();

    label1->setPixmap(p1);
    label2->setPixmap(p2);
    label3->setPixmap(p3);
    label4->setPixmap(p4);

    topbox->addWidget(label1);
    topbox->addStretch();
    topbox->addWidget(label2);

    middlebox->addWidget(spaceframe);
    middlebox->addLayout(workbox);
    middlebox->addWidget(spaceframe2);

    bottombox->addWidget(label3);
    bottombox->addStretch();
    bottombox->addWidget(label4);

    vbox->addLayout(topbox);
    vbox->addLayout(middlebox);
    vbox->addLayout(bottombox);

    setLayout(vbox);

    setMinimumSize(320, 240);
    setFrameStyle(QFrame::Box | QFrame::Raised);
}
//--------------------------------------------------------------------------------
FrameBox::~FrameBox()
{
    delete spaceframe;
    delete spaceframe2;

    delete label1;
    delete label2;
    delete label3;
    delete label4;

    delete topbox;
    delete workbox;
    delete middlebox;
    delete bottombox;

    delete vbox;
}
//--------------------------------------------------------------------------------
void FrameBox::loadStyle(const QString &name)
{
    QFile file(name);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        QString s = in.readAll();
        setStyleSheet(s);
    }
    else
    {
        emit log(QString(QObject::tr("file %1 not found")).arg(name));
    }
}
//--------------------------------------------------------------------------------
void FrameBox::addWidget(QWidget *widget)
{
    workbox->addWidget(widget);
}
//--------------------------------------------------------------------------------
void FrameBox::addLayout(QLayout *layout)
{
    workbox->addLayout(layout);
}
//--------------------------------------------------------------------------------
#if 0
void FrameBox::paintEvent ( QPaintEvent * /*event*/ )
{
    QPainter p(this);
    p.save();
    p.drawEllipse(10, 10, 10, 10);
    p.drawEllipse(width()-20, 10, 10, 10);
    p.drawEllipse(width()-20, height()-20, 10, 10);
    p.drawEllipse(10, height()-20, 10, 10);
//    p.drawLine(0, 0, 10, 10);
    p.restore();
}
#endif
//--------------------------------------------------------------------------------
