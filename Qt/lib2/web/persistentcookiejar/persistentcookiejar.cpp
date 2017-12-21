//--------------------------------------------------------------------------------
#include <QNetworkCookie>
#include <QTextStream>
#include <QIODevice>
#include <QDateTime>
#include <QWidget>
#include <QString>
#include <QDebug>
#include <QFile>
//--------------------------------------------------------------------------------
#include "persistentcookiejar.hpp"
//--------------------------------------------------------------------------------
PersistentCookieJar::PersistentCookieJar (QObject *parent, const QString& fname) :
    QNetworkCookieJar (parent)
{
    _fname = fname;
    if(parent)
    {
        connect(this, SIGNAL(info(QString)),  parent, SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parent, SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parent, SIGNAL(error(QString)));
    }
    else
    {
        connect(this, SIGNAL(info(QString)),  this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)), this, SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
PersistentCookieJar::~PersistentCookieJar ()
{

}
//--------------------------------------------------------------------------------
void PersistentCookieJar::log(const QString &text)
{
    qDebug() << text;
}
//--------------------------------------------------------------------------------
void PersistentCookieJar::debug(void)
{
    emit info("Cookies");
    foreach (QNetworkCookie cookie, allCookies ())
    {
        emit info("-----------------------");
        emit info(QString(tr("domain=%1")).arg(cookie.domain()));
        emit info(QString(tr("path=%1")).arg(cookie.path()));
        emit info(QString(tr("expirationDate=%1")).arg(cookie.expirationDate().toString()));
        //emit info(QString(tr("expirationDate=%1")).arg(cookie.expirationDate().toString("ddd, dd-MMM-yyyy hh:mm:ss"))); //Mon, 14-Oct-2013 11:43:56 GMT
        emit info(QString(tr("name=%1")).arg(cookie.name().data()));
        emit info(QString(tr("value=%1")).arg(cookie.value().data()));
        //emit info("-----------------------");
        //emit info(QString(tr("toRawForm=%1")).arg(cookie.toRawForm().data()));
    }
}
//--------------------------------------------------------------------------------
void PersistentCookieJar::save(void)
{
    QFile fout(_fname);
    if (!fout.open (QFile::WriteOnly | QFile::Truncate))
    {
        emit debug(QString(tr("File %1 open error for writing")).arg(_fname));
        return;
    }
    QTextStream out(&fout);

    foreach (QNetworkCookie cookie, allCookies())
    {
        out << cookie.toRawForm () << endl;
    }
    out.flush();
    fout.flush();
    fout.close();
}
//--------------------------------------------------------------------------------
void PersistentCookieJar::load(void)
{
    QFile fin(_fname);
    if (!fin.exists())
    {
        emit error(QString("file %1 not exists").arg(_fname));
        return;
    }

    if (!fin.open(QFile::ReadOnly))
    {
        emit error(QString(tr("File %1 not open for reading")).arg(_fname));
        return;
    }
    QTextStream in (&fin);
    QByteArray buffer = in.readAll().toUtf8();
    //emit debug(QString(tr("buffer: %1")).arg(buffer.constData()));
    setAllCookies(QNetworkCookie::parseCookies(buffer));
    fin.close();
}
//--------------------------------------------------------------------------------
