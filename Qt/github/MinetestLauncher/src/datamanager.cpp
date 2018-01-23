#include "datamanager.h"

#include <QDebug>

DataManager::DataManager(QString filename)
{
    file = new QFile(filename);
    if (file->exists() && file->size() > 0) {
        file->open(QIODevice::ReadOnly);
        jsonObject = QJsonDocument::fromJson(file->readAll()).object();
        file->close();
    }
    else {
        jsonObject = QJsonObject();
    }
}

void DataManager::loadProfiles() {
    if (jsonObject["instances"].isArray()) {
        QJsonArray profileArray = jsonObject["instances"].toArray();
        qDebug() << profileArray.size() << " profiles to be loaded.";
        for (int i = 0; i < profileArray.size(); i++) {
            manager->addProfile(Profile::fromJson(profileArray.at(i)));
        }
    }
    qDebug() << manager->profiles.size() << "profiles loaded.";
}

void DataManager::saveProfiles()
{
    QJsonArray profileArray = QJsonArray();
    for (Profile profile : manager->profiles) {
        profileArray.append(profile.toJson());
        qDebug() << profile.getName() << "saved.";
    }
    jsonObject["instances"] = profileArray;

    qDebug("Saved profiles");

    save();
}

void DataManager::save()
{
    file->open(QIODevice::WriteOnly | QIODevice::Truncate);
    QJsonDocument document;
    document.setObject(jsonObject);
    file->write(document.toJson());
    file->close();

    qDebug("Saved");
}
