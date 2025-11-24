//--------------------------------------------------------------------------------
#ifndef ABOUTBOX_HPP
#define ABOUTBOX_HPP
//--------------------------------------------------------------------------------
#include <QProcessEnvironment>
#include <QDesktopServices>
#include <QTextEdit>
#include <QDialog>
#include <QStyle>
#include <QTimer>
#include <QUrl>
//--------------------------------------------------------------------------------
#define P_AVATAR        "avatar"
#define P_TELEGRAM_QR   "telegram_qr"
#define P_ORGNAME       "orgName"
#define P_PROGRAMMNAME  "programmName"
#define P_VERSION       "version"
#define P_EMAIL         "email"
#define P_AUTHOR        "author"
#define P_TELEGRAM      "telegram"
#define P_GITHUB        "github"
//--------------------------------------------------------------------------------
namespace Ui {
    class AboutBox;
}
//--------------------------------------------------------------------------------
class AboutBox : public QDialog
{
    Q_OBJECT

    // не забыть, что название property должны совпадать с первым полем Q_PROPERTY
    Q_PROPERTY(QString avatar       READ get_avatar         WRITE set_avatar)
    Q_PROPERTY(QString telegram_qr  READ get_telegram_qr    WRITE set_telegram_qr)
    Q_PROPERTY(QString orgName      READ get_orgName        WRITE set_orgName)
    Q_PROPERTY(QString programmName READ get_programmName   WRITE set_programmName)
    Q_PROPERTY(QString version      READ get_version        WRITE set_version)
    Q_PROPERTY(QString email        READ get_email          WRITE set_email)
    Q_PROPERTY(QString author       READ get_author         WRITE set_author)
    Q_PROPERTY(QString telegram     READ get_telegram       WRITE set_telegram)
    Q_PROPERTY(QString github       READ get_github         WRITE set_github)

public:
    explicit AboutBox(QWidget *parent = nullptr);
    virtual ~AboutBox();

private slots:
    void send_mail(QString link);
    void go_telegramm(QString link);
    void show_env(void);

private:
    Ui::AboutBox *ui;
    QString avatar;
    QString telegram_qr;
    QString orgName;
    QString programmName;
    QString version;
    QString email;
    QString author;
    QString telegram;
    QString github;

    QString get_avatar(void);
    QString get_telegram_qr(void);
    QString get_orgName(void);
    QString get_programmName(void);
    QString get_version(void);
    QString get_email(void);
    QString get_author(void);
    QString get_telegram(void);
    QString get_github(void);

    void set_avatar(const QString &value);
    void set_telegram_qr(const QString &value);
    void set_orgName(const QString &value);
    void set_programmName(const QString &value);
    void set_version(const QString &value);
    void set_email(const QString &value);
    void set_author(const QString &value);
    void set_telegram(const QString &value);
    void set_github(const QString &value);

    void init(void);

};
//--------------------------------------------------------------------------------
#endif // ABOUTBOX_HPP
