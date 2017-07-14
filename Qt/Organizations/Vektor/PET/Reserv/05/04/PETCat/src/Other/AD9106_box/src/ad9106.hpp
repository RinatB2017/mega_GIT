/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#ifndef AD9106_HPP
#define AD9106_HPP
//--------------------------------------------------------------------------------
#include <QObject>
#include <QList>
//--------------------------------------------------------------------------------
typedef struct reg
{
    QString     name;
    uint16_t    address;
    uint16_t    default_value;
} reg_t;
//--------------------------------------------------------------------------------
class AD9106 : public QObject
{
    Q_OBJECT
public:
    enum ErrorBits
    {
        MEM_READ_ERR = 0x1,
        ODD_ADDR_ERR = 0x2,
        PERIOD_SHORT_ERR = 0x4,
        DOUT_START_SHORT_ERR = 0x8,
        PAT_DLY_SHORT_ERR = 0x10,
        DOUT_START_LG_ERR = 0x20,
        ERROR_CLEAR = 0x8000
    };

    explicit AD9106(QObject *parent = nullptr);

    uint16_t get_SRAM_StartAddress(void);
    bool get_reg(QString name, reg_t *reg);
    int get_cnt_registers(void);

    bool get_sl_name_registers(QStringList *sl);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void message(const QString &);

private:
    QList<reg> AD9106_registers;

    uint16_t SRAM_StartAddress = 0x6000;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif
