#ifndef BUS_H
#define BUS_H

#include <QObject>
#include <QTimer>

namespace iot
{
    class Device;
    class DataNode;
}

class Bus : public QObject
{
    Q_OBJECT
public:
    explicit Bus(iot::Device* device, QObject *parent = 0);
    void setSpeedRange(qreal min, qreal max);
    void setFuelConsumptionRange(qreal min, qreal max);
    void setAccelerationTimeRange(quint32 minTimeInSecs, quint32 maxTimeInSecs);
    void startSimulation();

private slots:

    void generateDataToIotServer();

private:

    void updateTargets();

    iot::Device* m_device;
    iot::DataNode* m_speedDataNode;
    iot::DataNode* m_fuelConsumptionDataNode;
    QTimer m_timer;
    qreal m_targetSpeed;
    qreal m_minSpeed;
    qreal m_maxSpeed;
    qreal m_speedIncrement;
    qreal m_currentSpeed;
    qreal m_currentFuelConsumption;
    qreal m_minFuelConsumption;
    qreal m_maxFuelConsumption;
    quint32 m_accelerationMinTime;
    quint32 m_accelerationMaxTime;
};

#endif // BUS_H
