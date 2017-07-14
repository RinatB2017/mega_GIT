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
#ifndef EMULATOR_V786_2_HPP
#define EMULATOR_V786_2_HPP
//--------------------------------------------------------------------------------
#include <QByteArray>
#include <QObject>
//--------------------------------------------------------------------------------
class Emulator_V786_2 : public QObject
{
    Q_OBJECT
public:
    explicit Emulator_V786_2(unsigned char address,
                             unsigned short serno,
                             unsigned short seryear,
                             const QString display,
                             unsigned short voltage,
                             unsigned short current,
                             QObject *parent = 0);

    void set_question(QByteArray question);
    QByteArray get_answer(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

private slots:
    void log(const QString &data);

private:
    QByteArray answer;
    unsigned char address;
    unsigned short serno;
    unsigned short seryear;
    QString display;
    unsigned short voltage;
    unsigned short current;

    void connect_log(void);
    void init(void);

    unsigned short crc16(unsigned char *block, unsigned char len);

};
//--------------------------------------------------------------------------------
#endif // EMULATOR_V786_2_HPP
