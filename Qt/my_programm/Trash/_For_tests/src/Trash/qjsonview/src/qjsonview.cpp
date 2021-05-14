#include <QGridLayout>
#include <QPainter>
#include <QVariantMap>
#include <QContextMenuEvent>
#include <QMenu>
#include <QClipboard>
#include <QApplication>
#include <QMouseEvent>
#include <QTextDocument>
#include <QDebug>
#include <QToolTip>

#include "qjsonview.hpp"
#include "qjson.h"

#define EXPANDABLE_MARGIN_LEFT 14
#define EXPANDED_MARGIN_LEFT 21


QJsonView::QJsonView(QWidget *parent) :
    QWidget(parent),
    lblSingle(new QLabel(this)),
    expanded(false),
    hoverEffectsEnabled(false)
{
    //needed for hover effects
    setAutoFillBackground(true);

    QGridLayout *layout = new QGridLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    setLayout(layout);

    //default: show one single QLabel with the whole value as its content
    layout->addWidget(lblSingle);
    lblSingle->setAutoFillBackground(true);
    lblSingle->setCursor(Qt::ArrowCursor);
    setValue(QVariant());
}

void QJsonView::setValue(QVariant value)
{
    if(expanded) collapse();

    v = value;
    lblSingle->setText(QString("<span style=\"font-family: monospace; overflow: hidden\">%1</span>")
                       .arg(variantToHtml(v)));
    layout()->setContentsMargins(isExpandable() ? EXPANDABLE_MARGIN_LEFT : 0, 0, 0, 0);

    //show hand cursor if expandable
    Qt::CursorShape cursor;
    if(isExpandable())
        cursor = Qt::PointingHandCursor;
    else
        cursor = Qt::ArrowCursor;
    setCursor(cursor);
    lblSingle->setCursor(cursor);

    update();
    emit resized();
}

void QJsonView::setJsonValue(QString json)
{
    setValue(QJson::decode(json));
}

void QJsonView::setHoverEffects(bool enabled)
{
    hoverEffectsEnabled = enabled;
    if(!hoverEffectsEnabled)
        unhover();
}

bool QJsonView::hoverEffects()
{
    //if my parent is also a QJsonView, return its property
    QJsonView *p = qobject_cast<QJsonView*>(parentWidget());
    if(p)
        return p->hoverEffects();
    else
        return hoverEffectsEnabled;
}

QString QJsonView::variantToHtml(QVariant data)
{
    if(data.type() == QVariant::String || data.type() == QVariant::ByteArray)
        return "<span style=\"color: #006000\">\"" + Qt::escape(data.toString()) + "\"</span>";
    else if(data.type() == QVariant::Int || data.type() == QVariant::LongLong)
        return "<span style=\"color: #800000\">" + Qt::escape(data.toString()) + "</span>";
    else if(data.type() == QVariant::Double)
        return "<span style=\"color: #800080\">" + Qt::escape(data.toString()) + "</span>";
    else if(data.type() == QVariant::Bool || data.isNull() || !data.isValid())
    {
        QString str = "null";
        if(data.type() == QVariant::Bool)
            str = data.toBool() ? "true" : "false";
        return "<span style=\"color: #000080\">" + str + "</span>";
    }

    else if(data.type() == QVariant::List)
    {
        QString str = "<span style=\"color: #606060\"><b>[</b></span>";
        bool first = true;
        foreach(QVariant e, data.toList())
        {
            if(!first)
                str += "<span style=\"color: #606060\"><b>, </b></span>";
            first = false;
            str += variantToHtml(e);
        }
        str += "<span style=\"color: #606060\"><b>]</b></span>";
        return str;
    }

    else if(data.type() == QVariant::Map)
    {
        QString str = "<span style=\"color: #606060\"><b>{</b></span>";
        QVariantMap map(data.toMap());

        //special entry: "children" => tree view
        bool containsChildren = false;
        QVariant children;
        if(map.contains("children")) {
            children = map.take("children");
            containsChildren = true;
        }

        //normal entries
        QVariantMap::iterator i;
        for(i = map.begin(); i != map.end(); ++i)
        {
            if(i != map.begin())
                str += "<span style=\"color: #606060\"><b>, </b></span>";
            str += Qt::escape(i.key()) + ": " + variantToHtml(i.value());
        }
        //entry "children"
        if(containsChildren) {
            if(!map.isEmpty())
                str += "<span style=\"color: #606060\"><b>, </b></span>";
            str += "children: " + variantToHtml(children);
        }

        str += "<span style=\"color: #606060\"><b>}</b></span>";
        return str;
    }

    else
        return data.toString();
}

void QJsonView::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    // i designed the graphics using a pixel font size of 15, so this should be scalable now.
    qreal scale = fontMetrics().height() / 15.0;
    p.scale(scale, scale);

    int h = height() / scale;

    p.drawRect(2, 2, 10, 10);
    p.drawLine(5, 7, 9, 7);
    if(!expanded)
        p.drawLine(7, 5, 7, 9);

    if(expanded)
    {
        QColor color(96, 96, 96);
        if(v.type() == QVariant::List)
        {
            p.fillRect(16, 2, 4, 1, color);
            p.fillRect(16, 3, 2, h - 6, color);
            p.fillRect(16, h - 3, 4, 1, color);
        }
        else
        {
            int mid = h / 2;
            p.fillRect(18, 2, 4, 1, color);
            p.fillRect(17, 3, 2, mid - 4, color);
            p.fillRect(16, mid - 1, 3, 1, color);
            p.fillRect(15, mid    , 3, 1, color);
            p.fillRect(16, mid + 1, 3, 1, color);
            p.fillRect(17, mid + 2, 2, h - mid - 5, color);
            p.fillRect(18, h - 3, 4, 1, color);
        }
    }
}

