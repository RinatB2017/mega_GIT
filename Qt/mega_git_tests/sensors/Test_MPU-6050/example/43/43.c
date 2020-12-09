
#define F_CPU 8000000L

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <math.h>

#include "lcd_lib_2.h"
#include "bcd.h"
#include "i2c.h"

signed char TEMPER_h;
signed char TEMPER_l;
signed long int t;

unsigned char ACCEL_xh;
unsigned char ACCEL_xl;
unsigned char ACCEL_yh;
unsigned char ACCEL_yl;
unsigned char ACCEL_zh;
unsigned char ACCEL_zl;
long int ACCEL_X;
long int ACCEL_Y;
long int ACCEL_Z;

long int ACCEL_XANGLE;
long int ACCEL_YANGLE;
long int ACCEL_ZANGLE;

unsigned char GYRO_xh;
unsigned char GYRO_xl;
unsigned char GYRO_yh;
unsigned char GYRO_yl;
unsigned char GYRO_zh;
unsigned char GYRO_zl;
long int GYRO_X;
long int GYRO_Y;
long int GYRO_Z;

long int GYRO_ANGLE_X=0;
long int GYRO_ANGLE_Y=0;
long int GYRO_ANGLE_Z=0;

unsigned char who;

#define LED1 PB0
#define LED2 PB1
#define LED3 PB2
#define LED4 PB3
#define LED5 PB4
#define LED6 PB5
#define LED_PORT PORTB
#define LED_DDR DDRB

//=======================================================================================
// MPU6050 - гироскоп и акселерометр
//=======================================================================================

