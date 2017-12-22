#ifndef IVIEW
#define IVIEW

#include "ModelComPort.h"

class IViewComPort
{
public:
    // Имя текущего COM-порта
    virtual QString getPortName() const = 0;
    virtual void setPortName(QString portName) = 0;
    virtual void addPortName(QString portName) = 0;
    // Текущая скорость
    virtual int getBaudrate() const = 0;
    virtual void setBaudrate(int baudrate) = 0;
    virtual void addBaudrate(int baudrate) = 0;

    // Включение/выключение светодиода
    virtual void ledOn(bool on) = 0;

    // Блокировка comboBox
    virtual void setEnablePortNameBox(bool enable) = 0;
    virtual void setEnableBaudrateBox(bool enable) = 0;

    // Изменение надписи на кнопке
    virtual void setTitleButton(QString) = 0;

    // signals
public:
    // Подключение к Com-порту
    virtual void processConnect() = 0;
    // Изменение имени Com-порта
    virtual void processNameComPortChanged(QString portName) = 0;
    // Изменение скорости Com-порта
    virtual void processBaudratePortChanged(int baudrate) = 0;
    // Отправка данных в COM-порт
    virtual void onCommand(QString command) = 0;
};

#endif // IVIEW
