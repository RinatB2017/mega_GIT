//--------------------------------------------------------------------------------
#include <QAbstractSocket>
#include <QTcpSocket>
//--------------------------------------------------------------------------------
#include "test_thread.hpp"
//--------------------------------------------------------------------------------
TestThread::TestThread(QObject* parent) :
    receiver(parent)
{
    stopped = false;
    sleep_interval = 1;
}
//--------------------------------------------------------------------------------
bool TestThread::init_proto(const QString addr, unsigned int port)
{
    this->address = QHostAddress(addr);
    this->port = port;
    socket = new QTcpSocket();
    proto = new Proto(socket);
    connectToHost();
    if (!socket->waitForConnected(10000))
    {
        print("Ошибка соединения", -1);
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
void TestThread::delete_proto()
{
    delete proto;
    delete socket;
}
//--------------------------------------------------------------------------------
void TestThread::stop()
{
    mutex.lock();
    stopped = true;
    mutex.unlock();
}
//--------------------------------------------------------------------------------
void TestThread::connectToHost()
{
    //    qDebug() << "connectToHost: address " << address.toString() << " port " << port;
    socket->connectToHost(address.toString(), port);
}
//--------------------------------------------------------------------------------
void TestThread::setSleepInterval(int interval)
{
    if (interval < 0) return;
    mutex.lock();
    sleep_interval = interval;
    mutex.unlock();
}
//--------------------------------------------------------------------------------
void TestThread::setConnectSettings(const QHostAddress& addr, int p)
{
    address = addr;
    port = p;
}
//--------------------------------------------------------------------------------
void TestThread::setSerialDescriptor(int d)
{
    descriptor = d;
}
//--------------------------------------------------------------------------------
void TestThread::OnSocketConnected()
{
    //print("Соединение установлено");
}
//--------------------------------------------------------------------------------
void TestThread::OnSocketDisconnected()
{
    //print("Соединение разорвано");
}
//--------------------------------------------------------------------------------
void TestThread::OnSocketError(QAbstractSocket::SocketError)
{
    //qDebug() << err;
}
//--------------------------------------------------------------------------------
void TestThread::OnStateChanged(QAbstractSocket::SocketState )
{
    //qDebug() << state;
    //print(stateString(state));
}
//--------------------------------------------------------------------------------
QString TestThread::stateString(QAbstractSocket::SocketState id_state)
{
    QString state;
    switch (id_state)
    {
    case QAbstractSocket::UnconnectedState: state = "Нет соединения"; break;
    case QAbstractSocket::HostLookupState: state = "Выполняется поиск сервера..."; break;
    case QAbstractSocket::ConnectingState: state = "Устанавливается соединение"; break;
    case QAbstractSocket::ConnectedState: state = "Соединение установлено"; break;
    case QAbstractSocket::BoundState: state = "Привязка к адресу и порту выполнена"; break;
    case QAbstractSocket::ClosingState: state = "Соединение закрыто (или в процессе...)"; break;
    case QAbstractSocket::ListeningState: state = "Только локальное использование"; break;
    default: state = "Состояние неопределено"; break;
    }
    return state;
}
//--------------------------------------------------------------------------------
void TestThread::print(const QString& text, int type, bool bold)
{
    QApplication::postEvent(receiver, new InfoEvent(text, type, bold));
}
//--------------------------------------------------------------------------------
void TestThread::print_serial(const QString& text)
{
    QApplication::postEvent(receiver, new SerialInfoEvent(text));
}
//--------------------------------------------------------------------------------
