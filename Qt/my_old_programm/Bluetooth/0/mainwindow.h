#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
// necessary for bluetooth
#include <QBluetoothLocalDevice>
#include <QBluetoothAddress>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothServer>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void deviceDiscovered(QBluetoothDeviceInfo);

private slots:

    void socketConnected();
    void dataReady2Read();
    void on_toolButton_clicked();

private:
    Ui::MainWindow *ui;
    QBluetoothLocalDevice localDevice;                  // the local interface
    QString localDeviceName;
    QList<QBluetoothAddress> remotes;
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QBluetoothSocket *socket;                           // a socket for communicate
};

#endif // MAINWINDOW_H
