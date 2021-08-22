#ifndef TEST_WIDGET_HPP
#define TEST_WIDGET_HPP
//--------------------------------------------------------------------------------
#include <QGroupBox>
#include <QWidget>
#include <QDebug>
//--------------------------------------------------------------------------------
class TestClass : public QWidget
{
    Q_OBJECT

public:
    TestClass(QWidget *parent = nullptr);
    ~TestClass();

    template<typename T>
    void f_test(const QString &left_oname);

    void f_test2(void);
};
//--------------------------------------------------------------------------------
#endif
