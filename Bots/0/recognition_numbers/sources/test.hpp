#ifndef TEST_HPP
#define TEST_HPP

#include <QApplication>

class MyApplication Q_DECL_FINAL : public QApplication
{
    Q_OBJECT

public:
    MyApplication(int &argc, char **argv);
    bool notify(QObject* receiver, QEvent* event) Q_DECL_OVERRIDE;
};

#endif
