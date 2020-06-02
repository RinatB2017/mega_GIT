#include "charts/pie/dependencywheelwidget.h"
#include <QDebug>

namespace d3widgets {

DependencyWheelWidget::DependencyWheelWidget(QWidget *parent) : d3widgets::D3Viewer(parent),
    mPadding(0.02),
    mMargin(90)
{
    addContextObject("api", this);
    load("qrc:/html/html/dependency_wheel.html");
    connect(this, SIGNAL(sizeChanged(QSize)), this, SIGNAL(update()));
}

QVariant DependencyWheelWidget::data()
{
    QVariantMap from;
    QMapIterator<QString, QSet<QString> > i(dependencies);
    while (i.hasNext()) {
        i.next();
        QVariantList to;
        foreach (const QString &value, i.value())
        {
            to.append(value);
        }
        from[i.key()] = to;
    }
    return QVariant(from);
}

int DependencyWheelWidget::margin()
{
    return mMargin;
}

float DependencyWheelWidget::padding()
{
    return mPadding;
}

void DependencyWheelWidget::setPadding(const float padding)
{
    if(mPadding != padding)
    {
        mPadding = padding;
        emit update();
    }
}

void DependencyWheelWidget::setMargin(const int margin)
{
    if(mMargin != margin)
    {
        mMargin = margin;
        emit update();
    }

}

void DependencyWheelWidget::appendDependency(const QString &from, const QString &to)
{
    dependencies[from].insert(to);
}

void DependencyWheelWidget::clearDependencies()
{
    dependencies.clear();
    emit update();
}

void DependencyWheelWidget::itemClicked(const QString &itemName)
{
    qDebug() << itemName;
}

}
