#ifndef DEFINES_HPP
#define DEFINES_HPP
//--------------------------------------------------------------------------------
#define htons(n) (unsigned short)((((unsigned short) (n)) << 8) | (((unsigned short) (n)) >> 8))
#define htonl(n) (unsigned int)((((unsigned int) (n)) << 16) | (((unsigned int) (n)) >> 16))
//--------------------------------------------------------------------------------
#define POWER_SUPPLY_NAME_B590 "Источник напряжения постоянного тока Б5-90"
#define POWER_SUPPLY_NAME_B588 "Источник напряжения постоянного тока Б5-88"
//--------------------------------------------------------------------------------
#define ORGNAME "Work"
#define APPNAME "Test_QHexEdit"
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
