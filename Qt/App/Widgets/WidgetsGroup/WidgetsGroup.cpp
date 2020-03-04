#include "WidgetsGroup.h"
#include "ui_WidgetsGroup.h"

#include <QChildEvent>
#include <QVBoxLayout>
#include <QDebug>
#include <QCoreApplication>

WidgetsGroup::WidgetsGroup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetsGroup)
{
    uiwasntbuilt = true;
    ui->setupUi(this);
    qDebug() << "------";
    for (QObjectList::const_iterator i = this->children().begin();
        i != this->children().end(); ++i)
    {
        qDebug() << (*i)->objectName();
    }
    uiwasntbuilt = false;
    qDebug() << "------";
    connect(this, SIGNAL(wantAddLayout(QLayout*)), this, SLOT(addLayout(QLayout*)), Qt::QueuedConnection);
}

WidgetsGroup::~WidgetsGroup()
{
    delete ui;
}

void WidgetsGroup :: addLayout (QLayout* layout)
{
    if (ui->ContentsFrame->layout() != nullptr) delete ui->ContentsFrame->layout();
    ui->ContentsFrame->setLayout(layout);
}

void WidgetsGroup :: childEvent (QChildEvent* event)
{
    if (!uiwasntbuilt)
    {
        qDebug() << "Was in addto";
        if (event->added())
        {
            QObject* child = event->child();
            qDebug() << child->metaObject()->className() << child->objectName();
            if (event->child()->metaObject()->className() == QString("QObject").toLatin1())
            {
                //ui->ContentsFrame->setLayout(static_cast<QLayout*>(event->child()));
                //QChildEvent* childevent = new QChildEvent(QEvent::ChildAdded, child);
                //QCoreApplication::postEvent(ui->ContentsFrame, childevent);
                emit wantAddLayout(static_cast<QLayout*>(event->child()));
                event->ignore();
            }
            else if (event->child()->isWidgetType())
            {
                //ui->ContentsFrame->layout()->addWidget(static_cast<QWidget*>(event->child()));
                event->ignore();
            }
        }
    }
    else
    {
        qDebug() << "Wasn't in addto";
        event->accept();
        QWidget::childEvent(event);
    }
}

void WidgetsGroup :: addWidget(QWidget* widget)
{
    if (ui->ContentsFrame->layout() != nullptr)
        ui->ContentsFrame->layout()->addWidget(widget);
}

void WidgetsGroup::switchContents ()
{
    if (ui->ContentsFrame->isVisible()) ui->ContentsFrame->setVisible(false);
    else ui->ContentsFrame->setVisible(true);
    qDebug() << "switch";
}
