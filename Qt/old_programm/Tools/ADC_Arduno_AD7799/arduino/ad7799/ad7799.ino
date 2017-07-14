#include <SD.h>

/*
 AD7799 sketch
 Arduino Uno
 */
//------------------------------------------------------------------------------------------------
#include <SPI.h>
//------------------------------------------------------------------------------------------------
#define AD7799_CS_LOW    digitalWrite(10,LOW);
#define AD7799_CS_HIGH    digitalWrite(10,HIGH);
//------------------------------------------------------------------------------------------------
/*AD7799 Registers*/
#define AD7799_REG_COMM      0 /* Communications Register(WO, 8-bit) */
#define AD7799_REG_STAT      0 /* Status Register        (RO, 8-bit) */
#define AD7799_REG_MODE      1 /* Mode Register          (RW, 16-bit */
#define AD7799_REG_CONF      2 /* Configuration Register (RW, 16-bit)*/
#define AD7799_REG_DATA      3 /* Data Register          (RO, 16-/24-bit) */
#define AD7799_REG_ID        4 /* ID Register            (RO, 8-bit) */
#define AD7799_REG_IO        5 /* IO Register            (RO, 8-bit) */
#define AD7799_REG_OFFSET    6 /* Offset Register        (RW, 24-bit */
#define AD7799_REG_FULLSALE  7 /* Full-Scale Register    (RW, 24-bit */

/* Communications Register Bit Designations (AD7799_REG_COMM) */
#define AD7799_COMM_WEN      (1 << 7)            /* Write Enable */
#define AD7799_COMM_WRITE    (0 << 6)            /* Write Operation */
#define AD7799_COMM_READ     (1 << 6)            /* Read Operation */
#define AD7799_COMM_ADDR(x)  (((x) & 0x7) << 3)  /* Register Address */
#define AD7799_COMM_CREAD    (1 << 2)            /* Continuous Read of Data Register */

/* Status Register Bit Designations (AD7799_REG_STAT) */
#define AD7799_STAT_RDY                (1 << 7) /* Ready */
#define AD7799_STAT_ERR                (1 << 6) /* Error (Overrange, Underrange) */
#define AD7799_STAT_CH3                (1 << 2) /* Channel 3 */
#define AD7799_STAT_CH2                (1 << 1) /* Channel 2 */
#define AD7799_STAT_CH1                (1 << 0) /* Channel 1 */

/* Mode Register Bit Designations (AD7799_REG_MODE) */
#define AD7799_MODE_SEL(x)  (((x) & 0x7) << 13)  /* Operation Mode Select */
#define AD7799_MODE_PSW(x)  (1 << 12)            /* Power Switch Control Bit */        
#define AD7799_MODE_RATE(x) ((x) & 0xF)          /* Filter Update Rate Select */

/* AD7799_MODE_SEL(x) options */
#define AD7799_MODE_CONT         0 /* Continuous Conversion Mode */
#define AD7799_MODE_SINGLE       1 /* Single Conversion Mode */
#define AD7799_MODE_IDLE         2 /* Idle Mode */
#define AD7799_MODE_PWRDN        3 /* Power-Down Mode */
#define AD7799_MODE_CAL_INT_ZERO 4 /* Internal Zero-Scale Calibration */
#define AD7799_MODE_CAL_INT_FULL 5 /* Internal Full-Scale Calibration */
#define AD7799_MODE_CAL_SYS_ZERO 6 /* System Zero-Scale Calibration */
#define AD7799_MODE_CAL_SYS_FULL 7 /* System Full-Scale Calibration */

/* Configuration Register Bit Designations (AD7799_REG_CONF) */
#define AD7799_CONF_BO_EN     (1 << 13)           /* Burnout Current Enable */
#define AD7799_CONF_UNIPOLAR  (1 << 12)           /* Unipolar/Bipolar Enable */
#define AD7799_CONF_GAIN(x)   (((x) & 0x7) << 8)  /* Gain Select */
#define AD7799_CONF_REFDET(x) (((x) & 0x1) << 5)  /* Reference detect function */
#define AD7799_CONF_BUF       (1 << 4)            /* Buffered Mode Enable */
#define AD7799_CONF_CHAN(x)   ((x) & 0x7)         /* Channel select */

