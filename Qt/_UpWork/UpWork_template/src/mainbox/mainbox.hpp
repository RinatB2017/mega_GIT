/*********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class QToolButton;
class QToolBar;
class QComboBox;
class QCheckBox;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    virtual ~MainBox();

private slots:
    void choice_test(void);
    bool test_0(void);
    bool test_1(void);
    bool test_2(void);
    bool test_3(void);
    bool test_4(void);
    bool test_5(void);

private:
    enum {
        ID_TEST_0 = 1000,
        ID_TEST_1,
        ID_TEST_2,
        ID_TEST_3,
        ID_TEST_4,
        ID_TEST_5,
        ID_TEST_6
    };
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)(void);
    } CMD_t;

    MySplashScreen *splash = 0;
    Ui::MainBox *ui = 0;

    QComboBox *cb_test = 0;
    QCheckBox *cb_block = 0;
    QList<CMD> commands;

    void init(void);

    QToolButton *add_button(QToolBar *tool_bar,
                            QToolButton *tool_button,
                            QIcon icon,
                            const QString &text,
                            const QString &tool_tip);

    void createTestBar(void);

protected:
    virtual void changeEvent(QEvent *event);
    virtual bool eventFilter(QObject*, QEvent* event);
    void paintEvent(QPaintEvent *);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
