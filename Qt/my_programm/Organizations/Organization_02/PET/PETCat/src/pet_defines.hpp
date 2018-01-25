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
#ifndef PET_DEFINES_HPP
#define PET_DEFINES_HPP
//--------------------------------------------------------------------------------
#include <QHostAddress>
//#include <stdint.h>
//--------------------------------------------------------------------------------
#include "pet.h"
#include "pet_bos.h"
#include "pet_brg.h"
#include "pet_errno.h"
//--------------------------------------------------------------------------------
// отказались от перечислений 20.01.2017
#define PET_RESULT  int
//--------------------------------------------------------------------------------
#define PET_ADDR_BRG_MIN    1
#define PET_ADDR_BRG_MAX    48
#define PET_BRG_CH_MIN      0
#define PET_BRG_CH_MAX      3
//--------------------------------------------------------------------------------
#define NETMASK_IPV4(a, b, c, d)    ((uint32_t)(((uint8_t)a << 24) | ((uint8_t)b << 16) | ((uint8_t)c << 8) | (uint8_t)d))
#define IPV4(a, b, c, d)    ((uint32_t)(((uint8_t)a << 24) | ((uint8_t)b << 16) | ((uint8_t)c << 8) | (uint8_t)d))
//--------------------------------------------------------------------------------
#pragma pack(push, 1)

typedef struct ts_bytes
{
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t f;
    uint8_t reserved[2];
} ts_bytes_t;

typedef struct ts_bytes_full
{
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t f;
    uint8_t g;
    uint8_t h;
} ts_bytes_full_t;

typedef struct s_bites
{
    uint8_t bit_0 : 1;
    uint8_t bit_1 : 1;
    uint8_t bit_2 : 1;
    uint8_t bit_3 : 1;
    uint8_t bit_4 : 1;
    uint8_t bit_5 : 1;
    uint8_t bit_6 : 1;
    uint8_t bit_7 : 1;
} s_bites_t;

//---
union U_TIME_STEP
{
    ts_bytes bytes;
    uint64_t value;
};
//---
struct TIME_STEP
{
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t f;
};
union U_INT64
{
    ts_bytes_full bytes;
    uint64_t value;
};
union U_BYTE
{
    s_bites bites;
    uint8_t value;
};

//TODO pet_frame_t может поменяться
typedef struct pet_frame_error {
    uint8_t  addr;                      /* Адрес ведомого устройства */
    uint8_t  cmd;                       /* Код команды */
    uint16_t len;                       /* Размер поля данных (байт) */
    pet_error_t result_cmd;
} pet_frame_error_t;

//TODO pet_wave_t может поменяться в будущем
typedef struct pet_wave_data {
    uint8_t     channel;       /* Канал осциллографирования */
    uint8_t     mode;          /* Режим осциллографирования */
    uint16_t    count;         /* Количество точек осцилографирования. Не более PET_WAVE_POINT_MAX */
    char buffer[1000];
} pet_wave_data_t;

