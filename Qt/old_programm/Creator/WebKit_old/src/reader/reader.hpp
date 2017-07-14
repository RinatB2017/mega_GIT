//--------------------------------------------------------------------------------
#ifndef READER_HPP
#define READER_HPP
//--------------------------------------------------------------------------------
#include <QWebView>
#include <QUrl>
//--------------------------------------------------------------------------------
class Reader : public QWebView
{
    Q_OBJECT
public:
    explicit Reader(QWidget *parent = 0);

    void set_main_url(const QUrl &url);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

public slots:
    void run(void);

private:
    QUrl main_url;
    bool is_loaded;

};
//--------------------------------------------------------------------------------
#endif // READER_HPP
