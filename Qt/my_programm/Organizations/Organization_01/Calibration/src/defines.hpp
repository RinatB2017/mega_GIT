#ifndef DEFINES_HPP
#define DEFINES_HPP
//--------------------------------------------------------------------------------
#include <QtGlobal>
//--------------------------------------------------------------------------------
#ifdef Q_OS_WIN
#   include <winsock2.h>
#   include <windows.h>
#endif
//--------------------------------------------------------------------------------
#define TRANSCEIVER         tr("Мультиметр В7-86 (USB)")
#define CALIBRATOR          tr("Калибратор (RS-232)")

#define TREE_LIMITS_CAPTION     tr("Пределы (калибровка)")

#define TREE_CONTROLS_CAPTION_U     tr("Напряжение")
#define TREE_CONTROLS_CAPTION_I     tr("Ток")
#define TREE_CONTROLS_CAPTION_O     tr("RLC")
#define TREE_CONTROLS_CAPTION_HLU   tr("Hi/Lo напряжение")

#define UDC                 tr("Напряжение постоянного тока")
#define UAC                 tr("Напряжение переменного тока")

#define IDC                 tr("Сила постоянного тока")
#define IAC                 tr("Сила переменного тока")

#define RR                  tr("Сопротивление")

#define LIMIT_UDC_1000V     tr("Напряжение постоянного тока, предел Udc = 1000В (требуется усилитель)")
#define LIMIT_UAC_200V      tr("Напряжение переменного тока, предел Uac = 200В (требуется усилитель)")
#define LIMIT_UAC_1000V     tr("Напряжение переменного тока, предел Uac = 750В (требуется усилитель)")
#define LIMIT_IDC_20A       tr("Сила постоянного тока, предел Idc = 20А (требуется усилитель)")
#define LIMIT_IAC_20A       tr("Сила переменного тока, предел Iac = 20А (требуется усилитель)")

#define LIMIT_UDC_200mV     tr("Предел Udc = 200мВ")
#define LIMIT_UDC_2V        tr("Предел Udc = 2В")
#define LIMIT_UDC_20V       tr("Предел Udc = 20В")
#define LIMIT_UDC_200V      tr("Предел Udc = 200В")

#define LIMIT_UAC_200mV     tr("Предел Uac = 200мВ")
#define LIMIT_UAC_2V        tr("Предел Uac = 2В")
#define LIMIT_UAC_20V       tr("Предел Uac = 20В")

#define LIMIT_IDC_200uA     tr("Предел Idc = 200мкА")
#define LIMIT_IDC_2mA       tr("Предел Idc = 2мА")
#define LIMIT_IDC_20mA      tr("Предел Idc = 20мА")
#define LIMIT_IDC_200mA     tr("Предел Idc = 200мА")
#define LIMIT_IDC_2000mA    tr("Предел Idc = 2000мА")

#define LIMIT_IAC_2mA       tr("Предел Iac = 2мА")
#define LIMIT_IAC_20mA      tr("Предел Iac = 20мА")
#define LIMIT_IAC_200mA     tr("Предел Iac = 200мА")
#define LIMIT_IAC_2000mA    tr("Предел Iac = 2000мА")

#define LIMIT_R_200         tr("Предел R = 200 Ом")
#define LIMIT_R_2K          tr("Предел R = 2 кОм")
#define LIMIT_R_20K         tr("Предел R = 20 кОм")
#define LIMIT_R_200K        tr("Предел R = 200 кОм")
#define LIMIT_R_2M          tr("Предел R = 2 МОм")

#define LIMIT_AUTO          8
//--------------------------------------------------------------------------------
#define CONTROL_UDC_10mV_TEXT           tr("+10.00 мВ")
#define CONTROL_UDC_minus_10mV_TEXT     tr("-10.00 мВ")
#define CONTROL_UDC_170mV_TEXT          tr("+170.00 мВ")
#define CONTROL_UDC_minus_170mV_TEXT    tr("-170.00 мВ")
#define CONTROL_UDC_0_5V_TEXT           tr("+0.5000 В")
#define CONTROL_UDC_minus_0_5V_TEXT     tr("-0.5000 В")
#define CONTROL_UDC_1V_TEXT             tr("+1.0000 В")
#define CONTROL_UDC_1_5V_TEXT           tr("+1.5000 В")
#define CONTROL_UDC_1_7V_TEXT           tr("+1.7000 В")
#define CONTROL_UDC_minus_1_7V_TEXT     tr("-1.7000 В")
#define CONTROL_UDC_17_0V_TEXT          tr("+17.000 В")
#define CONTROL_UDC_170V_TEXT           tr("+170.00 В")
#define CONTROL_UDC_1000V_TEXT          tr("+1000.0 В")
#define CONTROL_UDC_minus_1000V_TEXT    tr("-1000.0 В")

