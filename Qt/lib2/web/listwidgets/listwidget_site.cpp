//--------------------------------------------------------------------------------
#include <QListWidgetItem>
#include <QElapsedTimer>
#include <QApplication>
#include <QInputDialog>
#include <QWebView>
#include <QMenu>
//--------------------------------------------------------------------------------
#include "listwidget_site.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
ListWidget_Site::ListWidget_Site(QWebView *webview,
                                 QWidget *parent) :
    QListWidget(parent),
    web(webview)
{
    if(parent)
    {
        connect(this, SIGNAL(info(QString)),  parent, SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parent, SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parent, SIGNAL(error(QString)));
        connect(this, SIGNAL(trace(QString)), parent, SIGNAL(error(QString)));
    }
    else
    {
        connect(this, SIGNAL(info(QString)),  this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(trace(QString)), this, SLOT(log(QString)));
    }

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(popup(QPoint)));
    connect(this, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(itemChanged(QListWidgetItem*)));

    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(clicked(QModelIndex)));

    connect(web, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
}
//--------------------------------------------------------------------------------
void ListWidget_Site::log(const QString &text)
{
    qDebug() << text;
}
//--------------------------------------------------------------------------------
void ListWidget_Site::loadFinished(bool state)
{
    if(state == false)
        return;

    is_loaded = true;
}
//--------------------------------------------------------------------------------
void ListWidget_Site::add_item(const QString &name)
{
    if(name.isEmpty()) return;

    QListWidgetItem *item = new QListWidgetItem;
    item->setText(name);
    addItem(item);
}
//--------------------------------------------------------------------------------
void ListWidget_Site::popup(QPoint point)
{
    QMenu *popup_menu = new QMenu(this);
    QAction *add_action = new QAction(qApp->style()->standardIcon(QStyle::SP_FileDialogNewFolder),
                                      QObject::tr("add site"),
                                      this);
    popup_menu->addAction(add_action);

    connect(popup_menu, SIGNAL(triggered(QAction*)), this, SLOT(add_site(QAction*)));

    popup_menu->exec(QCursor::pos());
    Q_UNUSED(point)
}
//--------------------------------------------------------------------------------
void ListWidget_Site::itemChanged(QListWidgetItem *item)
{
    emit debug(QString("%1 %2")
               .arg(item->text())
               .arg(item->data(Qt::CheckStateRole).toBool()));
}
//--------------------------------------------------------------------------------
void ListWidget_Site::clicked(QModelIndex index)
{
    QString address = item(index.row())->text();
    if(address.isEmpty())
        return;

    is_loaded = false;
    web->load(address);
    wait();

    emit ready(QModelIndex());
    emit debug(QString("%1").arg(address));
}
//--------------------------------------------------------------------------------
void ListWidget_Site::add_site(QAction *action)
{
    bool ok;
    QString text = QInputDialog::getText(this,
                                         QObject::tr("add site"),
                                         QObject::tr("site:"),
                                         QLineEdit::Normal,
                                         "",
                                         &ok);
    if(ok && !text.isEmpty())
        add_item(text);

    Q_UNUSED(action)
}
//--------------------------------------------------------------------------------
void ListWidget_Site::wait(int timeout_msec)
{
    QElapsedTimer time;

    //emit debug("waiting...");
    time.start();
    while(!is_loaded)
    {
        QCoreApplication::processEvents();
        if(time.elapsed() > timeout_msec)
            break;
    }
}
//--------------------------------------------------------------------------------
