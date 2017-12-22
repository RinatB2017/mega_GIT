#include "buslistmodel.h"
#include "bus.h"

namespace
{
    const int OBJECT_ROLE = Qt::UserRole + 1;
    const QByteArray OBJECT_NAME = "bus";
}

BusListModel::BusListModel(QObject *parent) : QAbstractListModel(parent)
{
    m_roles[OBJECT_ROLE] = OBJECT_NAME;
    connect(&m_deviceList, &iot::DeviceList::getFinished, [this](bool success) {
        beginResetModel();
        if (success) {
            QList<iot::Device*> devices = m_deviceList.devices();
            qDebug() << "Got the device list, device count " << devices.count();
            foreach(iot::Device* device, devices) {
                // Parenting prevents scriptengine from deleting the objects on the QML side
                m_busses << new Bus(device, this);
            }
        } else {
            qDebug() << "Getting device list failed";
        }
        endResetModel();
    });
}

BusListModel::~BusListModel()
{
}

QHash<int, QByteArray> BusListModel::roleNames() const
{
    return m_roles;
}


QVariant BusListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_busses.count() || role != Qt::UserRole + 1) return QVariant();

    return QVariant(QMetaType::QObjectStar, &m_busses[index.row()]);
}


int BusListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_busses.count();
}

void BusListModel::refresh()
{
    m_deviceList.get();
}
