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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include <QModbusServer>
#include <QButtonGroup>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
    class SettingsDialog;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class SerialBox5;
class QToolButton;
class QComboBox;
class QLineEdit;
class QToolBar;
class QHexEdit;
class Modbus;
class SettingsDialog;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent,
            MySplashScreen *splash);
    ~MainBox(void);

private slots:
    void test(void);

private Q_SLOTS:
    void on_connectButton_clicked(void);
    void onStateChanged(int state);

    void coilChanged(int id);
    void discreteInputChanged(int id);
    void bitChanged(int id, QModbusDataUnit::RegisterType table, bool value);

    void setRegister(const QString &value);
    void updateWidgets(QModbusDataUnit::RegisterType table, int address, int size);

    void on_connectType_currentIndexChanged(int);

    void handleDeviceError(QModbusDevice::Error newError);

private:
    MySplashScreen *splash;
    Ui::MainBox *ui;
    SerialBox5 *serialBox;

    void init(void);

    void createTestBar(void);

    void initActions(void);
    void setupDeviceData(void);
    void setupWidgetContainers(void);

    QModbusServer *modbusDevice;

    QButtonGroup coilButtons;
    QButtonGroup discreteButtons;
    QHash<QString, QLineEdit *> registers;
    SettingsDialog *m_settingsDialog;

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