void QJsonView::mousePressEvent(QMouseEvent *e)
{
    if(isExpandable()
            && e->button() == Qt::LeftButton
            && (!expanded || e->x() < EXPANDED_MARGIN_LEFT))
    {
        if(!expanded)
            expand();
        else
            collapse();
    }
}

void QJsonView::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu menu(this);

    //copy value to clipboard
    QAction *copy;
    if(v.type() == QVariant::List || v.type() == QVariant::Map)
        copy = menu.addAction(tr("Copy value (JSON encoded)"));
    else if(v.type() == QVariant::String || v.type() == QVariant::ByteArray)
        copy = menu.addAction(tr("Copy string value"));
    else
        copy = menu.addAction(tr("Copy value"));

    //execute menu
    QAction *triggeredAction = menu.exec(e->globalPos());

    if(triggeredAction == copy)
    {
        QClipboard *clipboard = QApplication::clipboard();
        if(v.type() == QVariant::List || v.type() == QVariant::Map || v.type() == QVariant::Bool || v.isNull() || !v.isValid())
            clipboard->setText(QJson::encode(v, QJson::EncodeOptions(QJson::Compact | QJson::EncodeUnknownTypesAsNull)));
        else
            clipboard->setText(v.toString());
    }
}

void QJsonView::enterEvent(QEvent *)
{
    hover();

    //if my parent is also a QJsonView, i inform it that i have been entered
    QJsonView *p = qobject_cast<QJsonView*>(parentWidget());
    if(p) p->childEntered();
}

void QJsonView::leaveEvent(QEvent *)
{
    unhover();

    //if my parent is also a QJsonView, i inform it that i have been leaved
    QJsonView *p = qobject_cast<QJsonView*>(parentWidget());
    if(p) p->childLeaved();
}

bool QJsonView::isExpandable()
{
    return (v.type() == QVariant::List && !v.toList().isEmpty()) ||
            (v.type() == QVariant::Map && !v.toMap().isEmpty());
}

bool QJsonView::isExpanded()
{
    return expanded;
}

void QJsonView::setExpanded(bool expanded)
{
    if(expanded)
        expand();
    else
        collapse();
}

void QJsonView::expand()
{
    if(isExpandable())
    {
        lblSingle->setVisible(false);
        layout()->removeWidget(lblSingle);

        if(v.type() == QVariant::List)
        {
            foreach(QVariant e, v.toList())
            {
                QJsonView *w = new QJsonView(this);
                w->setValue(e);
                layout()->addWidget(w);
                childWidgets << w;
                //propagate signals to parent
                connect(w, SIGNAL(resized()), SIGNAL(resized()));
            }
        }
        else if(v.type() == QVariant::Map)
        {
            QVariantMap map(v.toMap());

            //normal entries
            QVariantMap::iterator i;
            int index = 0;
            QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
            sizePolicy.setHorizontalStretch(0);
            sizePolicy.setVerticalStretch(0);
            for(i = map.begin(); i != map.end(); ++i)
            {
                QLabel *k = new QLabel(this);
                k->setText("<span style=\"font-family: monospace\">" + Qt::escape(i.key()) + ": </span>");
                k->setAlignment(Qt::AlignTop | Qt::AlignLeft);
                k->setCursor(Qt::ArrowCursor);
                k->setAutoFillBackground(true);
                ((QGridLayout*)layout())->addWidget(k, index, 0);
                childWidgets << k;

                QJsonView *w = new QJsonView(this);
                w->setValue(i.value());
                ((QGridLayout*)layout())->addWidget(w, index, 1);
                w->setSizePolicy(sizePolicy);
                childWidgets << w;
                //propagate signals to parent
                connect(w, SIGNAL(resized()), SIGNAL(resized()));

                index++;
            }
        }

        layout()->setContentsMargins(EXPANDED_MARGIN_LEFT, 0, 0, 0);
        expanded = true;
        update();
        emit resized();
    }
}

void QJsonView::collapse()
{
    if(isExpandable())
    {
        foreach(QWidget *w, childWidgets)
        {
            w->deleteLater();
            layout()->removeWidget(w);
        }
        childWidgets.clear();

        lblSingle->setVisible(true);
        layout()->addWidget(lblSingle);

        layout()->setContentsMargins(isExpandable() ? EXPANDABLE_MARGIN_LEFT : 0, 0, 0, 0);
        expanded = false;
        update();
        emit resized();
    }
}

void QJsonView::childEntered()
{
    unhover();
}

void QJsonView::childLeaved()
{
    hover();
}

void QJsonView::hover()
{
    if(hoverEffects())
    {
        QPalette pal = palette();
        pal.setColor(backgroundRole(), Qt::white);
        setPalette(pal);
    }
}

void QJsonView::unhover()
{
    setPalette(parentWidget()->palette());
}
