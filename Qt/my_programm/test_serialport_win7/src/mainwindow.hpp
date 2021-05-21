#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

#include <QSerialPortInfo>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

private:
    Ui::MainWindow *ui;

    QSerialPort *serial = nullptr;

    void read_data(void);

    void serial5_error(QSerialPort::SerialPortError err);

    void get_portname(void);
    void port_open(void);
    void port_close(void);
    void port_send(void);
};
#endif // MAINWINDOW_HPP