#define CONTROL_UAC_17mV_20kHz_TEXT     tr("17.00 мВ 20 кГц")
#define CONTROL_UAC_190mV_1kHz_TEXT     tr("190.00 мВ 1 кГц")
#define CONTROL_UAC_190mV_50kHz_TEXT    tr("190.00 мВ 50 кГц")
#define CONTROL_UAC_190mV_100kHz_TEXT   tr("190.00 мВ 100 кГц")
#define CONTROL_UAC_0_5V_1kHz_TEXT      tr("0.5000 В 1 кГц")
#define CONTROL_UAC_1V_20Hz_TEXT        tr("1.0000 В 20 Гц")
#define CONTROL_UAC_1V_20kHz_TEXT       tr("1.0000 В 20 кГц")
#define CONTROL_UAC_1V_1kHz_TEXT        tr("1.0000 В 1 кГц")
#define CONTROL_UAC_1V_20kHz_TEXT       tr("1.0000 В 20 кГц")
#define CONTROL_UAC_1V_50kHz_TEXT       tr("1.0000 В 50 кГц")
#define CONTROL_UAC_1V_100kHz_TEXT      tr("1.0000 В 100 кГц")
#define CONTROL_UAC_1_7V_1kHz_TEXT      tr("1.7000 В 1 кГц")
#define CONTROL_UAC_10V_1kHz_TEXT       tr("10.000 В 1 кГц")
#define CONTROL_UAC_10V_20kHz_TEXT      tr("10.000 В 20 кГц")
#define CONTROL_UAC_10V_50kHz_TEXT      tr("10.000 В 50 кГц")
#define CONTROL_UAC_10V_100kHz_TEXT     tr("10.000 В 100 кГц")
#define CONTROL_UAC_100V_1kHz_TEXT      tr("100.00 В 1 кГц")
#define CONTROL_UAC_100V_20kHz_TEXT     tr("100.00 В 20 кГц")
#define CONTROL_UAC_100V_100kHz_TEXT    tr("100.00 В 100 кГц")
#define CONTROL_UAC_170V_1kHz_TEXT      tr("170.00 В 1 кГц")
#define CONTROL_UAC_400V_1kHz_TEXT      tr("400 В 1 кГц")

#define CONTROL_IDC_0_5mkA_TEXT         tr("0.5 мкА")
#define CONTROL_IDC_100mkA_TEXT         tr("+100.00 мкА")
#define CONTROL_IDC_minus_100mkA_TEXT   tr("-100.00 мкА")
#define CONTROL_IDC_1mA_TEXT            tr("+1.0000 мА")
#define CONTROL_IDC_minus_1mA_TEXT      tr("-1.0000 мА")
#define CONTROL_IDC_10mA_TEXT           tr("+10.000 мА")
#define CONTROL_IDC_minus_10mA_TEXT     tr("-10.000 мА")
#define CONTROL_IDC_100mA_TEXT          tr("+100.00 мА")
#define CONTROL_IDC_1000mA_TEXT         tr("+1000.0 мА")
#define CONTROL_IDC_minus_1000mA_TEXT   tr("-1000.0 мА")
#define CONTROL_IDC_19A_TEXT            tr("+19.000 А")
#define CONTROL_IDC_minus_19A_TEXT      tr("-19.000 А")

#define CONTROL_IAC_1mA_1_kHz_TEXT      tr("1 мА 1кГц")
#define CONTROL_IAC_10mA_1_kHz_TEXT     tr("10 мА 1кГц")
#define CONTROL_IAC_100mA_1_kHz_TEXT    tr("100 мА 1кГц")
#define CONTROL_IAC_1000mA_1_kHz_TEXT   tr("1000 мА 1кГц")
#define CONTROL_IAC_10A_1_kHz_TEXT      tr("10.000 А 1кГц")

#define CONTROL_R_0_Om_TEXT             tr("0 Ом")
#define CONTROL_R_10_Om_TEXT            tr("10.00 Ом")
#define CONTROL_R_100_Om_TEXT           tr("100.00 Ом")
#define CONTROL_R_190_Om_TEXT           tr("190.00 Ом")
#define CONTROL_R_1_kOm_TEXT            tr("1.0000 кОм")
#define CONTROL_R_1_9_kOm_TEXT          tr("1.9000 кОм")
#define CONTROL_R_10_kOm_TEXT           tr("10.000 кОм")
#define CONTROL_R_100_kOm_TEXT          tr("100.00 кОм")
#define CONTROL_R_1_MOm_TEXT            tr("1.0000 МОм")

