#ifndef DEFINES_HPP
#define DEFINES_HPP
//--------------------------------------------------------------------------------
#define ORGNAME "Work"
#define APPNAME "Sniffer_PCA10000"
//--------------------------------------------------------------------------------
#define QMAKE_TARGET_COMPANY    "Work"
#define QMAKE_TARGET_PRODUCT    APPNAME
#define QMAKE_TARGET_COPYRIGHT  "Copyright \\251 2015-2020"
#define RC_ICONS                "computer.ico"
#define VERSION                 VER_MAJOR.VER_MINOR.VER_BUILD
//--------------------------------------------------------------------------------
#define SLIP_START      0xAB
#define SLIP_END        0xBC
#define SLIP_ESC        0xCD
#define SLIP_ESC_START  SLIP_START+1
#define SLIP_ESC_END    SLIP_END+1
#define SLIP_ESC_ESC    SLIP_ESC+1

#define REQ_FOLLOW              0x00
#define RESP_FOLLOW             0x01
#define EVENT_DEVICE            0x02
#define REQ_SINGLE_PACKET       0x03
#define RESP_SINGLE_PACKET      0x04
#define EVENT_CONNECT           0x05
#define EVENT_PACKET            0x06
#define REQ_SCAN_CONT           0x07
#define RESP_SCAN_CONT          0x08
#define EVENT_DISCONNECT        0x09
#define EVENT_ERROR             0x0A
#define EVENT_EMPTY_DATA_PACKET 0x0B
#define SET_TEMPORARY_KEY       0x0C
#define PING_REQ                0x0D
#define PING_RESP               0x0E
#define TEST_COMMAND_ID         0x0F
#define TEST_RESULT_ID          0x10
#define UART_TEST_START         0x11
#define UART_DUMMY_PACKET       0x12
#define SWITCH_BAUD_RATE_REQ    0x13
#define SWITCH_BAUD_RATE_RESP   0x14
#define UART_OUT_START          0x15
#define UART_OUT_STOP           0x16
#define SET_ADV_CHANNEL_HOP_SEQ 0x17
#define GO_IDLE                 0xFE

//ADV_ACCESS_ADDRESS = [0xD6, 0xBE, 0x89, 0x8E]

#define SYNCWORD_POS    0
#define HEADER_LEN_POS  0
#define PAYLOAD_LEN_POS     HEADER_LEN_POS+1
#define PROTOVER_POS        PAYLOAD_LEN_POS+1
#define PACKETCOUNTER_POS   PROTOVER_POS+1
#define ID_POS              PACKETCOUNTER_POS+2

#define BLE_HEADER_LEN_POS  ID_POS+1
#define FLAGS_POS           BLE_HEADER_LEN_POS+1
#define CHANNEL_POS         FLAGS_POS+1
#define RSSI_POS            CHANNEL_POS+1
#define EVENTCOUNTER_POS    RSSI_POS+1
#define TIMESTAMP_POS       EVENTCOUNTER_POS+2
#define BLEPACKET_POS       TIMESTAMP_POS+4
#define TXADD_POS           BLEPACKET_POS + 4
#define TXADD_MSK           0x40
#define PAYLOAD_POS         BLE_HEADER_LEN_POS

#define HEADER_LENGTH       6
#define BLE_HEADER_LENGTH   10
#define PROTOVER            1

#define ADV_TYPE_ADV_IND    0x0
#define ADV_TYPE_ADV_DIRECT_IND     0x1
#define ADV_TYPE_ADV_NONCONN_IND    0x2
#define ADV_TYPE_ADV_DISCOVER_IND   0x6
#define ADV_TYPE_SCAN_REQ   0x3
#define ADV_TYPE_SCAN_RSP   0x4
#define ADV_TYPE_CONNECT_REQ    0x5
//--------------------------------------------------------------------------------
#define TEST_0  1000
#define TEST_0_TEXT  "test 0"

#define TEST_1  1001
#define TEST_1_TEXT  "test 1"

#define TEST_2  1002
#define TEST_2_TEXT  "test 2"

#define TEST_3  1003
#define TEST_3_TEXT  "test 3"
//--------------------------------------------------------------------------------
#define ICON_PROGRAMM   ":/mainwindow/computer.png"
//--------------------------------------------------------------------------------
#endif