/* AD7799_CONF_GAIN(x) options */
#define AD7799_GAIN_1       0
#define AD7799_GAIN_2       1
#define AD7799_GAIN_4       2
#define AD7799_GAIN_8       3
#define AD7799_GAIN_16      4
#define AD7799_GAIN_32      5
#define AD7799_GAIN_64      6
#define AD7799_GAIN_128     7

/* AD7799_CONF_REFDET(x) options */
#define AD7799_REFDET_ENA   1        
#define AD7799_REFDET_DIS   0

/* AD7799_CONF_CHAN(x) options */
#define AD7799_CH_AIN1P_AIN1M        0 /* AIN1(+) - AIN1(-) */
#define AD7799_CH_AIN2P_AIN2M        1 /* AIN2(+) - AIN2(-) */
#define AD7799_CH_AIN3P_AIN3M        2 /* AIN3(+) - AIN3(-) */
#define AD7799_CH_AIN1M_AIN1M        3 /* AIN1(-) - AIN1(-) */
#define AD7799_CH_AVDD_MONITOR       7 /* AVDD Monitor */

/* ID Register Bit Designations (AD7799_REG_ID) */
#define AD7799_ID                    0x9
#define AD7799_ID_MASK               0xF

/* IO (Excitation Current Sources) Register Bit Designations (AD7799_REG_IO) */
#define AD7799_IOEN                  (1 << 6)
#define AD7799_IO1(x)                (((x) & 0x1) << 4)
#define AD7799_IO2(x)                (((x) & 0x1) << 5)

#define AD7799_STATUS_RDY       0x80
#define AD7799_STATUS_ERROR     0x40
#define AD7799_STATUS_NOREF     0x20
#define AD7799_STATUS_IS_AD7799 0x08
#define AD7799_STATUS_CHAN_MASK 0x07

extern uint8_t ad7799_init_status;

#define AD7799_STATUS_REG 0x0
#define AD7799_MODE_REG   0x1
#define AD7799_CONFIG_REG 0x2
#define AD7799_DATA_REG   0x3
#define AD7799_ID_REG     0x4
#define AD7799_IO_REG     0x5
#define AD7799_OFFSET_REG 0x6
#define AD7799_SCALE_REG  0x7
#define AD7799_CONF_CHAN(x)          ((x) & 0x7)                         /* Channel select */
//------------------------------------------------------------------------------------------------
#define CS 10
//------------------------------------------------------------------------------------------------
enum { 
  AD7799_CONTINUOUS_CONVERSION_MODE = 0,  AD7799_SINGLE_CONVERSION_MODE,
  AD7799_IDLE_MODE, AD7799_POWERDOWN_MODE, AD7799_INTERNAL_OFFSET_CAL_MODE,
  AD7799_INTERNAL_SCALE_CAL_MODE,  AD7799_SYSTEM_OFFSET_CAL_MODE,
  AD7799_SYSTEM_SCALE_CAL_MODE};

enum { 
  AD7799_470_HZ = 1, AD7799_242_HZ, AD7799_123_HZ, AD7799_62_HZ,
  AD7799_50_HZ, AD7799_39_HZ, AD7799_33_2_HZ, AD7799_19_6_HZ,
  AD7799_16_7_1_HZ, AD7799_16_7_2_HZ, AD7799_12_5_HZ, AD7799_10_HZ,
  AD7799_8_33_HZ, AD7799_6_25_HZ, AD7799_4_17_HZ };

enum { 
  AD7799_1_GAIN, AD7799_2_GAIN, AD7799_4_GAIN, AD7799_8_GAIN,
  AD7799_16_GAIN, AD7799_32_GAIN, AD7799_64_GAIN, AD7799_128_GAIN };

enum { 
  AD7799_AIN1_CHAN, AD7799_AIN2_CHAN, AD7799_AIN3_CHAN,
  AD7799_AIN11_CHAN, AD7799_AVDD_CHAN };