#define CONTROL_C_0_pF_TEXT             tr("0,00 пФ")
#define CONTROL_C_100_pF_TEXT           tr("100,00 пФ")
#define CONTROL_C_1000_pF_TEXT          tr("1000,0 пФ")
#define CONTROL_C_10000_pF_TEXT         tr("10000 пФ")
#define CONTROL_C_0_1_mkF_TEXT          tr("0.10000 мкФ")
#define CONTROL_C_1_mkF_TEXT            tr("1.0000 мкФ")
#define CONTROL_C_10_mkF_TEXT           tr("10.000 мкФ")
#define CONTROL_C_1000_mkF_TEXT         tr("1000.0 мкФ")

#define CONTROL_L_0_mH_TEXT             tr("0,0000 мГн")
#define CONTROL_L_1_mH_TEXT             tr("1,0000 мГн")
#define CONTROL_L_10_mH_TEXT            tr("10,000 мГн")
#define CONTROL_L_100_mH_TEXT           tr("100,00 мГн")

//повышенное напряжение
#define CONTROL_HI_UDC_1V_TEXT          tr("DCV + 1.0000 В")
#define CONTROL_HI_UDC_10V_TEXT         tr("DCV + 10.0000 В")
#define CONTROL_HI_UAC_1V_1kHz_TEXT     tr("ACV 1.0000 В 1 кГц")
#define CONTROL_HI_UAC_10V_1kHz_TEXT    tr("ACV 10.0000 В 1 кГц")

#define CONTROL_HI_R_1kOm_TEXT          tr("R 1.0000 кОм")
#define CONTROL_HI_R_100kOm_TEXT        tr("R 100.00 кОм")

#define CONTROL_HI_IDC_10A_TEXT         tr("DCI + 10.000 А")
#define CONTROL_HI_FREQ_1MHz_TEXT       tr("Frq 1.0000 МГц")
#define CONTROL_HI_C_1mkF_TEXT          tr("Cap 1.0000 мкФ")
#define CONTROL_HI_L_10mH_TEXT          tr("Ind 10.000 мГн")

//пониженное напряжение
#define CONTROL_LO_UDC_1V_TEXT          tr("DCV + 1.0000 В")
#define CONTROL_LO_UDC_10V_TEXT         tr("DCV + 10.0000 В")
#define CONTROL_LO_UAC_1V_1kHz_TEXT     tr("ACV 1.0000 В 1 кГц")
#define CONTROL_LO_UAC_10V_1kHz_TEXT    tr("ACV 10.0000 В 1 кГц")

#define CONTROL_LO_R_1kOm_TEXT          tr("R 1.0000 кОм")
#define CONTROL_LO_R_100kOm_TEXT        tr("R 100.00 кОм")

#define CONTROL_LO_IDC_10A_TEXT         tr("DCI + 10.000 А")
#define CONTROL_LO_FREQ_1MHz_TEXT       tr("Frq 1.0000 МГц")
#define CONTROL_LO_C_1mkF_TEXT          tr("Cap 1.0000 мкФ")
#define CONTROL_LO_L_10mH_TEXT          tr("Ind 10.000 мГн")

//--------------------------------------------------------------------------------
#define V786_2_NAME tr("Мультиметр В7-86/2")
//--------------------------------------------------------------------------------
#define TIME_TRANSCEIVER        5000
#define MAX_TIME_TRANSCEIVER    65535
#define MIN_TIME_TRANSCEIVER    100
//--------------------------------------------------------------------------------
#define ORGNAME "Work"
//--------------------------------------------------------------------------------
#ifdef CALIBRATION_WIRED
    #define APPNAME "Калибровка B7-86 (проводная)"
#else
    #define APPNAME "Калибровка B7-86"
#endif
//--------------------------------------------------------------------------------
#define REPORT_NAME     "report/multimeter_%1_%2"
#define REPORT_NAME_PDF "%1/report/report_%2_%3.pdf"
//--------------------------------------------------------------------------------
#define MAX_TIMEOUT_MULTIMETER_CALIBRATION  10000
#define MAX_TIMEOUT_MULTIMETER_CONTROL_UI   10000
#define MAX_TIMEOUT_MULTIMETER_CONTROL_RLC  20000
//--------------------------------------------------------------------------------
#ifdef CALIBRATION_WIRED
    #define FIXED_ADDRESS   0
    #define FIXED_CHANNEL   0
#endif
//--------------------------------------------------------------------------------
#define ICON_PROGRAMM   ":/programm/pda_blue.png"
//--------------------------------------------------------------------------------
#endif
