/*********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#ifndef HELPBROWSER_HPP
#define HELPBROWSER_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QUrl>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class QPushButton;
class QTextBrowser; 
//--------------------------------------------------------------------------------
class HelpBrowser : public QWidget
{ 
    Q_OBJECT
public: 
    HelpBrowser(QString &page,
                QWidget *parent = 0,
                const char *name = 0);

    static void showPage(QString page,
                         bool is_maximized = true);

private slots: 
    void updateCaption(QUrl);

private:
    QTextBrowser *textBrowser = 0;
    QPushButton *homeButton = 0;
    QPushButton *backButton = 0;
    QPushButton *closeButton = 0;

    void setCaption(const QString &caption);
};
//--------------------------------------------------------------------------------
#endif //HELPBROWSER_HPP
