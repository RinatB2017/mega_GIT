//--------------------------------------------------------------------------------
#ifndef WEBSETTING_HPP
#define WEBSETTING_HPP
//--------------------------------------------------------------------------------
#include <QToolBar>
//--------------------------------------------------------------------------------
class WebSetting : public QToolBar
{
    Q_OBJECT
public:
    explicit WebSetting(const QString &title, 
                        QWidget *parent = nullptr);
    explicit WebSetting(QWidget *parent = nullptr);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

private slots:
    void slot_checkbox(bool state);

private:
    void init();

};
//--------------------------------------------------------------------------------
#endif // WEBSETTING_HPP
