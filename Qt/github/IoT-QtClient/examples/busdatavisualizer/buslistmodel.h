#ifndef BUSLISTMODEL_H
#define BUSLISTMODEL_H

#include <devicelist.h>

#include <QAbstractListModel>
#include <QDebug>

class Bus;

class BusListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    BusListModel(QObject* parent = 0);
    virtual ~BusListModel();

    QHash<int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;

    Q_INVOKABLE void refresh();

protected:

    QList<Bus*> m_busses;
    QHash<int, QByteArray> m_roles;
    iot::DeviceList m_deviceList;
};


#endif // BUSLISTMODEL_H
