#ifndef TEST_WIDGET_HPP
#define TEST_WIDGET_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QDebug>
//--------------------------------------------------------------------------------
class TestClass : public QWidget
{
    Q_OBJECT

public:
    explicit TestClass(QWidget *parent = nullptr);
    virtual ~TestClass();

    template<typename T>
    void f_test(const QString &left_oname);
};
//--------------------------------------------------------------------------------
#endif