#define MPU6050_RA_XG_OFFS_TC 0x00 //[7] PWR_MODE, [6:1] XG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU6050_RA_YG_OFFS_TC 0x01 //[7] PWR_MODE, [6:1] YG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU6050_RA_ZG_OFFS_TC 0x02 //[7] PWR_MODE, [6:1] ZG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU6050_RA_X_FINE_GAIN 0x03 //[7:0] X_FINE_GAIN
#define MPU6050_RA_Y_FINE_GAIN 0x04 //[7:0] Y_FINE_GAIN
#define MPU6050_RA_Z_FINE_GAIN 0x05 //[7:0] Z_FINE_GAIN
#define MPU6050_RA_XA_OFFS_H 0x06 //[15:0] XA_OFFS
#define MPU6050_RA_XA_OFFS_L_TC 0x07
#define MPU6050_RA_YA_OFFS_H 0x08 //[15:0] YA_OFFS
#define MPU6050_RA_YA_OFFS_L_TC 0x09
#define MPU6050_RA_ZA_OFFS_H 0x0A //[15:0] ZA_OFFS
#define MPU6050_RA_ZA_OFFS_L_TC 0x0B
#define MPU6050_RA_XG_OFFS_USRH 0x13 //[15:0] XG_OFFS_USR
#define MPU6050_RA_XG_OFFS_USRL 0x14
#define MPU6050_RA_YG_OFFS_USRH 0x15 //[15:0] YG_OFFS_USR
#define MPU6050_RA_YG_OFFS_USRL 0x16
#define MPU6050_RA_ZG_OFFS_USRH 0x17 //[15:0] ZG_OFFS_USR
#define MPU6050_RA_ZG_OFFS_USRL 0x18
#define MPU6050_RA_SMPLRT_DIV 0x19
#define MPU6050_RA_CONFIG 0x1A
#define MPU6050_RA_GYRO_CONFIG 0x1B
#define MPU6050_RA_ACCEL_CONFIG 0x1C
#define MPU6050_RA_FF_THR 0x1D
#define MPU6050_RA_FF_DUR 0x1E
#define MPU6050_RA_MOT_THR 0x1F
#define MPU6050_RA_MOT_DUR 0x20
#define MPU6050_RA_ZRMOT_THR 0x21
#define MPU6050_RA_ZRMOT_DUR 0x22
#define MPU6050_RA_FIFO_EN 0x23
#define MPU6050_RA_I2C_MST_CTRL 0x24
#define MPU6050_RA_I2C_SLV0_ADDR 0x25
#define MPU6050_RA_I2C_SLV0_REG 0x26
#define MPU6050_RA_I2C_SLV0_CTRL 0x27
#define MPU6050_RA_I2C_SLV1_ADDR 0x28
#define MPU6050_RA_I2C_SLV1_REG 0x29
#define MPU6050_RA_I2C_SLV1_CTRL 0x2A
#define MPU6050_RA_I2C_SLV2_ADDR 0x2B
#define MPU6050_RA_I2C_SLV2_REG 0x2C
#define MPU6050_RA_I2C_SLV2_CTRL 0x2D
#define MPU6050_RA_I2C_SLV3_ADDR 0x2E
#define MPU6050_RA_I2C_SLV3_REG 0x2F
#define MPU6050_RA_I2C_SLV3_CTRL 0x30
#define MPU6050_RA_I2C_SLV4_ADDR 0x31
#define MPU6050_RA_I2C_SLV4_REG 0x32
#define MPU6050_RA_I2C_SLV4_DO 0x33
#define MPU6050_RA_I2C_SLV4_CTRL 0x34
#define MPU6050_RA_I2C_SLV4_DI 0x35
#define MPU6050_RA_I2C_MST_STATUS 0x36
#define MPU6050_RA_INT_PIN_CFG 0x37
#define MPU6050_RA_INT_ENABLE 0x38
#define MPU6050_RA_DMP_INT_STATUS 0x39
#define MPU6050_RA_INT_STATUS 0x3A
#define MPU6050_RA_ACCEL_XOUT_H 0x3B
#define MPU6050_RA_ACCEL_XOUT_L 0x3C
#define MPU6050_RA_ACCEL_YOUT_H 0x3D
#define MPU6050_RA_ACCEL_YOUT_L 0x3E
#define MPU6050_RA_ACCEL_ZOUT_H 0x3F
#define MPU6050_RA_ACCEL_ZOUT_L 0x40
#define MPU6050_RA_TEMP_OUT_H 0x41
#define MPU6050_RA_TEMP_OUT_L 0x42
#define MPU6050_RA_GYRO_XOUT_H 0x43
#define MPU6050_RA_GYRO_XOUT_L 0x44
#define MPU6050_RA_GYRO_YOUT_H 0x45
#define MPU6050_RA_GYRO_YOUT_L 0x46
#define MPU6050_RA_GYRO_ZOUT_H 0x47
#define MPU6050_RA_GYRO_ZOUT_L 0x48
#define MPU6050_RA_EXT_SENS_DATA_00 0x49
#define MPU6050_RA_EXT_SENS_DATA_01 0x4A
#define MPU6050_RA_EXT_SENS_DATA_02 0x4B
#define MPU6050_RA_EXT_SENS_DATA_03 0x4C
#define MPU6050_RA_EXT_SENS_DATA_04 0x4D
#define MPU6050_RA_EXT_SENS_DATA_05 0x4E
#define MPU6050_RA_EXT_SENS_DATA_06 0x4F
#define MPU6050_RA_EXT_SENS_DATA_07 0x50
#define MPU6050_RA_EXT_SENS_DATA_08 0x51
#define MPU6050_RA_EXT_SENS_DATA_09 0x52
#define MPU6050_RA_EXT_SENS_DATA_10 0x53
#define MPU6050_RA_EXT_SENS_DATA_11 0x54
#define MPU6050_RA_EXT_SENS_DATA_12 0x55
#define MPU6050_RA_EXT_SENS_DATA_13 0x56
#define MPU6050_RA_EXT_SENS_DATA_14 0x57
#define MPU6050_RA_EXT_SENS_DATA_15 0x58
#define MPU6050_RA_EXT_SENS_DATA_16 0x59
#define MPU6050_RA_EXT_SENS_DATA_17 0x5A
#define MPU6050_RA_EXT_SENS_DATA_18 0x5B
#define MPU6050_RA_EXT_SENS_DATA_19 0x5C
#define MPU6050_RA_EXT_SENS_DATA_20 0x5D
#define MPU6050_RA_EXT_SENS_DATA_21 0x5E
#define MPU6050_RA_EXT_SENS_DATA_22 0x5F
#define MPU6050_RA_EXT_SENS_DATA_23 0x60
#define MPU6050_RA_MOT_DETECT_STATUS 0x61
#define MPU6050_RA_I2C_SLV0_DO 0x63
#define MPU6050_RA_I2C_SLV1_DO 0x64
#define MPU6050_RA_I2C_SLV2_DO 0x65
#define MPU6050_RA_I2C_SLV3_DO 0x66
#define MPU6050_RA_I2C_MST_DELAY_CTRL 0x67
#define MPU6050_RA_SIGNAL_PATH_RESET 0x68
#define MPU6050_RA_MOT_DETECT_CTRL 0x69
#define MPU6050_RA_USER_CTRL 0x6A
#define MPU6050_RA_PWR_MGMT_1 0x6B
#define MPU6050_RA_PWR_MGMT_2 0x6C
#define MPU6050_RA_BANK_SEL 0x6D
#define MPU6050_RA_MEM_START_ADDR 0x6E
#define MPU6050_RA_MEM_R_W 0x6F
#define MPU6050_RA_DMP_CFG_1 0x70
#define MPU6050_RA_DMP_CFG_2 0x71
#define MPU6050_RA_FIFO_COUNTH 0x72
#define MPU6050_RA_FIFO_COUNTL 0x73
#define MPU6050_RA_FIFO_R_W 0x74
#define MPU6050_RA_WHO_AM_I 0x75