#pragma pack(pop)
//--------------------------------------------------------------------------------
// структура для findpetbox
typedef struct pet_discovery_data {
    QHostAddress    host;
    quint16         port;
    uint8_t         addr;
    uint32_t        vid;
    uint32_t        pid;
    uint8_t         uid[6];
} pet_discovery_data_t;
//--------------------------------------------------------------------------------
#if 0
поток данных 120 мегабайт в секунду
за минуту будет 120 * 60 = 7,2 гигабайта
за час 432 гигабайта
это не залезет в оперативку однозначно
поэтому надо считывать и анализировать по частям
#endif
//--------------------------------------------------------------------------------
#define DEFAULT_DEMO_PET_PORT       PET_NET_CMD_PORT_DEFAULT
//--------------------------------------------------------------------------------
//кол-во пикселей по горизонтали и по вертикали
#define MAX_POINT   15
//--------------------------------------------------------------------------------
#define MAX_EVENT_IN_PACKET ((UDP_MAX_DATA_SIZE - sizeof(HEADER)) / sizeof(PET_EVENT))
//--------------------------------------------------------------------------------
#define MAX_TIMEOUT_MSEC (2 * PET_TRN_TIMEOUT_DFLT)
//#define MAX_TIMEOUT_DISCOVERY_MSEC  3000
//--------------------------------------------------------------------------------
#define MIN_SIZE_PICTURE    100
#define MAX_SIZE_PICTURE    1024
//--------------------------------------------------------------------------------
// картинка всегда квадратная
#define DEFAULT_SIZE_PICTURE    512
//--------------------------------------------------------------------------------
// размер временной картинки
#define TEMP_IMAGE_W    150
#define TEMP_IMAGE_H    50
//--------------------------------------------------------------------------------
#define MAX_SIZE_Y   1025
#define MAX_SIZE_X   1025
//--------------------------------------------------------------------------------
#define MIN_UDP_PORT    1024
//#define MIN_UDP_PORT    49152
#define MAX_UDP_PORT    0xFFFF
//--------------------------------------------------------------------------------
//#define RESOLUTION_128_128      128
#define RESOLUTION_256_256      256
#define RESOLUTION_512_512      512
#define RESOLUTION_1024_1024    1024
//--------------------------------------------------------------------------------
#define NAME_ICON_0                 ":/images/0.png"
#define NAME_ICON_1                 ":/images/1.png"
#define NAME_ICON_2                 ":/images/2.png"
#define NAME_ICON_3                 ":/images/3.png"
#define NAME_ICON_4                 ":/images/4.png"
#define NAME_ICON_5                 ":/images/5.png"
#define NAME_ICON_6                 ":/images/6.png"
#define NAME_ICON_7                 ":/images/7.png"
#define NAME_ICON_8                 ":/images/8.png"
#define NAME_ICON_9                 ":/images/9.png"
#define NAME_RADIATION_ICON         ":/images/radiation.png"
#define NAME_DATABASE_ICON          ":/images/database.png"
#define NAME_HELP_ICON              ":/images/playlist.png"
//--------------------------------------------------------------------------------
#define NAME_PROGRAMM               "PET CAT"
#define NAME_PRINT                  "Печать"

#define NAME_SCAN                   "Сбор данных "
#define NAME_SCAN_CONTROL           "Управление сбором данных "
#ifdef SCAN_HARD
#define NAME_SCAN_HARD              "Сбор данных "
#endif

#define NAME_FILE_MENU              "&Файл"
#define NAME_TASKS_MENU             "&Задачи"
#define NAME_WINDOWS_MENU           "&Окна"
#define NAME_HELP_MENU              "&Помошь"

#define NAME_ANALIZE                "Анализ данных"
#define NAME_TOOLS                  "Инструменты"
#define NAME_TESTS                  "Тесты"
#define NAME_TASKS                  "Задачи"

#if (defined MKT) || (defined SIMULATOR) || (defined UPDATE_FIRMWARE)
#   define NAME_OTHER               "Прочее"
#endif

#define NAME_FLOODMAP               "FloodMap"
#define NAME_HISTOGRAM_CONSULIENCE  "Анализ совпадений"
#define NAME_HISTOGRAM_ENERGY       "Спектрометр"

#define NAME_OSCILLOSCOPE           "Осциллограф"

#define NAME_MAIN_OPTIONS           "Общие настройки"
#define NAME_OPTIONS                "Настройки"

#define NAME_LOGGING                "Логирование"

#define NAME_DEMO                   "Демо-ПЭТ"

#define NAME_IMITATOR_PET           "Имитатор ПЭТ"
#define NAME_IMITATOR_BRG           "Имитатор БРГ"

#define NAME_MKT                    "МКТ"

#define NAME_SIMULATOR              "Симулятор"

#define NAME_UPDATE_FIRMWARE        "Обновление БРГ"

#define NAME_METADATA               "Данные"
#define NAME_METADATA_APPEND        "Добавить..."

#define NAME_CREATOR                "Создание тестового файла (dat3)"

#define NAME_VIEWER                 "Просмотр данных"

