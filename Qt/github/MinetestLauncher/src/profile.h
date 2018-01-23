#ifndef PROFILE_H
#define PROFILE_H

#include <QString>
#include <QDir>
#include <QJsonObject>
#include <QTabWidget>
#include "processui.h"

using namespace std;

class Profile
{
public:
    Profile(QString, QString);
    static Profile fromJson(QJsonValue);
    QJsonObject toJson();
    QDir getPath();

    QString getName() { return name; }
    QString getVersion() { return version; }

    void play(QTabWidget*);

private:
    void build();
    void start();
    bool created = false;
    QDir prefix = QDir(QDir::home().filePath(".minetest"));
    QString name, version;
};

#endif // PROFILE_H
