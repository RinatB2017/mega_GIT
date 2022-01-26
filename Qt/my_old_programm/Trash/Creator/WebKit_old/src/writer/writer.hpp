//--------------------------------------------------------------------------------
#ifndef WRITER_HPP
#define WRITER_HPP
//--------------------------------------------------------------------------------
#include <QWebView>
//--------------------------------------------------------------------------------
class Writer : public QWebView
{
    Q_OBJECT
public:
    explicit Writer(QWidget *parent = nullptr);
    
signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

public slots:
    
};
//--------------------------------------------------------------------------------
#endif // WRITER_HPP
