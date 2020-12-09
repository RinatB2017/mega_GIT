#include "base/d3viewer.h"

#include <QWebView>
#include <QVBoxLayout>
#include <QWebPage>
#include <QWebFrame>
#include <QDebug>

#include "d3webpage.h"

namespace d3widgets {

D3Viewer::D3Viewer(QWidget *parent) :
    QWidget(parent),
    d3WebView(NULL),
    d3WebPage(NULL)
{
    setScrollBarsVisible(false);

    connect(page(), SIGNAL(loadFinished(bool)), this , SIGNAL(loadFinished(bool)));

    setBackgroundBrush(Qt::transparent);
}

D3Viewer::~D3Viewer()
{

}

QWebView *D3Viewer::view()
{
    if (NULL == d3WebView)
    {
        d3WebView = new QWebView(this);
        d3WebView->settings()->setDefaultTextEncoding("utf-8");
        d3WebPage = new D3WebPage(this);
        d3WebView->setPage(d3WebPage);
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(d3WebView);
        mainLayout->setMargin(0);
    }
    return d3WebView;
}
QWebPage *D3Viewer::page()
{
    return view()->page();
}

QWebFrame *D3Viewer::frame()
{
    return page()->currentFrame();
}

int D3Viewer::width()
{
    return this->size().width(); //.width();
}

int D3Viewer::height()
{
    return this->size().height();
}

void D3Viewer::setScrollBarPolicy(Qt::Orientation orientation, Qt::ScrollBarPolicy policy)
{
    frame()->setScrollBarPolicy(orientation, policy);
}

void D3Viewer::setScrollBarsVisible(const bool isVisible)
{
    Qt::ScrollBarPolicy policy;
    isVisible ? policy = Qt::ScrollBarAlwaysOn : policy = Qt::ScrollBarAlwaysOff;
    setScrollBarPolicy(Qt::Vertical, policy);
    setScrollBarPolicy(Qt::Horizontal, policy);
}

void D3Viewer::addContextObject(const QString &name, QObject *object)
{
    frame()->addToJavaScriptWindowObject( name, object );
}

void D3Viewer::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    emit sizeChanged( frame()->contentsSize() );
}

void D3Viewer::load(const QString &url)
{
    //TODO: check if exist
    d3Url = url;
    view()->load(QUrl(url));
}

void D3Viewer::reload()
{
    if (d3Url.isEmpty())
    {
        qWarning() << "D3Viewer url is empty";
        return;
    }
    view()->load(d3Url);
}

void D3Viewer::evaluateScript(const QString &script)
{
    frame()->evaluateJavaScript(script);
}

void D3Viewer::setBackgroundBrush(const QBrush &brush)
{
    QPalette palette = view()->palette();
    palette.setBrush(QPalette::Base, brush);
    page()->setPalette(palette);
    view()->setAttribute(Qt::WA_OpaquePaintEvent, false);
}

void D3Viewer::printObject(QVariant object)
{
    qDebug() << "JS object: " <<  object;
}

}
