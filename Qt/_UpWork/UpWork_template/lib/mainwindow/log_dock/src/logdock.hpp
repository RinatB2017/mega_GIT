/*********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#ifndef LOGDOCK_HPP
#define LOGDOCK_HPP
//--------------------------------------------------------------------------------
#include <QDockWidget>
//--------------------------------------------------------------------------------
class LogBox;
//--------------------------------------------------------------------------------
class LogDock : public QDockWidget
{
    Q_OBJECT
public:
    explicit LogDock(const QString &title,
                     QWidget *parent = 0);

    void load_settings(void);
    void save_settings(void);

    void load_html(void);
    void save_html(void);

    void  set_font(QFont font);
    QFont get_font(void);

public slots:
    void infoLog(const QString &text);
    void debugLog(const QString &text);
    void errorLog(const QString &text);
    void messageLog(const QString &text);

    void syslog(int level,
                const QString &src,
                const QString &message);

private:
    LogBox *le = 0;
};
//--------------------------------------------------------------------------------
#endif // LOGDOCK_HPP
