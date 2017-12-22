#ifndef BUSCREATOR_H
#define BUSCREATOR_H

#include <QObject>

#include <devicelist.h>

class Bus;

class BusCreator : public QObject
{
    Q_OBJECT
public:
    BusCreator(int busCount, QObject* parent = 0);
    virtual ~BusCreator();

    bool create();

private slots:

    void onDeviceListFetched(bool success);

private:

    void createBusses();

    iot::DeviceList m_deviceList;
    int m_busCount;
    QList<iot::Device*> m_busDevices;
    QList<Bus*> m_busses;
};

#endif // BUSCREATOR_H
