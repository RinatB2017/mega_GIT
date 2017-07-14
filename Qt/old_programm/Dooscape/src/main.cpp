#include "mainwindow.h"
#include "screenshoot.h"
#include "starteruser.h"

#include <QApplication>

/*
    This file is part of Dooscape.

    Dooscape is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Dooscape is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Dooscape.  If not, see <http://www.gnu.org/licenses/>.
*/

static void associateFileTypes(const QStringList &fileTypes)
{
    QString displayName = QGuiApplication::applicationDisplayName();
    QString filePath = QCoreApplication::applicationFilePath();
    QString fileName = QFileInfo(filePath).fileName();

    QSettings settings("HKEY_CURRENT_USER\\Software\\Classes\\Applications\\" + fileName, QSettings::NativeFormat);
    settings.setValue("FriendlyAppName", displayName);

    settings.beginGroup("SupportedTypes");
    foreach (const QString& fileType, fileTypes)
        settings.setValue(fileType, QString());
    settings.endGroup();

    settings.beginGroup("shell");
    settings.beginGroup("open");
    settings.setValue("FriendlyAppName", displayName);
    settings.beginGroup("Command");
    settings.setValue(".", QChar('"') + QDir::toNativeSeparators(filePath) + QString("\" \"%1\""));
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("Dooscape");
    a.setApplicationVersion("1.2");
    a.setOrganizationName("Doocode");
    a.setOrganizationDomain("http://doocode.esy.es/");
    a.setApplicationDisplayName(QCoreApplication::applicationName()+" "+QCoreApplication::applicationVersion());
    a.setWindowIcon(QIcon(":/res/icon.ico"));

    /* ScreenShoot screen;
    screen.show();*/

    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QCoreApplication::applicationDirPath() + QDir::separator() + QString("dooscape_") + locale);
    QCoreApplication::installTranslator(&translator);

#ifdef Q_OS_WIN
    QStringList canOpenFile;
    canOpenFile << ".html" << ".xhtlm" << ".htm" << ".shtm" << ".txt" << ".bmp" << ".gif" << ".jpeg" << ".jpg" << ".png";
    associateFileTypes(canOpenFile);
#endif

    /*// Création de la zone de mémoire partagé identifiée par une chaîne de caractères. On utilisera un GUID pour avoir un identifiant unique pour qu'il n'y ait pas de conflit possible avec d'autres applications
    QSharedMemory sharedMemory("{69B55631-A712-4d8c-8852-A8ED297EF439}");
    if(sharedMemory.create(sizeof(int))==false) // On vérifie à la création de cette zone mémoire si celle-ci existe
    {
        // Si le navigateur est ouvert
    }*/

    MainWindow *w = new MainWindow;
    StarterUser *s = new StarterUser;
    s->setMainWindow(w);

    QSettings config;
    if(config.value("Version")<QCoreApplication::applicationVersion())
    {
        w->hide();

        s->start();
    }
    else
        w->loadStartup();

    const QStringList args = QCoreApplication::arguments();
    if(args.count() > 1)
    {
        QString fileName = args[1];
        w->loadFile(fileName);
    }

    return a.exec();
}
