//--------------------------------------------------------------------------------
#ifndef ListWidget_Site_HPP
#define ListWidget_Site_HPP
//--------------------------------------------------------------------------------
#include <QListWidget>
//--------------------------------------------------------------------------------
class QListWidgetItem;
class QWebView;
//--------------------------------------------------------------------------------
class ListWidget_Site : public QListWidget
{
    Q_OBJECT
public:
    ListWidget_Site(QWebView *webview,
                    QWidget *parent = 0);
    void add_item(const QString &name);
    
signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void ready(QModelIndex);

private slots:
    void log(const QString &text);
    void itemChanged(QListWidgetItem *item);
    void popup(QPoint point);
    void add_site(QAction *action);
    void clicked(QModelIndex index);
    void loadFinished(bool state);

private:
    bool is_loaded;
    QWebView *web;
    void wait(int timeout_msec = 30000);

};
//--------------------------------------------------------------------------------
#endif // ListWidget_Site_HPP
