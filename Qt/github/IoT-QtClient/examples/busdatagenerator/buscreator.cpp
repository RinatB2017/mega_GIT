#include "buscreator.h"
#include "bus.h"

#include <error.h>
#include <device.h>

#include <QDebug>

BusCreator::BusCreator(int busCount, QObject* parent) : QObject(parent)
{
    // there is a limit for devices on the free iotticket account
    m_busCount = qMin(busCount, 5);
    connect(&m_deviceList, &iot::DeviceList::getFinished, this, &BusCreator::onDeviceListFetched);
//    create();
    //    m_deviceList.get();
}

BusCreator::~BusCreator()
{
    qDebug() << "destruct";
}

bool BusCreator::create()
{
    if (m_busDevices.count() > 0) {
        qDebug() << "Devices already created";
        return false;
    }

    m_deviceList.get();
    return true;
}

void BusCreator::onDeviceListFetched(bool success)
{
    if (success) {
        // Create busses if those have not yet been created.
        m_busDevices = m_deviceList.devices();
        qDebug() << "Bus count: " << m_busDevices.count();
        if (m_busCount == m_busDevices.count()) {
            qDebug() << "All bus devices already created";
            createBusses();
        } else {
            int busCount = m_busDevices.count();
            while (m_busCount > busCount) {
                qDebug() << "create device";
                iot::Device* device = new iot::Device();
                device->setManufacturer("TKL");
                device->setDescription("TKL bus");
                device->setType("Bus");
                busCount++;
                device->setName(QString("Bus%1").arg(busCount));
                connect(device, &iot::Device::registerFinished, [=](bool success) {
                    if (success) {
                        qDebug() << "Bus created, name: " << device->name();
                        m_busDevices << device;
                        if (m_busDevices.count() == m_busCount) {
                            qDebug() << "all bus decices created";
                            createBusses();
                        }
                    } else {
                        qDebug() << "device registration failed";
                    }
                });
                device->registerDevice();
            }
        }
    } else {
        qDebug() << "Error fetching devices";
        qDebug() << (int)m_deviceList.getError()->errorType();
    }
}

void BusCreator::createBusses()
{
    foreach(iot::Device* device, m_busDevices) {
        Bus* bus = new Bus(device);
        bus->startSimulation();
        m_busses << bus;
    }
}
