/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
**                                                                              **
**     This program is free software: you can redistribute it and/or modify     **
**     it under the terms of the GNU General Public License as published by     **
**     the Free Software Foundation, either version 3 of the License, or        **
**     (at your option) any later version.                                      **
**                                                                              **
**     This program is distributed in the hope that it will be useful,          **
**     but WITHOUT ANY WARRANTY; without even the implied warranty of           **
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
**     GNU General Public License for more details.                             **
**                                                                              **
**     You should have received a copy of the GNU General Public License        **
**     along with this program.  If not, see http://www.gnu.org/licenses/.      **
**                                                                              **
**********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#include <iostream>
#include <cstdlib>
//--------------------------------------------------------------------------------
#include <QTemporaryFile>
#include <QTextStream>
#include <QString>
#include <QFile>
//--------------------------------------------------------------------------------
using namespace std;
//--------------------------------------------------------------------------------
int major = 0;
int minor = 0;
int build = 0;
int patch = 0;
//--------------------------------------------------------------------------------
int readFile(const QString &filename)
{
    QString str;
    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        cerr << "file " << filename.data()->toAscii() << " not open" << std::endl;
#else
        cerr << "file " << filename.data()->toLatin1() << " not open" << std::endl;
#endif
        return -1;
    }
    int cnt = 0;
    QTextStream in(&file);
    while(!in.atEnd())
    {
        QString line = in.readLine();
        QStringList sl = line.split(" ");
        if(sl.count() == 3)
        {
            if(sl.at(0) == "APP_MAJOR")
            {
                major = sl.at(2).toInt();
                cnt++;
            }
            if(sl.at(0) == "APP_MINOR")
            {
                minor = sl.at(2).toInt();
                cnt++;
            }
            if(sl.at(0) == "APP_BUILD")
            {
                build = sl.at(2).toInt();
                cnt++;
            }
            if(sl.at(0) == "APP_PATCH")
            {
                patch = sl.at(2).toInt();
                cnt++;
            }
        }
        if(cnt >= 4)
        {
            // чтобы не считать данные дважды
            break;
        }
    }
    build++;
    return 0;
}
//--------------------------------------------------------------------------------
int writeFile(const QString &filename)
{
    QTemporaryFile tempFile;
    if (tempFile.open())
    {
        QString eol = "\n";

        QTextStream out(&tempFile);
        out << "APP_MAJOR = " << major << eol;
        out << "APP_MINOR = " << minor << eol;
        out << "APP_BUILD = " << build << eol;
        out << "APP_PATCH = " << patch << eol;
        out << "\n";
        out << "DEFINES += APP_MAJOR=$${APP_MAJOR} " << eol;
        out << "DEFINES += APP_MINOR=$${APP_MINOR} " << eol;
        out << "DEFINES += APP_BUILD=$${APP_BUILD} " << eol;
        out << "DEFINES += APP_PATCH=$${APP_PATCH} " << eol;

        const QString tempFileName = tempFile.fileName();
        tempFile.close();

        QFile::remove(filename);
        QFile::copy(tempFileName, filename);
    }
    else
    {
        cerr << "Error creating temporary file!" << std::endl;
        return -1;
    }
    return 0;
}
//--------------------------------------------------------------------------------
int main(int argc, char *argv[])
{ 
    int res;

    if (argc != 2)
    {
        cerr << "PRI" << std::endl;
        cerr << "Usage: auto_inc_version filename" << std::endl;
        return -1;
    }

    cout << "read file " << argv[1] << std::endl;
    res = readFile(argv[1]);
    if(res < 0) return res;

    cout << major << "." << minor << "." << build << "." << patch << "." << std::endl;

    cout << "write file " << argv[1] << std::endl;
    res = writeFile(argv[1]);
    if(res < 0) return res;

    return 0;
} 
//--------------------------------------------------------------------------------
