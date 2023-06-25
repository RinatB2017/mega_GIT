/*********************************************************************************
**                                                                              **
**     Copyright (C) 2022                                                       **
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
#include "converter_uir.hpp"
//--------------------------------------------------------------------------------
Converter_UIR::Converter_UIR(QObject *parent) :
    QObject(parent)
{
    mem_data.clear();
}
//--------------------------------------------------------------------------------
Converter_UIR::~Converter_UIR()
{

}
//--------------------------------------------------------------------------------
void Converter_UIR::input(const QString &i_data)
{
    if(i_data.isEmpty())
    {
        return;
    }
    QString temp_str;
    temp_str.append(mem_data);
    temp_str.append(i_data);

    int len = temp_str.length();
    for(int n=0; n<len; n+=2)
    {
        QString key = temp_str.mid(n, 2);
        if(key == "UD" ||
                key == "UA" ||
                key == "ID" ||
                key == "IA")
        {
            QString str_temp = temp_str.mid(n+2);
            ba_sprit(str_temp);
            break;
        }
        else
        {
            analize(key);
        }
    }
    if(len % 2)
        mem_data = temp_str.right(1);
    else
        mem_data.clear();
}
//--------------------------------------------------------------------------------
void Converter_UIR::ba_sprit(const QString &ba)
{
    if(ba.isEmpty())    return;

    bool ok;
    QStringList sl = ba.split("F");
    if(sl.count() == 2)
    {
        // 0 - значение, 1 - частота
        float t_value = sl.at(0).toFloat(&ok);
        if(ok)
        {
            value = t_value;
            Connection::set_value(value);
            emit error(QString(">>>>> ba_sprit[0] %1").arg(value));
        }

        float t_freq = sl.at(1).toFloat(&ok);
        if(ok) freq = t_freq;
    }
    else
    {
        // просто значение
        float t_value = ba.toFloat(&ok);
        if(ok)
        {
            value = t_value;
            Connection::set_value(value);
            emit error(QString(">>>>> ba_sprit[1] %1").arg(value));
        }
    }
}
//--------------------------------------------------------------------------------
void Converter_UIR::analize(const QString &key)
{
    if(key.isEmpty())   return;
    //emit debug(key);

    //Q_ASSERT(key.length() == 2);
    if(key.length() != 2)
    {
        return;
    }

    switch(state)
    {
    case STATE_NONE:
        if(key == "X1") state = STATE_U;
        if(key == "X2") state = STATE_I;
        if(key == "X0") state = STATE_R;
        if(key == "X3") state = STATE_F;
        if(key == "X4") state = STATE_F;
        break;

    case STATE_U:
        state_result = STATE_U;
        if(key == "Z7")
        {
            ba_value.clear();
            value = 0.0f;
            k_value = 1.0f;
            state = STATE_VALUE;
        }
        break;

    case STATE_I:
        state_result = STATE_I;
        if(key == "Z7")
        {
            ba_value.clear();
            value = 0.0f;
            k_value = 1.0f;
            state = STATE_VALUE;
        }
        break;

    case STATE_R:
        state_result = STATE_R;
        ba_value.clear();
        value = 0.0f;
        k_value = 1.0f;
        state = STATE_VALUE;
        break;

    case STATE_F:
        state_result = STATE_F;
        if(key == "Z7")
        {
            ba_freq.clear();
            freq  = 0.0f;
            k_freq = 1.0f;
            state = STATE_FREQ;
        }
        break;

    case STATE_VALUE:
        if(key == "Y7") k_value *= 10.0f;
        if(key == "Y6") k_value *= 0.1f;

        if(key == "X0") ba_value.append("0");
        if(key == "X1") ba_value.append("1");
        if(key == "X2") ba_value.append("2");
        if(key == "X3") ba_value.append("3");
        if(key == "X4") ba_value.append("4");
        if(key == "X5") ba_value.append("5");
        if(key == "X6") ba_value.append("6");
        if(key == "X7") ba_value.append("7");
        if(key == "X8") ba_value.append("8");
        if(key == "X9") ba_value.append("9");

        if(key == "Z7")
        {
            bool ok;
            float t_value = ba_value.toFloat(&ok);
            if(ok)
            {
                value = t_value * k_value;
                Connection::set_value(value);
                emit error(QString(">>>>> ba_sprit[2] %1").arg(value));
            }
            state = STATE_NONE;
        }
        break;

    case STATE_FREQ:
        if(key == "Y7") k_freq *= 10.0f;
        if(key == "Y6") k_freq *= 0.1f;

        if(key == "X0") ba_freq.append("0");
        if(key == "X1") ba_freq.append("1");
        if(key == "X2") ba_freq.append("2");
        if(key == "X3") ba_freq.append("3");
        if(key == "X4") ba_freq.append("4");
        if(key == "X5") ba_freq.append("5");
        if(key == "X6") ba_freq.append("6");
        if(key == "X7") ba_freq.append("7");
        if(key == "X8") ba_freq.append("8");
        if(key == "X9") ba_freq.append("9");

        if(key == "Z7")
        {
            bool ok;
            float t_freq = ba_freq.toFloat(&ok);
            if(ok)
            {
                freq = t_freq * k_freq;
            }
            state = STATE_NONE;
        }
        break;
    }
}
//--------------------------------------------------------------------------------
void Converter_UIR::print_value(void)
{
    QString temp;

    switch(state_result)
    {
    case STATE_NONE:    break;
    case STATE_U:       temp.append("U ");     break;
    case STATE_I:       temp.append("I ");     break;
    case STATE_R:       temp.append("R ");     break;
    case STATE_F:       temp.append("F ");     break;
    case STATE_VALUE:
        break;
    default:
        emit error(QString("Unknown state: %1").arg(state));
        break;
    }

    temp.append(QString("Value: %1").arg(value, 0, 'f', 3));

    emit info(temp);
}
//--------------------------------------------------------------------------------
