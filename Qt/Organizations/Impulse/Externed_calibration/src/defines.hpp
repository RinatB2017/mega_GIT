#ifndef DEFINES_HPP
#define DEFINES_HPP
//--------------------------------------------------------------------------------
#define ORGNAME "Work"
#define APPNAME "Externed_calibration"
//--------------------------------------------------------------------------------
#define QMAKE_TARGET_COMPANY    "Work"
#define QMAKE_TARGET_PRODUCT    APPNAME
#define QMAKE_TARGET_COPYRIGHT  "Copyright 2015-2020"
#define RC_ICONS                "computer.ico"
#define VERSION                 VER_MAJOR.VER_MINOR.VER_BUILD
//--------------------------------------------------------------------------------
#define POWER_SUPPLY_NAME_B588 "источник напряжения постоянного тока Б5-88"
#define POWER_SUPPLY_NAME_B590 "источник напряжения постоянного тока Б5-90"
//--------------------------------------------------------------------------------
#define DEFAULT_I   2500
//--------------------------------------------------------------------------------
enum {
  NO_TASK = 0,                  // Пусто (нет задачи)
  TASK_CALIBRATION_ADC_U = 1,   // Запуск калибровки АЦП по напряжению
  TASK_CALIBRATION_ADC_I = 2,   // Запуск калибровки АЦП по току
  TASK_CALIBRATION_DAC_U = 3,   // Запуск калибровки ЦАП по напряжению
  TASK_CALIBRATION_DAC_I = 4,   // Запуск калибровки ЦАП по току
  TASK_SLP = 5,                 // Режим SLP
  TASK_SHOW_ERROR = 6,          // Отображение ошибки - выход вниз (запомнить до тех пор пока не окажемся внизу (там отображение)
  TASK_RETURN_MENU = 7          // Выход в основное меню (или меню настроек, если неверны калибровки)
};
//--------------------------------------------------------------------------------
#define ICON_PROGRAMM   ":/mainwindow/computer.png"
//--------------------------------------------------------------------------------
#endif