#define NAME_CALIBRATION            "Калибровка БРГ"

#define NAME_TEST_1                 "Тест 1 (net)"
#define NAME_TEST_2                 "Тест 2"

#define NAME_DEMO_ICON                  NAME_RADIATION_ICON
#define NAME_MAIN_OPTIONS_ICON          NAME_RADIATION_ICON
#define NAME_LOGGING_ICON               NAME_RADIATION_ICON
#define NAME_OPTIONS_ICON               NAME_RADIATION_ICON
#define NAME_SCAN_ICON                  NAME_RADIATION_ICON
#define NAME_SCAN_CONTROL_ICON          NAME_RADIATION_ICON
#define NAME_OSCILLOSCOPE_ICON          NAME_RADIATION_ICON
#define NAME_FLOODMAP_ICON              NAME_RADIATION_ICON
#define NAME_HISTOGRAM_CONSULIENCE_ICON NAME_RADIATION_ICON
#define NAME_HISTOGRAM_ENERGY_ICON      NAME_RADIATION_ICON
#define NAME_IMITATOR_PET_ICON          NAME_RADIATION_ICON
#define NAME_IMITATOR_BRG_ICON          NAME_RADIATION_ICON
#define NAME_CREATOR_ICON               NAME_RADIATION_ICON
#define NAME_VIEWER_ICON                NAME_RADIATION_ICON
#define NAME_SERVER_ICON                NAME_RADIATION_ICON
#define NAME_OTHER_ICON                 NAME_RADIATION_ICON
#define NAME_CALIBRATION_ICON           NAME_RADIATION_ICON
#define NAME_TEST_1_ICON                NAME_RADIATION_ICON
#define NAME_TEST_2_ICON                NAME_RADIATION_ICON
#define NAME_MKT_ICON                   NAME_RADIATION_ICON
#define NAME_SIMULATOR_ICON             NAME_RADIATION_ICON
#define NAME_UPDATE_FIRMWARE_ICON       NAME_RADIATION_ICON
//--------------------------------------------------------------------------------
#define MAX_SIZE_BUF_ENERGY 1000000
//---
#define HISTOGRAM_MIN_ENERGY    0
#define HISTOGRAM_MAX_ENERGY    (4 * 0xFFFF)
#define HISTOGRAM_MIN_N         10
#define HISTOGRAM_MAX_N         5000
//--------------------------------------------------------------------------------
#define DEFAULT_SIZE    640
#define DEFAULT_R       (DEFAULT_SIZE / 2 - 50)
#define CENTER_X        (DEFAULT_SIZE / 2)
#define CENTER_Y        (DEFAULT_SIZE / 2)
//--------------------------------------------------------------------------------
#define SB_TIME_MIN     10
#define SB_TIME_MAX     630
#define SB_TIME_STEP    10
#define SB_TIME_DEFAULT SB_TIME_MAX
//--------------------------------------------------------------------------------
#define MIN_WINDOW_CONSILENCE 10
#define MAX_WINDOW_CONSILENCE 10000
//--------------------------------------------------------------------------------
enum ID {
    ID_OSCILLOSCOPE = 1000,
    ID_LOGGING,
    ID_SCAN_CONTROL,
    ID_SCAN_HARD,
    ID_FLOODMAP,
    ID_HISTOGRAM_CONSULIENCE,
    ID_HISTOGRAM_ENERGY,
    ID_OPTIONS,
    ID_DEMO,
    ID_IMITATOR_PET,
    ID_IMITATOR_BRG,
    ID_CREATOR,
    ID_VIEWER,
    ID_MKT,
    ID_SIMULATOR,
    ID_CALIBRATION,
    ID_TEST_1,
    ID_TEST_2,
    ID_UPDATE_FIRMWARE
};
enum PALETTE {
    GRAY = 2000,
    COLOR
};
enum CURVE {
    DOTS = 0,
    LINES,
    SPLINE_LINES
};
//--------------------------------------------------------------------------------
#define ID_METADATA_APPEND  2000
#define ID_METADATA_DELETE  3000
#define ID_METADATA         4000
//--------------------------------------------------------------------------------
// макс кол-во событий, читаемых за раз
#define MAX_EVENTS      1000000
// макс размер буфера для хранения событий
#define MAX_EVENTS_BUF  (MAX_EVENTS * sizeof(struct pet_event))
//--------------------------------------------------------------------------------
#define SERVER_NAME_SIZE           50
#define SERVER_DESCRIPTION_SIZE    100
//--------------------------------------------------------------------------------
#define COLOR_CURVE_GRAPHER             QColor(170, 0, 120)
#define COLOR_CURVE_GRAPHER_DEFAULT     QColor(Qt::red)
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
    //#define HOME_DIR    "/home/boss/BitTorrent_sync/PETCAT"
    //#define HOME_DIR    "~/Programming/my_programm_bin/Organization_02/PETCat/"
    #define HOME_DIR    "/dev/shm"