#define MPU6050_R 0b11010001
#define MPU6050_W 0b11010000

//=======================================================================================
// запись I2C
//=======================================================================================
void I2C_write (unsigned char reg, unsigned char value) 
{
    i2c_start_cond();
    i2c_send_byte(MPU6050_W);
    i2c_send_byte(reg);
    i2c_send_byte(value);
    i2c_stop_cond();
}

//=======================================================================================
// инициализация, конфигурация
//=======================================================================================
void Init_6050 (void) 
{
    //Sets sample rate to 8000/1+7 = 1000Hz
    I2C_write (MPU6050_RA_SMPLRT_DIV, 0x07);

    //Disable FSync, 256Hz DLPF
    I2C_write (MPU6050_RA_CONFIG, 0x00);

    //Disable gyro self tests, scale of 2000 degrees/s
    I2C_write (MPU6050_RA_GYRO_CONFIG, 0b00011000);

    //Disable accel self tests, scale of +-2g, no DHPF
    I2C_write (MPU6050_RA_ACCEL_CONFIG, 0x00);

    //Freefall threshold of |0mg|
    I2C_write (MPU6050_RA_FF_THR, 0x00);

    //Freefall duration limit of 0
    I2C_write (MPU6050_RA_FF_DUR, 0x00);

    //Motion threshold of 0mg
    I2C_write (MPU6050_RA_MOT_THR, 0x00);

    //Motion duration of 0s
    I2C_write (MPU6050_RA_MOT_DUR, 0x00);

    //Zero motion threshold
    I2C_write (MPU6050_RA_ZRMOT_THR, 0x00);

    //Zero motion duration threshold
    I2C_write (MPU6050_RA_ZRMOT_DUR, 0x00);

    //Disable sensor output to FIFO buffer
    I2C_write (MPU6050_RA_FIFO_EN, 0x00);

    
    //AUX I2C setup
    //Sets AUX I2C to single master control, plus other config
    I2C_write (MPU6050_RA_I2C_MST_CTRL, 0x00);
    //Setup AUX I2C slaves
    I2C_write (MPU6050_RA_I2C_SLV0_ADDR, 0x00);
    I2C_write (MPU6050_RA_I2C_SLV0_REG, 0x00);
    I2C_write (MPU6050_RA_I2C_SLV0_CTRL, 0x00);
    I2C_write (MPU6050_RA_I2C_SLV1_ADDR, 0x00);
    I2C_write (MPU6050_RA_I2C_SLV1_REG, 0x00);
    I2C_write (MPU6050_RA_I2C_SLV1_CTRL, 0x00);
    I2C_write (MPU6050_RA_I2C_SLV2_ADDR, 0x00);
    I2C_write (MPU6050_RA_I2C_SLV2_REG, 0x00);
    I2C_write (MPU6050_RA_I2C_SLV2_CTRL, 0x00);
    I2C_write (MPU6050_RA_I2C_SLV3_ADDR, 0x00);
    I2C_write (MPU6050_RA_I2C_SLV3_REG, 0x00);
    I2C_write (MPU6050_RA_I2C_SLV3_CTRL, 0x00);
    I2C_write (MPU6050_RA_I2C_SLV4_ADDR, 0x00);
    I2C_write (MPU6050_RA_I2C_SLV4_REG, 0x00);
    I2C_write (MPU6050_RA_I2C_SLV4_DO, 0x00);
    I2C_write (MPU6050_RA_I2C_SLV4_CTRL, 0x00);
    I2C_write (MPU6050_RA_I2C_SLV4_DI, 0x00);
    //MPU6050_RA_I2C_MST_STATUS //Read-only
    //Setup INT pin and AUX I2C pass through
    I2C_write (MPU6050_RA_INT_PIN_CFG, 0x00);
    //Enable data ready interrupt
    I2C_write (MPU6050_RA_INT_ENABLE, 0x00);


    //Slave out, dont care
    I2C_write (MPU6050_RA_I2C_SLV0_DO, 0x00);
    I2C_write (MPU6050_RA_I2C_SLV1_DO, 0x00);
    I2C_write (MPU6050_RA_I2C_SLV2_DO, 0x00);
    I2C_write (MPU6050_RA_I2C_SLV3_DO, 0x00);
    //More slave config
    I2C_write (MPU6050_RA_I2C_MST_DELAY_CTRL, 0x00);
    //Reset sensor signal paths
    I2C_write (MPU6050_RA_SIGNAL_PATH_RESET, 0x00);
    //Motion detection control
    I2C_write (MPU6050_RA_MOT_DETECT_CTRL, 0x00);
    //Disables FIFO, AUX I2C, FIFO and I2C reset bits to 0
    I2C_write (MPU6050_RA_USER_CTRL, 0x00);
    //Sets clock source to gyro reference w/ PLL
    I2C_write (MPU6050_RA_PWR_MGMT_1, 0b00000010);
    //Controls frequency of wakeups in accel low power mode plus the sensor standby modes
    I2C_write (MPU6050_RA_PWR_MGMT_2, 0x00);


    /*
    i2c_start_cond();               // запуск i2c
    i2c_send_byte(MPU6050_W);       // передача адреса устройства, режим записи
    i2c_send_byte(0x19);            // передача адреса памяти
    i2c_send_byte(0b00000000);      // Register 25
    i2c_send_byte(0b00000110);      // Register 26
    i2c_send_byte(0b11110000);      // Register 27
    i2c_send_byte(0b11110000);      // Register 28
    i2c_stop_cond();                // остановка i2c

    i2c_start_cond();               // запуск i2c
    i2c_send_byte(MPU6050_W);       // передача адреса устройства, режим записи
    i2c_send_byte(0x1F);            // передача адреса памяти
    i2c_send_byte(250);             // Register 31
    i2c_stop_cond();                // остановка i2c

    i2c_start_cond();               // запуск i2c
    i2c_send_byte(MPU6050_W);       // передача адреса устройства, режим записи
    i2c_send_byte(0x6A);            // передача адреса памяти
    i2c_send_byte(0b00000000);      // Register 106
    i2c_send_byte(0b00100000);      // Register 107
    i2c_send_byte(0b01000000);      // Register 108
    i2c_stop_cond();                // остановка i2c
*/
}

