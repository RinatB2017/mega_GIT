#ifndef DEFINES_HPP
#define DEFINES_HPP
//--------------------------------------------------------------------------------
#define ORGNAME "Work"
//--------------------------------------------------------------------------------
#ifdef USE_B588
#ifndef USER_CALIBRATIONS
    #define APPNAME "Фабричная калибровка источников напряжения постоянного тока Б5-88"
#else
    #define APPNAME "Калибровка источников напряжения постоянного тока Б5-88"
#endif
#endif

#ifdef USE_B590
#ifndef USER_CALIBRATIONS
    #define APPNAME "Фабричная калибровка источников напряжения постоянного тока Б5-90"
#else
    #define APPNAME "Калибровка источников напряжения постоянного тока Б5-90"
#endif
#endif
//--------------------------------------------------------------------------------
#define POWER_SUPPLY_NAME_B588 "Источник напряжения постоянного тока Б5-88"
#define POWER_SUPPLY_NAME_B590 "Источник напряжения постоянного тока Б5-90"
//--------------------------------------------------------------------------------
#define PICTURE_OK      ":/pictures/button_ok.png"
#define PICTURE_FAIL    ":/pictures/button_cancel.png"
//--------------------------------------------------------------------------------
#define DEFAULT_U       3000
#define DEFAULT_I       2500
#define TIME_READY_POWERSUPPLY_MS   3000

#define MAX_REPEAT_FIND_U   50
#define MAX_REPEAT_FIND_I   50
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
#define REPORT_NAME     "report/powersupply_%1_%2"
#define REPORT_NAME_PDF "%1/report/powersupply_%2_%3.pdf"
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
#define DELTA_TEMP_CALIBRATION_CURRENT  15
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// значения в mV
#define B588_U_POINT_1  1000
#define B588_U_POINT_2  60000
#define B588_U_POINT_3  64000

// значения в uV
#define B588_U_MAX_DEVIATION_POINT_1    3
#define B588_U_MAX_DEVIATION_POINT_2    200
#define B588_U_MAX_DEVIATION_POINT_3    200

// начальное значение DAC
#define B588_U_BEGIN_DAC_POINT_1    1000
#define B588_U_BEGIN_DAC_POINT_2    60000
#define B588_U_BEGIN_DAC_POINT_3    64000
//--------------------------------------------------------------------------------
// значения в mA
#define B588_I_POINT_0  0
#define B588_I_POINT_1  50
#define B588_I_POINT_2  100
#define B588_I_POINT_3  250
#define B588_I_POINT_4  500
#define B588_I_POINT_5  1000
#define B588_I_POINT_6  2500
#define B588_I_POINT_7  5000
#define B588_I_POINT_8  10000
#define B588_I_POINT_9  25000
#define B588_I_POINT_10 37500
#define B588_I_POINT_11 50000
#define B588_I_POINT_12 64000

// значения в uV
#define B588_I_MAX_DEVIATION_POINT_1    3
#define B588_I_MAX_DEVIATION_POINT_2    3
#define B588_I_MAX_DEVIATION_POINT_3    3

// значения в uV
#define B588_I_BEGIN_DAC_POINT_1    3
#define B588_I_BEGIN_DAC_POINT_2    200
#define B588_I_BEGIN_DAC_POINT_3    250
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// значения в mV
#define B590_U_POINT_0  100
#define B590_U_POINT_1  1000
#define B590_U_POINT_2  60000
#define B590_U_POINT_3  65000

// значения в uV
#define B590_U_MAX_DEVIATION_POINT_0    200
#define B590_U_MAX_DEVIATION_POINT_1    200
#define B590_U_MAX_DEVIATION_POINT_2    200
#define B590_U_MAX_DEVIATION_POINT_3    200

// начальное значение DAC
#define B590_U_BEGIN_DAC_POINT_0    10
#define B590_U_BEGIN_DAC_POINT_1    1000
#define B590_U_BEGIN_DAC_POINT_2    60000
#define B590_U_BEGIN_DAC_POINT_3    65000
//--------------------------------------------------------------------------------
// значения в mA
#define B590_I_POINT_0  0
#define B590_I_POINT_1  50
#define B590_I_POINT_2  100
#define B590_I_POINT_3  250
#define B590_I_POINT_4  500
#define B590_I_POINT_5  1000
#define B590_I_POINT_6  2500
#define B590_I_POINT_7  5000
#define B590_I_POINT_8  10000
#define B590_I_POINT_9  25000
#define B590_I_POINT_10 37500
#define B590_I_POINT_11 50000
#define B590_I_POINT_12 64000

