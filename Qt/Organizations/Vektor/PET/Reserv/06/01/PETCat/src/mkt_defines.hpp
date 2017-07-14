#ifndef MKT_DEFINES_HPP
#define MKT_DEFINES_HPP
//--------------------------------------------------------------------------------
#include <QtGlobal>
#include <stdint.h>
//--------------------------------------------------------------------------------
typedef struct term_param
{
    QString slave_name;
    int slave_address;
    int width_window;
    int num_sensors;
    int min_temperature;
    int max_temperature;
    int interval;
} term_param_t;
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#define DEFAULT_SLAVE_NAME      "/dev/ttyUSB0"
#endif
#ifdef Q_OS_WIN
#define DEFAULT_SLAVE_NAME      "COM1"
#endif
//--------------------------------------------------------------------------------
#define MIN_MODBUS_ADDRESS  1
#define MAX_MODBUS_ADDRESS  247
//--------------------------------------------------------------------------------
#define MIN_INTERVAL    1
#define MAX_INTERVAL    60
//--------------------------------------------------------------------------------
#define MIN_SENSORS     1
#define MAX_SENSORS     50
//--------------------------------------------------------------------------------
#define MIN_WIDTH_WINDOW    1
#define MAX_WIDTH_WINDOW    10
//--------------------------------------------------------------------------------
#define DEFAULT_SLAVE_ADDRESS   1
#define DEFAULT_WIDTH_WINDOW    2
#define DEFAULT_NUM_SENSORS     1
#define DEFAULT_MIN_TEMPERATURE 0
#define DEFAULT_MAX_TEMPERATURE 50
//--------------------------------------------------------------------------------
#define MIN_TEMPERATURE     -100
#define MAX_TEMPERATURE     100
//--------------------------------------------------------------------------------
#define DEFAULT_MODBUS_TIMEOUT_SEC  1
//--------------------------------------------------------------------------------
#define VERSION                 VER_MAJOR.VER_MINOR.VER_BUILD
#define RC_ICONS                ":/images/computer.ico"
//--------------------------------------------------------------------------------
#define ADDRESS_STATUS          0
#define ADDRESS_NUM_SENSORS     1
#define ADDRESS_WIDTH_WINDOW    2
#define ADDRESS_RESET           3
#define ADDRESS_VER_FIRMVARE    4
//--------------------------------------------------------------------------------
#define ERROR_VALUE_SMALL   "значение слишком маленькое"
#define ERROR_VALUE_LARGE   "значение слишком большое"
//--------------------------------------------------------------------------------
#endif