//=======================================================================================
// акселерометр
//=======================================================================================
void GET_ACCEL () 
{
    i2c_start_cond();               // запуск i2c
    i2c_send_byte(MPU6050_W);       // передача адреса устройства, режим записи
    i2c_send_byte(0x3B);            // передача адреса памяти
    i2c_stop_cond();                // остановка i2c

    i2c_start_cond();               // запуск i2c
    i2c_send_byte(MPU6050_R);       // передача адреса устройства, режим чтения
    ACCEL_xh = i2c_get_byte(0);     // читаем данные
    ACCEL_xl = i2c_get_byte(0);
    ACCEL_yh = i2c_get_byte(0);
    ACCEL_yl = i2c_get_byte(0);
    ACCEL_zh = i2c_get_byte(0);
    ACCEL_zl = i2c_get_byte(1);
    i2c_stop_cond();                // остановка i2c

    ACCEL_X = ACCEL_xh*256 + ACCEL_xl;
    ACCEL_Y = ACCEL_yh*256 + ACCEL_yl;
    ACCEL_Z = ACCEL_zh*256 + ACCEL_zl;
}
//=======================================================================================
// гироскоп
//=======================================================================================
void GET_GYRO () 
{
    i2c_start_cond();               // запуск i2c
    i2c_send_byte(MPU6050_W);       // передача адреса устройства, режим записи
    i2c_send_byte(0x43);            // передача адреса памяти
    i2c_stop_cond();                // остановка i2c

    i2c_start_cond();               // запуск i2c
    i2c_send_byte(MPU6050_R);       // передача адреса устройства, режим чтения
    GYRO_xh = i2c_get_byte(0);      // читаем данные
    GYRO_xl = i2c_get_byte(0);
    GYRO_yh = i2c_get_byte(0);
    GYRO_yl = i2c_get_byte(0);
    GYRO_zh = i2c_get_byte(0);
    GYRO_zl = i2c_get_byte(1);
    i2c_stop_cond();                // остановка i2c

    GYRO_X = GYRO_xh*256 + GYRO_xl;
    GYRO_Y = GYRO_yh*256 + GYRO_yl;
    GYRO_Z = GYRO_zh*256 + GYRO_zl;
}

