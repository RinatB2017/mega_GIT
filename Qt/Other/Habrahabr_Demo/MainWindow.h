#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/*
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMainWindow>
#else
#include <QtGui/QMainWindow>
#endif
*/

#include <QMainWindow>

#include "IViewComPort.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public IViewComPort
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // Имя текущего COM-порта
    virtual QString getPortName() const;
    virtual void setPortName(QString portName);
    virtual void addPortName(QString portName);
    // Текущая скорость
    virtual int getBaudrate() const;
    virtual void setBaudrate(int baudrate);
    virtual void addBaudrate(int baudrate);

    // Включение/выключение светодиода
    virtual void ledOn(bool on);

    // Блокировка comboBox
    virtual void setEnablePortNameBox(bool enable);
    virtual void setEnableBaudrateBox(bool enable);

    // Изменение надписи на кнопке
    virtual void setTitleButton(QString title);

private slots:
    void checkCustomBaudRatePolicy(int idx);
    void checkCustomPortName(int idx);

signals:
    // Подключение к Com-порту
    void processConnect();
    // Изменение имени Com-порта
    void processNameComPortChanged(QString portName);
    // Изменение скорости Com-порта
    void processBaudratePortChanged(int baudrate);
    // Отправка команды в COM-порт
    void onCommand(QString cmd);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
