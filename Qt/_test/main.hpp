#ifndef MAIN_HPP
#define MAIN_HPP

#include <QDebug>
//---------------------------------------------------------------------------------------------------
class Main
{
public:

    Main()
    {
        x = 666;
    }

private:
    int x = 0;
};
//---------------------------------------------------------------------------------------------------
class A : virtual public Main
{
public:
    int get_x(void)
    {
        return x;
    }
private:
    int x = 1;
};
//---------------------------------------------------------------------------------------------------
class B : virtual public Main
{
public:
    int get_x(void)
    {
        return x;
    }
private:
    int x = 2;
};
//---------------------------------------------------------------------------------------------------
class C : public A, B
{
public:
    int get_x(void)
    {
        return B::get_x();
    }
};
//---------------------------------------------------------------------------------------------------
class D
{
public:
    D()
    {
        init();
    }

    void init(void)
    {
        qDebug() << "D: init";
    }
};

class E : public D
{
public:
    E()
    {
        init();
    }

    void init(void)
    {
        qDebug() << "E: init";
    }
};
//---------------------------------------------------------------------------------------------------
#endif // MAIN_HPP