//------------------------------------------------------------------------------------------------
/*******************************************************************************
 * @brief Sends 32 consecutive 1's on SPI in order to reset the part.
 *
 * @param None.
 *
 * @return  None.    
 *******************************************************************************/
void AD7799_Reset(void)
{
  unsigned char dataToSend[5] = { 0x03, 0xff, 0xff, 0xff, 0xff };
  AD7799_CS_LOW;            
  SPI_Write(dataToSend,4);
  AD7799_CS_HIGH;        
}
//------------------------------------------------------------------------------------------------
void  SPI_Write(unsigned char *data,int size)
{
  for(int i=0;i<size;i++) 
  {
    (void) SPI.transfer(data[i]);
  }
}
//------------------------------------------------------------------------------------------------

void  SPI_Read(unsigned char *data,int size)
{
  for(int i=0;i<size;i++) 
  {
    data[i]= SPI.transfer(0xFF);
  }
}
//------------------------------------------------------------------------------------------------
/*******************************************************************************
 * @brief Writes the value to the register
 *
 * @param -  regAddress - The address of the register to write to.
 * @param -  regValue - The value to write to the register.
 * @param -  size - The size of the register to write.
 *
 * @return  None.    
 *******************************************************************************/
void AD7799_SetRegisterValue(unsigned char regAddress, unsigned long regValue, unsigned char size)
{
  unsigned char data[5] = { 0x03, 0x00, 0x00, 0x00, 0x00 };
  data[1] = AD7799_COMM_WRITE |  AD7799_COMM_ADDR(regAddress);
  if(size == 1)
  {
    data[2] = (unsigned char)regValue;
  }
  if(size == 2)
  {
    data[3] = (unsigned char)((regValue & 0x0000FF) >> 0);
    data[2] = (unsigned char)((regValue & 0x00FF00) >> 8);
  }
  if(size == 3)
  {
    data[4] = (unsigned char)((regValue & 0x0000FF) >> 0);
    data[3] = (unsigned char)((regValue & 0x00FF00) >> 8);
    data[2] = (unsigned char)((regValue & 0xFF0000) >> 16);
  }
  AD7799_CS_LOW;            
  SPI_Write(data,(1 + size));
  AD7799_CS_HIGH;
}
//------------------------------------------------------------------------------------------------
unsigned long AD7799_GetRegisterValue(unsigned char regAddress, unsigned char size)
{
  unsigned char data[5] = { 0x03, 0x00, 0x00, 0x00, 0x00 };
  unsigned long receivedData = 0x00;        
  data[1] = AD7799_COMM_READ |  AD7799_COMM_ADDR(regAddress);
  AD7799_CS_LOW;  
  // SPI_Write(data,1);
  SPI_Write(&data[1],1);
  SPI_Read(data,size);
  AD7799_CS_HIGH;
  if(size == 1)
  {
    receivedData += (data[0] << 0);
  }
  if(size == 2)
  {
    receivedData += (data[0] << 8);
    receivedData += (data[1] << 0);
  }
  if(size == 3)
  {
    receivedData += (data[0] << 16);
    receivedData += (data[1] << 8);
    receivedData += (data[2] << 0);
  }
  return receivedData;
}
//-----------------------------------------------------------------------------------------------
/*******************************************************************************
 * @brief Initializes the AD7799 and checks if the device is present.
 *
 * @param None.
 *
 * @return status - Result of the initialization procedure.
 *                  Example: 1 - if initialization was successful (ID is 0x0B).
 *                           0 - if initialization was unsuccessful.
 *******************************************************************************/
unsigned char AD7799_Init(void)
{ 
  unsigned char status = 0x1;
  status = AD7799_GetRegisterValue(AD7799_REG_ID, 1);

  if((status & 0x0F) != AD7799_ID)
  {
    status = 0x0;
  }
  return(status);
}
//------------------------------------------------------------------------------------------------
/*******************************************************************************
 * @brief  Sets the gain of the In-Amp.
 *
 * @param  gain - Gain.
 *
 * @return  None.    
 *******************************************************************************/
