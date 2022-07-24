#ifndef DEFINES_H
#define DEFINES_H

const int KEY = 22;         // кнопка переключения server/client

const int PIN_NOIS = 16;    // влажность
const int PIN_TEMP = 17;    // температура

#define MAX_SSID    50
#define MAX_PASS    50

#define MAX_HOST    50
#define MAX_SERIAL  50
#define MAX_IMEI    50

#define MAX_NTP_NAME    50

#define uS_TO_S_FACTOR 1000000
#define TIME_TO_SLEEP  5*60     // 5 минут
#define NUMBER_MEASUREMENTS 5   // количество измерений до передачи

#define ANALOG_SENSORS  2       // количество подключенных датчиков

#endif
