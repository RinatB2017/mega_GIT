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
        in >> str;
        if(str == "VER_MAJOR")
        {
            in >> major;
            cnt++;
        }
        if(str == "VER_MINOR")
        {
            in >> minor;
            cnt++;
        }
        if(str == "VER_BUILD")
        {
            in >> build;
            cnt++;
        }
        if(str == "VER_PATCH")
        {
            in >> patch;
            cnt++;
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
        out << "#ifndef VERSION_HPP" << eol;
        out << "#define VERSION_HPP" << eol;
        out << "//-----" << eol;
        out << "#define VER_MAJOR " << major << eol;
        out << "#define VER_MINOR " << minor << eol;
        out << "#define VER_BUILD " << build << eol;
        out << "#define VER_PATCH " << patch << eol;
        out << "//-----" << eol;
        out << "#endif // VERSION_HPP" << eol;

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
        cerr << "HPP" << std::endl;
        cerr << "Usage: auto_inc_version filename" << std::endl;
        return -1;
    }

    res = readFile(argv[1]);
    if(res < 0) return res;

    cout << major << "." << minor << "." << build << patch << "." << std::endl;

    res = writeFile(argv[1]);
    if(res < 0) return res;

    return 0;
} 
//--------------------------------------------------------------------------------