#else
    #define HOME_DIR    "."
#endif
//--------------------------------------------------------------------------------
#define DEFAULT_BROADCAT_ADDR "192.168.10.255"
//#define DEFAULT_BROADCAT_ADDR "192.168.0.255"
//--------------------------------------------------------------------------------
#define PETCAT_OPTIONS_MAIN_GROUP                   "Main_options"
#define PETCAT_OPTIONS_FLOODMAP_GROUP               "Floodmap_options"
#define PETCAT_OPTIONS_OSCILLOSCOPE_GROUP           "Oscilloscope_options"
#define PETCAT_OPTIONS_HISTOGRAM_CONSULIENCE_GROUP  "Histogram_consulience_options"
#define PETCAT_OPTIONS_HISTOGRAM_ENERGY_GROUP       "Histogram_energy_options"
#define PETCAT_OPTIONS_IMITATOR_BRG_GROUP           "Imitator_brg_options"

#define PARAM_GROUP_NAME        "Options"
#define PARAM_PORT_CMD_NAME     "port_cmd"
#define PARAM_PORT_EVENT_NAME   "port_event"
#define PARAM_MAX_PET_TIMEOUT   "max_pet_timeout"
#define PARAM_MAX_NET_TIMEOUT   "max_net_timeout"

#define PARAM_IS_SHOWS_INFO     "is_shows_info"
#define PARAM_IS_SHOWS_DEBUG    "is_shows_debug"
#define PARAM_IS_SHOWS_ERROR    "is_shows_error"
#define PARAM_IS_SHOWS_TRACE    "is_shows_trace"

#define DEFAULT_PARAM_IS_SHOWS_INFO     true
#define DEFAULT_PARAM_IS_SHOWS_DEBUG    true
#define DEFAULT_PARAM_IS_SHOWS_ERROR    true
#define DEFAULT_PARAM_IS_SHOWS_TRACE    true

#define DEFAULT_PARAM_MAX_NET_TIMEOUT 3000
#define DEFAULT_PARAM_MAX_PET_TIMEOUT 1000
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
#define PARAM_oscilloscope_cb_sync           "oscilloscope_cb_sync"
#define PARAM_oscilloscope_cb_auto           "oscilloscope_cb_auto"
#define PARAM_oscilloscope_cb_type_curve     "oscilloscope_cb_type_curve"
#define PARAM_oscilloscope_cb_channel        "oscilloscope_cb_channel"
#define PARAM_oscilloscope_sb_time           "oscilloscope_sb_time"
#define PARAM_oscilloscope_sb_vert_scale_min "oscilloscope_sb_vert_scale_min"
#define PARAM_oscilloscope_sb_vert_scale_max "oscilloscope_sb_vert_scale_max"

#define PARAM_oscilloscope_clr_x_plus        "oscilloscope_clr_x_plus"
#define PARAM_oscilloscope_clr_x_minus       "oscilloscope_clr_x_minus"
#define PARAM_oscilloscope_clr_y_plus        "oscilloscope_clr_y_plus"
#define PARAM_oscilloscope_clr_y_minus       "oscilloscope_clr_y_minus"
#define PARAM_oscilloscope_clr_sum           "oscilloscope_clr_sum"

