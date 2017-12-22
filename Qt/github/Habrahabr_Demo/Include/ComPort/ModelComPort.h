#ifndef MODEL_H
#define MODEL_H

#include <QtCore/qglobal.h>
#include <QThread>
#include <QIcon>
#include <QString>
#include <QStringList>
#include <QSerialPort>
#include <QByteArray>

#include "ComPortThread.h"

class ModelComPort
{
public:
    ModelComPort();
    ~ModelComPort();

    // Соединение с COM-портом
    void connectToComPort();

    // Наименование порта
    void setPortName(QString portName);
    QString getPortName() const;

    // Скорость порта
    void setBaudrate(int baudrate);
    int getBaudrate() const;

    // Получение списка COM-портов
    QList<QString> getListNamePorts() const;

    // Получение состояния порта
    bool isConnect() const;

    // Запись в COM-порт
    void onCommand(QString command);

    // Прием ответа из COM-порта
    void response(QByteArray msg);

private:
    // Поиск существующих COM-портов в системе
    void searchComPorts();
    // Отправка команды на контроллер
    void sendCommand(int command);
    // Разбор принятой команды
    void evaluateCommand(quint8 cmd);

private:
    bool m_connected;               // Есть ли соединение с COM-портом
    QString m_portName;             // Имя COM-порта
    QList<QString> m_listPorts;     // Список COM-портов в системе
    int m_baudrate;
    int m_dataBits;
    int m_parity;
    int m_stopBits;
    int m_flowControl;

    QByteArray m_inBuf;

    ComPortThread* thread;
};

#endif // MODEL_H
