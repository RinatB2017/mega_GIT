#ifndef D3VIEWER_H
#define D3VIEWER_H

#include <QWidget>
#include <QVariant>

class QWebView;
class QWebPage;
class QWebFrame;

class QResizeEvent;

class D3WebPage;

namespace d3widgets {

class D3Viewer : public QWidget
{
    Q_OBJECT
public:
    explicit D3Viewer(QWidget *parent = 0);
    virtual ~D3Viewer();

    Q_PROPERTY(int width READ width) //widget width
    Q_PROPERTY(int height READ height) //widget height

    void setScrollBarPolicy ( Qt::Orientation orientation, Qt::ScrollBarPolicy policy ); //helper
    void setScrollBarsVisible( const bool isVisible); //set scroll bars visibility
    void setBackgroundBrush(const QBrush &brush); //set background to brush

    void addContextObject(const QString &name, QObject *object); //set context obj to frame()

    void load(const QString &url); //load page to view() and save url to d3Url
    void reload();                //reload d3Url
    void evaluateScript(const QString &script);

public slots:
    int width();  //see Q_PROPERTY
    int height(); //see Q_PROPERTY

    void printObject(QVariant object); //debug helper function (call it from JS)

signals:
    void sizeChanged(const QSize &size); //emitted in resizeEvent
    void loadFinished(const bool ok); //on page() loadFinished

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QWebView *d3WebView;
    D3WebPage *d3WebPage;
    QString d3Url;

    QWebView  *view(); //helper function d3WebView
    QWebPage  *page(); //helper funciton view()->page();
    QWebFrame *frame();//helper funciton page()->currentFrame();

};  //class D3Viewer
}  //namespace d3widgets

#endif // D3VIEWER_H
