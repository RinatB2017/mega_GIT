/*********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#ifndef STRUCTS_H
#define STRUCTS_H
//--------------------------------------------------------------------------------
#include <stdint.h>
#include "defines.h"
//--------------------------------------------------------------------------------
;
#pragma pack(push, 1)
//--------------------------------------------------------------------------------
// структура для накапливания данных
// хранится в data_measurement_filename
struct bp_measurement {
    uint8_t cnt_measurement;            // счётчик измерений
    uint32_t avg_0[NUMBER_MEASUREMENTS];  // измеренные значения
    uint32_t avg_1[NUMBER_MEASUREMENTS];  // измеренные значения
    uint16_t packet_id;
};

// структура для хранения не отправленных данных (проблема с сервером)
// хранится в data_filename
struct bp_data {
    uint32_t unixtime;
    bool not_saved;

    uint16_t values_count;  //2
    struct {
        uint32_t max;
        uint32_t min;
        uint32_t avg;
    } values[2];
};

// структура для хранения настроек
// хранится в options_filename
struct bp_options {
  char ssid[MAX_SSID];
  char pass[MAX_PASS];

  char host[MAX_HOST];

  char serial[MAX_SERIAL];
  char IMEI[MAX_IMEI];

  char ntpServer[MAX_NTP_NAME];

  unsigned short CRC16;
};

struct bp_message_data_version {
    uint8_t minor;
    uint8_t major;
};

struct bp_message_station {
    char serial[12];
    char imei[16];
};

struct bp_analog_value {
    uint32_t max;
    uint32_t min;
    uint32_t avg;
};

struct bp_message_data_error {
    union {
        struct {
            uint16_t power            : 1;
            uint16_t air_pressure     : 1;
            uint16_t wind             : 1;
            uint16_t gsm              : 1;
            uint16_t display          : 1;
            uint16_t external_memory  : 1;
            uint16_t default_settings : 1;
            uint16_t rezerved         : 8;
            uint16_t more_data        : 1;
        } flags;

        uint16_t value;
    };
};

struct bp_analog_message {
    struct {
        uint16_t magic;

        uint16_t type;
        uint16_t itype;

        uint16_t packet_id;

        uint16_t data_length;
        uint16_t data_crc;

        uint32_t hash;
    } m_header;
    struct {
        struct bp_message_data_version version;

        uint32_t unixtime;
        struct bp_message_station station;
        struct bp_message_data_error error;

        uint16_t values_count;  //ANALOG_SENSORS
        struct {
            uint32_t max;
            uint32_t min;
            uint32_t avg;
        } values[ANALOG_SENSORS];
    } m_data;
};
#pragma pack(pop)
//--------------------------------------------------------------------------------
#endif
