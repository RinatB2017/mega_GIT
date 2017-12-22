#ifndef BUS_H
#define BUS_H

#include <QObject>
#include <QTimer>
#include <QVariant>
#include <QDateTime>

namespace iot
{
    class Device;
    class DataNode;
}

class Bus : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal speed READ speed NOTIFY speedChanged)
    Q_PROPERTY(qreal fuelConsumption READ fuelConsumption NOTIFY fuelConsumptionChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
public:
    explicit Bus(iot::Device* device, QObject *parent = 0);
    explicit Bus(QObject *parent = 0);
    virtual ~Bus();

    qreal speed() const;
    qreal fuelConsumption() const;
    QString name() const;

    Q_INVOKABLE void startReading();
    Q_INVOKABLE void stopReading();

signals:

    void speedChanged(qreal speed);
    void fuelConsumptionChanged(qreal consumption);
    void nameChanged(const QString& name);

private:

    void setFuelConsumption(const QPair<QVariant, QDateTime>& val);
    void setSpeed(const QPair<QVariant, QDateTime>& val);

    iot::Device* m_device;
    QPair<QVariant, QDateTime> m_speed;
    QPair<QVariant, QDateTime> m_fuelConsumption;
    QTimer m_timer;
};

#endif // BUS_H
