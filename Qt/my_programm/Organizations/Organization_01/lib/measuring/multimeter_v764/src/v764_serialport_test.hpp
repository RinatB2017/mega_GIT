/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#ifndef SERIALPORT_V764_TEST_HPP
#define SERIALPORT_V764_TEST_HPP
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QObject>
//--------------------------------------------------------------------------------
class QPushButton;
class QCheckBox;
class QSpinBox;
//--------------------------------------------------------------------------------
class SerialPort_V764_test : public QObject
{
    Q_OBJECT

public:
    explicit SerialPort_V764_test(QObject *parent = nullptr);
    ~SerialPort_V764_test();

    bool isOpen(void);
    void close(void);
    void setPort(QSerialPortInfo info);
    bool setBaudRate(int speed);
    bool open(int mode);
    QByteArray readAll(void);

    void write(QByteArray data);
    void write(const char *data, int len);

signals:
    void readyRead(void);
    void error(QSerialPort::SerialPortError);

    void measurement_mode(int);
    void current_limit(int);
    void current_sign(int);

public slots:
    void receive(const QVariant &data);

private slots:
    void create_fake_toolbar(void);
    void regenerate_value(void);
    void hand_data(bool state);

private:
    QByteArray input;
    QByteArray output;

    bool is_open;
    bool is_hand_value;
    int measuring_value;
    unsigned char code_button;
    unsigned char mode_measuring;

    QSpinBox *sb_value;
    QCheckBox *cb_hand_value;

    bool check_question_45(void);
    bool check_question_46(void);
    bool check_question_47(void);
    bool check_question_48(void);
    bool check_question_53(void);

    void prepare_answer_45(void);
    void prepare_answer_46(void);
    void prepare_answer_47(void);
    void prepare_answer_48(void);
    void prepare_answer_53(void);
};
//--------------------------------------------------------------------------------
#endif