#define DEFAULT_PARAM_oscilloscope_cb_sync         0
#define DEFAULT_PARAM_oscilloscope_cb_auto         0
#define DEFAULT_PARAM_oscilloscope_cb_type_curve   1
#define DEFAULT_PARAM_oscilloscope_cb_channel      0
#define DEFAULT_PARAM_oscilloscope_sb_vert_scale_min   -32768
#define DEFAULT_PARAM_oscilloscope_sb_vert_scale_max   32768

#define DEFAULT_PARAM_oscilloscope_clr_x_plus      0xFF0000
#define DEFAULT_PARAM_oscilloscope_clr_x_minus     0x00FF00
#define DEFAULT_PARAM_oscilloscope_clr_y_plus      0xFF0000
#define DEFAULT_PARAM_oscilloscope_clr_y_minus     0x00FF00
#define DEFAULT_PARAM_oscilloscope_clr_sum         0x0000FF
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
#define PARAM_histogram_consulience_sb_nudChANumber   "histogram_consulience_sb_nudChANumber"
#define PARAM_histogram_consulience_sb_nudChBNumber   "histogram_consulience_sb_nudChBNumber"
#define PARAM_histogram_consulience_dsb_nudChACor     "histogram_consulience_dsb_nudChACorr"
#define PARAM_histogram_consulience_dsb_nudChBCor     "histogram_consulience_dsb_nudChBCorr"
#define PARAM_histogram_consulience_dsb_nudTInt       "histogram_consulience_dsb_nudTInt"
#define PARAM_histogram_consulience_dsb_nudWindow     "histogram_consulience_dsb_nudWindow"
#define PARAM_histogram_consulience_dsb_nudN          "histogram_consulience_dsb_nudN"
#define PARAM_histogram_consulience_clr_p             "histogram_consulience_clr_p"
#define PARAM_histogram_consulience_clr_gauss         "histogram_consulience_clr_gauss"

#define DEFAULT_PARAM_histogram_consulience_sb_nudChANumber 2
#define DEFAULT_PARAM_histogram_consulience_sb_nudChBNumber 2
#define DEFAULT_PARAM_histogram_consulience_dsb_nudChACor   0
#define DEFAULT_PARAM_histogram_consulience_dsb_nudChBCor   0
#define DEFAULT_PARAM_histogram_consulience_dsb_nudTInt     10
#define DEFAULT_PARAM_histogram_consulience_dsb_nudWindow   10
#define DEFAULT_PARAM_histogram_consulience_dsb_nudN        1000
#define DEFAULT_PARAM_histogram_consulience_clr_p           0xFF0000
#define DEFAULT_PARAM_histogram_consulience_clr_gauss       0x00FF00

#define PARAM_histogram_consulience_channel_a_expanded      "histogram_consulience_channel_a_expanded"
#define PARAM_histogram_consulience_channel_b_expanded      "histogram_consulience_channel_b_expanded"
#define PARAM_histogram_consulience_ts_expanded             "histogram_consulience_ts_expanded"
#define PARAM_histogram_consulience_consillence_expanded    "histogram_consulience_consillence_expanded"
#define PARAM_histogram_consulience_analiz_expanded         "histogram_consulience_analiz_expanded"
#define PARAM_histogram_consulience_events_flag_expanded    "histogram_consulience_events_flag_expanded"
#define PARAM_histogram_consulience_param_expanded          "histogram_consulience_param_expanded"

#define DEFAULT_PARAM_histogram_consulience_channel_a_expanded      false
#define DEFAULT_PARAM_histogram_consulience_channel_b_expanded      false
#define DEFAULT_PARAM_histogram_consulience_ts_expanded             false
#define DEFAULT_PARAM_histogram_consulience_consillence_expanded    false
#define DEFAULT_PARAM_histogram_consulience_analiz_expanded         false
#define DEFAULT_PARAM_histogram_consulience_events_flag_expanded    false
#define DEFAULT_PARAM_histogram_consulience_param_expanded          false
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
#define PARAM_histogram_energy_sb_e_min             "histogram_energy_sb_e_min"
#define PARAM_histogram_energy_sb_e_max             "histogram_energy_sb_e_max"
#define PARAM_histogram_energy_sb_N                 "histogram_energy_sb_N"

