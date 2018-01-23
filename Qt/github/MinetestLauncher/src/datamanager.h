#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "profilemanager.h"

#include <QString>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

const QJsonObject DEFAULT_OBJECT
{
    {"instances", QJsonArray()}
};
//QString DEFAULT_PATH = QDir::homePath() + "/.minetest/minetestlauncher.conf";

class DataManager
{
public:
    DataManager(QString);
    inline void setManager(ProfileManager *mgr) { manager = mgr; }
    void loadProfiles();
    void saveProfiles();
    void save();

private:
    ProfileManager *manager;
    QJsonObject jsonObject;
    QFile *file;
};

#endif // DATAMANAGER_H
