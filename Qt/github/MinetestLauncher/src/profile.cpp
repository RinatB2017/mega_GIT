#include "profile.h"

Profile::Profile(QString n, QString v)
{
    name = n;
    version = v;
}

Profile Profile::fromJson(QJsonValue val)
{
    QJsonObject obj = val.toObject();
    return Profile(obj.value("name").toString(), obj.value("version").toString());
}

QJsonObject Profile::toJson()
{
    QJsonObject object;
    object.insert("name", name);
    object.insert("version", version);
    return object;
}

QDir Profile::getPath()
{
    return QDir(prefix.filePath(name));
}

void Profile::play(QTabWidget *widget)
{
    //QString cmd = "/bin/bash -c \"/home/myself/Code/C++/MinetestLauncher/scripts/linux.sh create %1 %2\"";
    QString cmd = "/bin/bash -c \"scripts/linux.sh create %1 %2\"";
    ProcessUi* process = new ProcessUi(widget, cmd.arg(name, version));
    widget->addTab(process, name);
    process->start();
    widget->setCurrentIndex(widget->indexOf(process));
}

void Profile::build()
{
#ifdef _WIN32
#else
#endif
}

void Profile::start()
{

}