#define DEFAULT_PARAM_histogram_energy_sb_e_min     10
#define DEFAULT_PARAM_histogram_energy_sb_e_max     100000
#define DEFAULT_PARAM_histogram_energy_sb_N         2000

#define PARAM_histogram_energy_color_histogramm     "histogram_energy_color_histogramm"
#define DEFAULT_PARAM_histogram_energy_color_histogramm 0xFF0000

#define PARAM_histogram_energy_flag_e_min           "histogram_energy_flag_e_min"
#define PARAM_histogram_energy_flag_e_max           "histogram_energy_flag_e_max"

#define DEFAULT_PARAM_histogram_energy_flag_e_min   false
#define DEFAULT_PARAM_histogram_energy_flag_e_max   false

#define PARAM_histogram_energy_color_histogramm     "histogram_energy_color_histogramm"

#define PARAM_histogram_energy_source_expanded      "histogram_energy_source_expanded"
#define PARAM_histogram_energy_energy_expanded      "histogram_energy_energy_expanded"
#define PARAM_histogram_energy_events_flag_expanded "histogram_energy_events_flag_expanded"
#define PARAM_histogram_energy_param_expanded       "histogram_energy_param_expanded"

#define DEFAULT_PARAM_histogram_energy_source_expanded      false
#define DEFAULT_PARAM_histogram_energy_energy_expanded      false
#define DEFAULT_PARAM_histogram_energy_events_flag_expanded false
#define DEFAULT_PARAM_histogram_energy_param_expanded       false
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
#define PARAM_floodmap_events_flags                         "floodmap_events_flags"
#define DEFAULT_PARAM_floodmap_events_flags                 0xfe

#define PARAM_histogram_energy_events_flags                 "histogram_energy_events_flags"
#define DEFAULT_PARAM_histogram_energy_events_flags         0xfe

#define PARAM_histogram_consulience_events_flags            "histogram_consulience_events_flags"
#define DEFAULT_PARAM_histogram_consulience_events_flags    0xfe

#define PARAM_floodmap_sb_min_energy                    "floodmap_sb_min_energy"
#define PARAM_floodmap_sb_max_energy                    "floodmap_sb_max_energy"

#define DEFAULT_PARAM_floodmap_sb_min_energy            0
#define DEFAULT_PARAM_floodmap_sb_max_energy            4 * 0xFFFF

#define PARAM_floodmap_flag_min_energy                  "floodmap_flag_min_energy"
#define PARAM_floodmap_flag_max_energy                  "floodmap_flag_max_energy"

#define DEFAULT_PARAM_floodmap_flag_min_energy          false
#define DEFAULT_PARAM_floodmap_flag_max_energy          false

#define PARAM_floodmap_source_expanded                  "floodmap_source_expanded"
#define PARAM_floodmap_energy_expanded                  "floodmap_energy_expanded"
#define PARAM_floodmap_events_flag_expanded             "floodmap_events_flag_expanded"
#define PARAM_floodmap_picture_expanded                 "floodmap_picture_expanded"

#define DEFAULT_PARAM_floodmap_source_expanded          false
#define DEFAULT_PARAM_floodmap_energy_expanded          false
#define DEFAULT_PARAM_floodmap_event_flag_expanded      false
#define DEFAULT_PARAM_floodmap_picture_expanded         false
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
#define PARAM_imitator_brg_vid      "imitator_brg_vid"
#define PARAM_imitator_brg_pid      "imitator_brg_pid"
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
#define IMITATOR_BRG_DEFAULT_VID    0xC251
#define IMITATOR_BRG_DEFAULT_PID    0x2301
//--------------------------------------------------------------------------------
#endif
