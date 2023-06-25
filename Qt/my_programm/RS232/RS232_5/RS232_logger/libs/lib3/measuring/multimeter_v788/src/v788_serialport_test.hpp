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
#ifndef V788_SERIALPORT_TEST_HPP
#define V788_SERIALPORT_TEST_HPP
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QCheckBox>
#include <QSpinBox>
#include <QObject>
#include <QtMath>
//--------------------------------------------------------------------------------
#include <QtGlobal>
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
class SerialPort_V788_test : public QObject
{
    Q_OBJECT

public:
    explicit SerialPort_V788_test(QObject *parent = nullptr);
    virtual ~SerialPort_V788_test();

    bool isOpen(void);
    void close(void);
    void setPort(QSerialPortInfo info);
    bool setBaudRate(int speed);
    bool open(int mode);
    QByteArray readAll(void);

    bool serial_open(void);
    void serial_close(void);
    void setPortName(QString);
    QStringList get_port_names(void);

    // входные данные
    void write(const QByteArray &data);
    void write(const char *data, int len);

    void receive(const QVariant &data);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    // выходные данные
    void output(const QByteArray &data);

    void readyRead(void);
    void s_error(QSerialPort::SerialPortError);

    void measurement_mode(int);
    void current_limit(int);
    void current_sign(int);

public slots:
    // входные данные
    void input(const QByteArray &data);

    //TODO генерация значения синусоиды
    void get_test_value(void);

private slots:
    void create_fake_toolbar(void);
    void regenerate_value(void);
    void hand_data(bool state);

private:
    QByteArray ba_input;
    QByteArray ba_output;

    bool is_open = false;
    bool is_hand_value = false;
    unsigned char code_button = 0;
    unsigned char mode_measuring = 0;

    qreal measuring_value = 0;
    int index_measuring_value = 0;

    QSpinBox *sb_value = nullptr;
    QCheckBox *cb_hand_value = nullptr;

    void print_log(const QString &text);
    void print_err_log(const QString &text);

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