// значения в uV
#define B590_I_MAX_DEVIATION_POINT_0    200
#define B590_I_MAX_DEVIATION_POINT_1    200
#define B590_I_MAX_DEVIATION_POINT_2    200
#define B590_I_MAX_DEVIATION_POINT_3    250
#define B590_I_MAX_DEVIATION_POINT_4    250
#define B590_I_MAX_DEVIATION_POINT_5    200
#define B590_I_MAX_DEVIATION_POINT_6    200
#define B590_I_MAX_DEVIATION_POINT_7    200
#define B590_I_MAX_DEVIATION_POINT_8    200
#define B590_I_MAX_DEVIATION_POINT_9    200
#define B590_I_MAX_DEVIATION_POINT_10   200
#define B590_I_MAX_DEVIATION_POINT_11   200
#define B590_I_MAX_DEVIATION_POINT_12   200

// начальное значение DAC
#define B590_I_BEGIN_DAC_POINT_0    0
#define B590_I_BEGIN_DAC_POINT_1    50
#define B590_I_BEGIN_DAC_POINT_2    100
#define B590_I_BEGIN_DAC_POINT_3    250
#define B590_I_BEGIN_DAC_POINT_4    500
#define B590_I_BEGIN_DAC_POINT_5    1000
#define B590_I_BEGIN_DAC_POINT_6    2500
#define B590_I_BEGIN_DAC_POINT_7    5000
#define B590_I_BEGIN_DAC_POINT_8    10000
#define B590_I_BEGIN_DAC_POINT_9    25000
#define B590_I_BEGIN_DAC_POINT_10   37500
#define B590_I_BEGIN_DAC_POINT_11   50000
#define B590_I_BEGIN_DAC_POINT_12   64000

#define B590_I_END_DAC_POINT_0      1000
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
#define CMD_FIND_U_POINT_0      1000
#define CMD_FIND_U_POINT_0_TEXT "find U_POINT_0"

#define CMD_FIND_U_POINT_1      1010
#define CMD_FIND_U_POINT_1_TEXT "find U_POINT_1"

#define CMD_FIND_U_POINT_2      1020
#define CMD_FIND_U_POINT_2_TEXT "find U_POINT_2"

#define CMD_FIND_U_POINT_3      1030
#define CMD_FIND_U_POINT_3_TEXT "find U_POINT_3"

//---
#define CMD_FIND_I_POINT_0      2000
#define CMD_FIND_I_POINT_0_TEXT "find I_POINT_0"

#define CMD_FIND_I_POINT_1      2010
#define CMD_FIND_I_POINT_1_TEXT "find I_POINT_1"

#define CMD_FIND_I_POINT_2      2020
#define CMD_FIND_I_POINT_2_TEXT "find I_POINT_2"

#define CMD_FIND_I_POINT_3      2030
#define CMD_FIND_I_POINT_3_TEXT "find I_POINT_3"

#define CMD_FIND_I_POINT_4      2040
#define CMD_FIND_I_POINT_4_TEXT "find I_POINT_4"

#define CMD_FIND_I_POINT_5      2050
#define CMD_FIND_I_POINT_5_TEXT "find I_POINT_5"

#define CMD_FIND_I_POINT_6      2060
#define CMD_FIND_I_POINT_6_TEXT "find I_POINT_6"

#define CMD_FIND_I_POINT_7      2070
#define CMD_FIND_I_POINT_7_TEXT "find I_POINT_7"

#define CMD_FIND_I_POINT_8      2080
#define CMD_FIND_I_POINT_8_TEXT "find I_POINT_8"

#define CMD_FIND_I_POINT_9      2090
#define CMD_FIND_I_POINT_9_TEXT "find I_POINT_9"

#define CMD_FIND_I_POINT_10      2100
#define CMD_FIND_I_POINT_10_TEXT "find I_POINT_10"

#define CMD_FIND_I_POINT_11      2110
#define CMD_FIND_I_POINT_11_TEXT "find I_POINT_11"

#define CMD_FIND_I_POINT_12      2120
#define CMD_FIND_I_POINT_12_TEXT "find I_POINT_12"

//---
#define CMD_TEST                3000
#define CMD_TEST_TEXT           "test"
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
#define CONTROL_1V_TEXT         "1В"
#define CONTROL_15V_TEXT        "15В"
#define CONTROL_30V_TEXT        "30В"
#define CONTROL_60V_TEXT        "60В"

#define CONTROL_01A_TEXT        "0.1A"
#define CONTROL_5A_TEXT         "5A"
#define CONTROL_10A_TEXT        "10A"
#define CONTROL_30A_TEXT        "30A"
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
#define ICON_PROGRAMM   ":/mainwindow/computer.png"
//--------------------------------------------------------------------------------
#endif