void AD7799_SetGain(unsigned long gain)
{
  unsigned long command;
  command = AD7799_GetRegisterValue(AD7799_REG_CONF,2);
  command &= ~AD7799_CONF_GAIN(0xFF);
  command |= AD7799_CONF_GAIN(gain);
  AD7799_SetRegisterValue(AD7799_REG_CONF, command, 2);
}
//------------------------------------------------------------------------------------------------
/*******************************************************************************
 * @brief Selects the channel of AD7799.
 *
 * @param  channel - ADC channel selection.
 *
 * @return  None.    
 *******************************************************************************/
void AD7799_SetChannel(unsigned long channel)
{
  unsigned long command;
  command = AD7799_GetRegisterValue(AD7799_REG_CONF,2);
  command &= ~AD7799_CONF_CHAN(0xFF);
  command |= AD7799_CONF_CHAN(channel);
  AD7799_SetRegisterValue(AD7799_REG_CONF, command, 2);
}
//------------------------------------------------------------------------------------------------
/*******************************************************************************
 * @brief Enables or disables the reference detect function.
 *
 * @param state - State of the reference detect function.
 *               Example: 0        - Reference detect disabled.
 *                        1        - Reference detect enabled.
 *
 * @return None.    
 *******************************************************************************/
void AD7799_SetReference(unsigned char state)
{
  unsigned long command = 0;
  command = AD7799_GetRegisterValue(AD7799_REG_CONF,2);
  command &= ~AD7799_CONF_REFDET(1);
  command |= AD7799_CONF_REFDET(state);
  AD7799_SetRegisterValue(AD7799_REG_CONF, command, 2);
}
//------------------------------------------------------------------------------------------------
/*******************************************************************************
 * @brief Sets the operating mode of AD7799.
 *
 * @param mode - Mode of operation.
 *
 * @return  None.    
 *******************************************************************************/
void AD7799_SetMode(unsigned long mode)
{
  unsigned long command;
  command = AD7799_GetRegisterValue(AD7799_REG_MODE,2);
  command &= ~AD7799_MODE_SEL(0xFF);
  command |= AD7799_MODE_SEL(mode);
  AD7799_SetRegisterValue(AD7799_REG_MODE, command, 2);
}
//------------------------------------------------------------------------------------------------
/*******************************************************************************
 * @brief Reads /RDY bit of status reg.
 *
 * @param None.
 *
 * @return rdy  - 0 if RDY is 1.
 *              - 1 if RDY is 0.
 *******************************************************************************/
