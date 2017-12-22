#include "ComPortThread.h"

#include <QSerialPort>
#include <QTime>
#include <QDebug>

ComPortThread* ComPortThread::m_instance = 0;
int ComPortThread::m_refCount = 0;

ComPortThread::ComPortThread(QObject *parent)
    : QThread(parent),
      m_waitTimeout(0),
      m_isQuit(false),
      m_isConnect(false),
      m_isDisconnecting(false),
      m_isConnecting(false)
{
    QMutexLocker locker(&mutex);
}

ComPortThread::~ComPortThread()
{
    mutex.lock();
    m_isQuit = true;
    m_isConnect = false;
    m_instance = 0;
    cond.wakeOne();
    qDebug("~ComPortThread()");
    mutex.unlock();
    wait();
}

void ComPortThread::connectCom(QString namePort, int baudRate, int dataBits, int parity, int stopBits, int flowControl)
{
    m_portName = namePort;
    m_baudrate = baudRate;
    m_dataBits = dataBits;
    m_parity = parity;
    m_stopBits = stopBits;
    m_flowControl = flowControl;

    // Если поток не запущен - запускаем его
    if (!isRunning())
    {
        m_isConnecting = true;
        start();
        m_isQuit = false;
    }
    else
    {
        // Если поток запущен, будим его
        cond.wakeOne();
    }
}

void ComPortThread::disconnectCom()
{
    mutex.lock();
    m_isDisconnecting = true;
    mutex.unlock();
    cond.wakeOne();
}

// Отправка сообщения в COM-порт
void ComPortThread::transaction(const QByteArray& request, int waitTimeout)
{
    if (m_isConnect)
    {
        m_request = request;
        m_waitTimeout = waitTimeout;

        cond.wakeOne();
    }
}

// Работа потока
void ComPortThread::run()
{
    QSerialPort serial;

    // Блокируем поток для инициализации переменных потока
    mutex.lock();
    // Имя текущего COM-порта
    QString currentPortName = m_portName;

    // Время ожидания ответа
    int currentWaitTimeout = m_waitTimeout;

    // Информация, отправляемая в COM-порт
    QByteArray currentRequest = m_request;
    mutex.unlock();

    while (!m_isQuit)
    {
        // Если было изменение имени COM-порта
        if (m_isConnecting)
        {
            // Устанавливаем имя COM-порта
            serial.setPortName(currentPortName);

            // Открываем COM-порт
            if (serial.open(QIODevice::ReadWrite))
            {
                // Выставляем настройки
                if ((serial.setBaudRate(m_baudrate)
                        && serial.setDataBits((QSerialPort::DataBits)m_dataBits)
                        && serial.setParity((QSerialPort::Parity)m_parity)
                        && serial.setStopBits((QSerialPort::StopBits)m_stopBits)
                        && serial.setFlowControl((QSerialPort::FlowControl)m_flowControl)))
                {
                    m_isConnect = true;
                    m_isConnecting = false;
                }
                else
                {
                    m_isConnect = false;
                    m_isConnecting = false;

                    emit error(tr("Can't open %1, error code %2")
                               .arg(m_portName)
                               .arg(serial.error()));
                    return;
                }
            }
            else
            {
                m_isConnect = false;
                m_isConnecting = false;

                emit error(tr("Can't open %1, error code %2")
                           .arg(m_portName)
                           .arg(serial.error()));
                return;
            }
        }
        else if (m_isDisconnecting)
        {
            serial.close();
            m_isDisconnecting = false;
            m_request.clear();
            m_isQuit = true;
        }
        else
        {
            // Отправляем в COM-порт команду
            if (!currentRequest.isEmpty())
            {
                serial.write(currentRequest);

                // Даем время на отправку
                if (serial.waitForBytesWritten(m_waitTimeout))
                {
                    // Даем время на получение ответа
                    if (serial.waitForReadyRead(currentWaitTimeout))
                    {
                        // Читаем ответ
                        QByteArray responseFromPort = serial.readAll();

                        while (serial.waitForReadyRead(10))
                        {
                            responseFromPort += serial.readAll();
                        }

                        // Отправляем сигнал о том, что ответ получен
                        emit responseMsg(responseFromPort);
                    }
                    else
                    {
                        // Ошибка по таймауту ожидания ответа
                        emit timeout(tr("Wait read response timeout %1")
                                     .arg(QTime::currentTime().toString()));
                    }
                }
                else
                {
                    // Ошибка по таймауту ожидания передачи запроса
                    emit timeout(tr("Wait write request timeout %1")
                                 .arg(QTime::currentTime().toString()));
                }

                // Очищаем текущую команду
                currentRequest.clear();
            }
            else
            {
                mutex.lock();
                // Засыпаем до следующей отправки
                cond.wait(&mutex);
                currentWaitTimeout = m_waitTimeout;
                currentRequest = m_request;
                mutex.unlock();
            }
        }
    }
}

bool ComPortThread::isConnect()
{
    return m_isConnect;
}

void ComPortThread::free()
{
    if (--m_refCount == 0)
    {
        delete this;
    }
}