//=======================================================================================
// температура
//=======================================================================================
void GET_TEMPER () 
{
    i2c_start_cond();               // запуск i2c
    i2c_send_byte(MPU6050_W);       // передача адреса устройства, режим записи
    i2c_send_byte(0x41);            // передача адреса памяти
    i2c_stop_cond();                // остановка i2c

    i2c_start_cond();               // запуск i2c
    i2c_send_byte(MPU6050_R);       // передача адреса устройства, режим чтения
    TEMPER_h = i2c_get_byte(0);     // читаем данные
    TEMPER_l = i2c_get_byte(1);
    i2c_stop_cond();                // остановка i2c
}

//=======================================================================================
// идентификатор (должно быть значение 104)
//=======================================================================================
void WHO_AM_I () 
{
    i2c_start_cond();               // запуск i2c
    i2c_send_byte(MPU6050_W);       // передача адреса устройства, режим записи
    i2c_send_byte(0x75);            // передача адреса памяти
    i2c_stop_cond();                // остановка i2c

    i2c_start_cond();               // запуск i2c
    i2c_send_byte(MPU6050_R);       // передача адреса устройства, режим чтения
    who = i2c_get_byte(1);
    i2c_stop_cond();                // остановка i2c
}


//=======================================================================================
int main (void)
{
    // порт
    DDRB  = (1<<LED1)|(1<<LED2)|(1<<LED3)|(1<<LED4)|(1<<LED5)|(1<<LED6);
    PORTB = (0<<LED1)|(0<<LED2)|(0<<LED3)|(0<<LED4)|(0<<LED5)|(0<<LED6);

    // инициализации
    LCD_Init();
    i2c_init();
    Init_6050();

    LCD_Clear();

    //WHO_AM_I();
    //LCD_Goto(0,3);
    //BCD_4IntLcd(who);

    while (1)
    {
        GET_TEMPER();
        t=TEMPER_h*256;
        t=t+TEMPER_l+12412;
        t=t/340;
        //t=t*100;
        //t=t+3653;
        //t=t/100;
        LCD_Goto(0,2);
        LCD_WriteData('t');
        LCD_WriteData('=');
        BCD_2Lcd(t);


        GET_ACCEL();

        ACCEL_XANGLE = 57.295*atan((float)-ACCEL_X/ sqrt(pow((float)ACCEL_Y,2)+pow((float)ACCEL_Z,2)));
        ACCEL_YANGLE = 57.295*atan((float)-ACCEL_Y/ sqrt(pow((float)ACCEL_X,2)+pow((float)ACCEL_Z,2)));
        ACCEL_ZANGLE = 57.295*atan((float)-ACCEL_Z/ sqrt(pow((float)ACCEL_X,2)+pow((float)ACCEL_Y,2)));


        if(ACCEL_XANGLE>=-90  && ACCEL_XANGLE<-60) {LED_PORT |=(1<<LED1); LED_PORT &=~(1<<LED2);}
        if(ACCEL_XANGLE>=-60 && ACCEL_XANGLE<-30) {LED_PORT |=(1<<LED2); LED_PORT &=~(1<<LED3);}
        if(ACCEL_XANGLE>=-30 && ACCEL_XANGLE<0) {LED_PORT |=(1<<LED3); LED_PORT &=~(1<<LED4);}
        if(ACCEL_XANGLE>=0 && ACCEL_XANGLE<30) {LED_PORT |=(1<<LED4); LED_PORT &=~(1<<LED5);}
        if(ACCEL_XANGLE>=30 && ACCEL_XANGLE<60) {LED_PORT |=(1<<LED5); LED_PORT &=~(1<<LED6);}
        if(ACCEL_XANGLE>=60 && ACCEL_XANGLE<90) {LED_PORT |=(1<<LED6); }


        LCD_Goto(0,0);
        LCD_WriteData('A');
        if (ACCEL_X<0) {LCD_Goto(2,0); LCD_WriteData('-'); ACCEL_X=-(ACCEL_X);} else {LCD_Goto(2,0); LCD_WriteData('+');}
        if (ACCEL_Y<0) {LCD_Goto(8,0); LCD_WriteData('-'); ACCEL_Y=-(ACCEL_Y);} else {LCD_Goto(8,0); LCD_WriteData('+');}
        if (ACCEL_Z<0) {LCD_Goto(14,0); LCD_WriteData('-'); ACCEL_Z=-(ACCEL_Z);} else {LCD_Goto(14,0); LCD_WriteData('+');}
        LCD_Goto(3,0);
        BCD_4IntLcd(ACCEL_X/8);
        LCD_WriteData(':');
        LCD_Goto(9,0);
        BCD_4IntLcd(ACCEL_Y/8);
        LCD_WriteData(':');
        LCD_Goto(15,0);
        BCD_4IntLcd(ACCEL_Z/8);

        GET_GYRO();

        GYRO_ANGLE_Z=GYRO_ANGLE_Z+GYRO_Z/22;
        LCD_Goto(16,3);
        BCD_3IntLcd(GYRO_ANGLE_Z/10);


        LCD_Goto(0,1);
        LCD_WriteData('G');
        if (GYRO_X<0) {
            LCD_Goto(2,1); LCD_WriteData('-');
            GYRO_X=-(GYRO_X);
            
        } else {
            LCD_Goto(2,1);
            LCD_WriteData('+');
        }
        if (GYRO_Y<0)
        {
            LCD_Goto(8,1);
            LCD_WriteData('-');
            GYRO_Y=-(GYRO_Y);
        } else {
            LCD_Goto(8,1);
            LCD_WriteData('+');
        }
        if (GYRO_Z<0)
        {
            LCD_Goto(14,1);
            LCD_WriteData('-');
            GYRO_Z=-(GYRO_Z);
        } else {
            LCD_Goto(14,1);
            LCD_WriteData('+');
            
        }
        LCD_Goto(3,1);
        BCD_4IntLcd(GYRO_X/10);
        LCD_WriteData(':');
        LCD_Goto(9,1);
        BCD_4IntLcd(GYRO_Y/10);
        LCD_WriteData(':');
        LCD_Goto(15,1);
        BCD_4IntLcd(GYRO_Z/10);


        if(ACCEL_XANGLE<0)
        {
            LCD_Goto(2,3);
            LCD_WriteData('-');
            ACCEL_XANGLE=-(ACCEL_XANGLE);
        } else {
            LCD_Goto(2,3);
            LCD_WriteData('+');
        }
        if(ACCEL_YANGLE<0)
        {
            LCD_Goto(6,3);
            LCD_WriteData('-');
            ACCEL_YANGLE=-(ACCEL_YANGLE);
        } else {
            LCD_Goto(6,3);
            LCD_WriteData('+');
        }
        if(ACCEL_ZANGLE<0)
        {
            LCD_Goto(10,3);
            LCD_WriteData('-');
            ACCEL_ZANGLE=-(ACCEL_ZANGLE);
        } else {
            LCD_Goto(10,3);
            LCD_WriteData('+');
        }


        LCD_Goto(0,3);
        LCD_WriteData('<');

        LCD_Goto(3,3);
        BCD_2Lcd(ACCEL_XANGLE);
        LCD_WriteData(':');
        LCD_Goto(7,3);
        BCD_2Lcd(ACCEL_YANGLE);
        LCD_WriteData(':');
        LCD_Goto(11,3);
        BCD_2Lcd(ACCEL_ZANGLE);
        _delay_ms(50);
    } // end while(1)
} // end main
