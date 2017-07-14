//--------------------------------------------------------------------------------
#ifndef PERSISTENTCOOKIEJAR_H
#define PERSISTENTCOOKIEJAR_H
//--------------------------------------------------------------------------------
#include <QNetworkCookieJar>
#include <QObject>
#include <QString>
//--------------------------------------------------------------------------------
class PersistentCookieJar : public QNetworkCookieJar
{
    Q_OBJECT
protected:
    QString _fname;

public:
    explicit PersistentCookieJar(QObject *parent, const QString& fname);
    ~PersistentCookieJar ();

    void save(void);
    void load(void);
    void debug(void);

private slots:
    void log(const QString &text);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

public slots:

};
//--------------------------------------------------------------------------------

#endif // PERSISTENTCOOKIEJAR_H