unsigned char AD7799_Ready(void)
{
  unsigned char rdy = 0;
  rdy = (AD7799_GetRegisterValue(AD7799_REG_STAT,1) & AD7799_STAT_RDY);   

  return(!rdy);
}
//------------------------------------------------------------------------------------------------
unsigned char AD7799_Ready_channel_1(void)
{
  unsigned char rdy = 0;
  rdy = (AD7799_GetRegisterValue(AD7799_REG_STAT,1) & AD7799_STAT_CH1);   

  return(!rdy);
}
//------------------------------------------------------------------------------------------------
unsigned char AD7799_Ready_channel_2(void)
{
  unsigned char rdy = 0;
  rdy = (AD7799_GetRegisterValue(AD7799_REG_STAT,1) & AD7799_STAT_CH2);   

  return(!rdy);
}
//------------------------------------------------------------------------------------------------
unsigned char AD7799_Ready_channel_3(void)
{
  unsigned char rdy = 0;
  rdy = (AD7799_GetRegisterValue(AD7799_REG_STAT,1) & AD7799_STAT_CH3);   

  return(!rdy);
}
//------------------------------------------------------------------------------------------------
void spi_init(void) 
{
  pinMode(10, OUTPUT);
  // start the SPI library:
  SPI.setClockDivider(SPI_CLOCK_DIV32);
  SPI.begin();
}
//------------------------------------------------------------------------------------------------
void setup(void) 
{
  Serial.begin(9600);
  spi_init();
}
//------------------------------------------------------------------------------------------------
void print_data(String str, unsigned long value)
{
  Serial.print(str);
  if(value > 0x800000) 
  {
    value -= 0x800000;
    value = ((value * 2500) >> 23);
    Serial.print("+");
  } 
  else 
  {
    value = 0x800000 - value;
    value = ((value * 2500) >> 23);
    Serial.print("-");
  }
  Serial.print(value);
  Serial.println(" mV");
}
//------------------------------------------------------------------------------------------------
void get_data_1(void)
{
  bool ok = false;
  
  AD7799_SetChannel(AD7799_CH_AIN1P_AIN1M);     /* AIN1(+) - AIN1(-) */
  ok = false;
  while(!ok)
  {
    ok = AD7799_Ready_channel_1();
    delay(100);
  }
  
  AD7799_SetGain(AD7799_GAIN_1);                // set gain to 1
  AD7799_SetReference(AD7799_REFDET_ENA); 
  AD7799_SetMode(AD7799_MODE_SINGLE);
  ok = false;
  while(!ok)
  {
    ok = AD7799_Ready();
    delay(100);
  }
  print_data("Data1 :", AD7799_GetRegisterValue(AD7799_REG_DATA, 3));
  delay(100);
}
//------------------------------------------------------------------------------------------------
void get_data_2(void)
{
  bool ok = false;
 
  AD7799_SetChannel(AD7799_CH_AIN2P_AIN2M);     /* AIN2(+) - AIN2(-) */
  ok = false;
  while(!ok)
  {
    ok = AD7799_Ready_channel_2();
    delay(100);
  }
  
  AD7799_SetGain(AD7799_GAIN_1);                // set gain to 1
  AD7799_SetReference(AD7799_REFDET_ENA); 
  AD7799_SetMode(AD7799_MODE_SINGLE);
  ok = false;
  while(!ok)
  {
    ok = AD7799_Ready();
    delay(100);
  }
  print_data("Data2 :", AD7799_GetRegisterValue(AD7799_REG_DATA, 3));
  delay(100);
}
//------------------------------------------------------------------------------------------------
void get_data_3(void)
{
  bool ok = false;
  
  AD7799_SetChannel(AD7799_CH_AIN3P_AIN3M);     /* AIN3(+) - AIN3(-) */
  ok = false;
  while(!ok)
  {
    ok = AD7799_Ready_channel_3();
    delay(100);
  }
  
  AD7799_SetGain(AD7799_GAIN_1);                // set gain to 1
  AD7799_SetReference(AD7799_REFDET_ENA); 
  AD7799_SetMode(AD7799_MODE_SINGLE);
  ok = false;
  while(!ok)
  {
    ok = AD7799_Ready();
    delay(100);
  }
  print_data("Data3 :", AD7799_GetRegisterValue(AD7799_REG_DATA, 3));
  delay(100);
}
//------------------------------------------------------------------------------------------------
void loop(void) 
{
  AD7799_Reset();
  delay(200);	
  Serial.println("AD7799 Init() ");
  if(!AD7799_Init())	
  {
    Serial.println("AD7799 Err ");
    while(1);
  }
  AD7799_SetGain(AD7799_GAIN_1);                // set gain to 1
  //AD7799_SetChannel(AD7799_CH_AIN1P_AIN1M);     /* AIN1(+) - AIN1(-) */
  //AD7799_SetChannel(AD7799_CH_AIN2P_AIN2M);     /* AIN2(+) - AIN2(-) */
  //AD7799_SetChannel(AD7799_CH_AIN3P_AIN3M);     /* AIN3(+) - AIN3(-) */
  //AD7799_SetChannel(AD7799_CH_AIN1M_AIN1M);     /* AIN1(-) - AIN1(-) */
  AD7799_SetReference(AD7799_REFDET_ENA); 
  //AD7799_SetMode(AD7799_MODE_CONT);
  Serial.println("get data");
  delay(200);
  while(1) 
  {
    Serial.println("-----");
    get_data_1();
    get_data_2();
    get_data_3();
  }
}
//------------------------------------------------------------------------------------------------
