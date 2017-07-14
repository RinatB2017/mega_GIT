//--------------------------------------------------------------------------------
#ifndef _R4DEFS_H
#define _R4DEFS_H
//--------------------------------------------------------------------------------
#include <QtDebug>
#include <QColor>
//--------------------------------------------------------------------------------
typedef unsigned char uchar;
typedef unsigned short ushort;

typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned long ULONG;
//--------------------------------------------------------------------------------
#define qError( msg ) qWarning() << QString( " *** error: %1 : %2 : %3 : line %4" ).arg( msg ).arg(__FUNCTION__).arg( __FILE__ ).arg( __LINE__ )
//--------------------------------------------------------------------------------
enum InterStatus { Inter_none=0, Inter_ok=1, Inter_err=2, Inter_warn=3 };
enum DeviceStatus { Device_None=0, Device_Ok=1, Device_Warning=2, Device_Error=3 };
//--------------------------------------------------------------------------------
// === VALUES
#define HT              0x09
#define NAK             0x15
#define PAGE_SIZE		960
#define BLOCK_SIZE		32
//--------------------------------------------------------------------------------
// === STRINGS
#define NORM_STRING		"НОРМА"
#define DAMAGE_STRING	"АВАРИЯ"
#define ON_STRING       "ВКЛ"
#define OFF_STRING      "ВЫКЛ"

#define SET_STRING      "УСТАНОВИТЬ"
#define GET_STRING      "ЗАПРОСИТЬ"

#define SET_SHORT       ">>"
#define GET_SHORT       "*"
//--------------------------------------------------------------------------------
#define TYPE_CELL_1         "ЯЧЕЙКА ВВОДА-ВЫВОДА 1 (ЯВ-В-1) # %1"
#define TYPE_CELL_1_SHORT	"ЯВ-В-1 # %1"
#define TYPE_CELL_2         "ЯЧЕЙКА ВЫВОДА 2 (ЯВЫВ-2) # %1"
#define TYPE_CELL_2_SHORT	"ЯВЫВ-2 # %2"
#define TYPE_CELL_3         "ЯЧЕЙКА ВВОДА 1 (ЯВ-1) # %1"
#define TYPE_CELL_3_SHORT	"ЯВ-1 # %1"
//--------------------------------------------------------------------------------
// === COLORS
#define MODULE_HEADER_COLOR    Qt::darkGray
#define FRAME_HEADER_COLOR     Qt::gray
#define MANAGE_UNIT_HEADER_COLOR Qt::gray
//--------------------------------------------------------------------------------
// === ALIGN
#define ALIGN_VH_CENTER         Qt::AlignVCenter | Qt::AlignHCenter
//--------------------------------------------------------------------------------
// === FONT
#define FRAME_POINT_SIZE_1     10
#define FRAME_POINT_SIZE_2     15
#define FRAME_POINT_SIZE_3     20
//--------------------------------------------------------------------------------
#define EDIT_POINT_SIZE        30
//--------------------------------------------------------------------------------
// === STYLES
#define LABEL_STYLE             QFrame::Box | QFrame::Sunken
#define FRAME_STYLE             QFrame::StyledPanel | QFrame::Plain
#define MANAGE_UNIT_STYLE       QFrame::Box | QFrame::Raised
#define INDICATOR_STYLE         QFrame::StyledPanel | QFrame::Plain
//--------------------------------------------------------------------------------
// === ABOUT SIZE
#define MAX_MODULE_WIDTH            800
#define MAX_MODULE_HEIGHT           600
#define FIXED_HEADER_HEIGHT         50
#define FIXED_LABEL_HEIGHT          25
#define FIXED_STATUS_LABEL_HEIGHT	25
#define MIN_NAV_BUTTON_WIDTH		75
#define SET_GET_BUTTON_WIDTH		50
#define HEIGHT_BOTTOM_FRAME         50
#define MANAGE_UNIT_HEADER_WIDTH	300
#define MANAGE_UNIT_HEADER_HEIGHT	25
//--------------------------------------------------------------------------------
// === RETURN VALUES
#define RET_ERROR			-1
#define RET_OK				0
//--------------------------------------------------------------------------------
// === ERROR MESSAGES
#define ERROR_MESSAGE_1			"NULL POINTER REVEALED"
#define ERROR_MESSAGE_2(val)	"VALUE #val IS OUT OF RANGE"
#define ERROR_MESSAGE_3			"UNEXPECTED CASE"
#define ERROR_MESSAGE_4			"TYPECAST ERROR"
//--------------------------------------------------------------------------------
#define NULL_POINTER_ERROR		"NULL POINTER REVEALED"
//--------------------------------------------------------------------------------
enum KIND_PU { PU_ARM_DS = 0,
               PU_ARM_S = 1,
               PU_ARM_OS = 2
             };
//--------------------------------------------------------------------------------
// === PU BUTTONS
enum PuBtn {
    // --- кнопки корреспондентов
    Btn_S1 = 1,
    Btn_S2 = 2,
    Btn_S3 = 3,
    Btn_S4 = 4,
    Btn_S5 = 5,
    Btn_S6 = 6,
    Btn_S7 = 7,
    Btn_S8 = 8,

    Btn_S9 =  9,
    Btn_S10 = 10,
    Btn_S11 = 11,
    Btn_S12 = 12,
    Btn_S13 = 13,
    Btn_S14 = 14,
    Btn_S15 = 15,
    Btn_S16 = 16,

    Btn_S17 = 17,
    Btn_S18 = 18,
    Btn_S19 = 19,
    Btn_S20 = 20,
    Btn_S21 = 21,
    Btn_S22 = 22,
    Btn_S23 = 23,
    Btn_S24 = 24,

    Btn_S25 = 25,
    Btn_S26 = 26,
    Btn_S27 = 27,
    Btn_S28 = 28,
    // --- кнопки наборного поля
    Btn_NS1 = 31,
    Btn_NS2 = 32,
    Btn_NS3 = 33,
    Btn_NS4 = 34,
    Btn_NS5 = 35,
    Btn_NS6 = 36,
    Btn_NS7 = 37,
    Btn_NS8 = 38,
    Btn_NS9 = 39,

    Btn_NS10 = 40,
    Btn_NS11 = 41,
    Btn_NS12 = 42,
    Btn_NS13 = 43,
    Btn_NS14 = 44,
    Btn_NS15 = 45,
    Btn_NS16 = 46
};
//--------------------------------------------------------------------------------
#define MinPuBtnIndex	1
#define MaxPuBtnIndex	46
//--------------------------------------------------------------------------------
enum BtnEvent {
    Btn_Event_None = -1,
    Btn_Event_Down = 0,
    Btn_Event_Up = 1
};
//--------------------------------------------------------------------------------
const QColor my_green = QColor(80, 200, 120);
const QColor my_red = QColor(0xf3, 0x47, 0x23);
const QColor my_yellow = QColor(0xff, 0xd7, 0x00);
const QColor my_active_color = QColor(80, 170, 255);
//--------------------------------------------------------------------------------
#endif
