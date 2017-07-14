//--------------------------------------------------------------------------------
#include <SPI.h>
//---------------------------------------------------------------
const int slaveSelectPin = 10;
//---------------------------------------------------------------
/*AD7799 Registers*/
#define AD7799_REG_COMM		0	/* Communications Register	(WO, 8-bit) */
#define AD7799_REG_STAT	    	0 	/* Status Register	    	(RO, 8-bit) */
#define AD7799_REG_MODE	    	1 	/* Mode Register	     	        (RW, 16-bit */
#define AD7799_REG_CONF	    	2 	/* Configuration Register	(RW, 16-bit)*/
#define AD7799_REG_DATA	    	3 	/* Data Register	     	        (RO, 16-/24-bit) */
#define AD7799_REG_ID	    	4 	/* ID Register	     		(RO, 8-bit) */
#define AD7799_REG_IO	    	5 	/* IO Register	     		(RO, 8-bit) */
#define AD7799_REG_OFFSET   	6 	/* Offset Register	    	(RW, 24-bit */
#define AD7799_REG_FULLSALE	7 	/* Full-Scale Register		(RW, 24-bit */

/* Communications Register Bit Designations (AD7799_REG_COMM) */
#define AD7799_COMM_WEN		(1 << 7)			        /* Write Enable */
#define AD7799_COMM_WRITE		(0 << 6)			/* Write Operation */
#define AD7799_COMM_READ    	        (1 << 6)			/* Read Operation */
#define AD7799_COMM_ADDR(x)		(((x) & 0x7) << 3)	        /* Register Address */
#define AD7799_COMM_CREAD		(1 << 2)			/* Continuous Read of Data Register */

/* Status Register Bit Designations (AD7799_REG_STAT) */
#define AD7799_STAT_RDY		(1 << 7) 	/* Ready */
#define AD7799_STAT_ERR		(1 << 6) 	/* Error (Overrange, Underrange) */
#define AD7799_STAT_CH3		(1 << 2) 	/* Channel 3 */
#define AD7799_STAT_CH2		(1 << 1) 	/* Channel 2 */
#define AD7799_STAT_CH1		(1 << 0) 	/* Channel 1 */

/* Mode Register Bit Designations (AD7799_REG_MODE) */
#define AD7799_MODE_SEL(x)		(((x) & 0x7) << 13)	        /* Operation Mode Select */
#define AD7799_MODE_PSW(x)		(1 << 12)			/* Power Switch Control Bit */
#define AD7799_MODE_RATE(x)		((x) & 0xF)			/* Filter Update Rate Select */

/* AD7799_MODE_SEL(x) options */
#define AD7799_MODE_CONT		0	/* Continuous Conversion Mode */
#define AD7799_MODE_SINGLE		1 	/* Single Conversion Mode */
#define AD7799_MODE_IDLE		2 	/* Idle Mode */
#define AD7799_MODE_PWRDN		3 	/* Power-Down Mode */
#define AD7799_MODE_CAL_INT_ZERO	4 	/* Internal Zero-Scale Calibration */
#define AD7799_MODE_CAL_INT_FULL	5 	/* Internal Full-Scale Calibration */
#define AD7799_MODE_CAL_SYS_ZERO	6 	/* System Zero-Scale Calibration */
#define AD7799_MODE_CAL_SYS_FULL	7 	/* System Full-Scale Calibration */

/* Configuration Register Bit Designations (AD7799_REG_CONF) */
#define AD7799_CONF_BO_EN	 (1 << 13)       	/* Burnout Current Enable */
#define AD7799_CONF_UNIPOLAR     (1 << 12) 		/* Unipolar/Bipolar Enable */
#define AD7799_CONF_GAIN(x)	 (((x) & 0x7) << 8) 	/* Gain Select */
#define AD7799_CONF_REFDET(x)    (((x) & 0x1) << 5) 	/* Reference detect function */
#define AD7799_CONF_BUF		 (1 << 4) 		/* Buffered Mode Enable */
#define AD7799_CONF_CHAN(x)      ((x) & 0x7) 		/* Channel select */

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
#define AD7799_REFDET_DIS    0

/* AD7799_CONF_CHAN(x) options */
#define AD7799_CH_AIN1P_AIN1M	0	/* AIN1(+) - AIN1(-) */
#define AD7799_CH_AIN2P_AIN2M	1	/* AIN2(+) - AIN2(-) */
#define AD7799_CH_AIN3P_AIN3M	2	/* AIN3(+) - AIN3(-) */
#define AD7799_CH_AIN1M_AIN1M	3	/* AIN1(-) - AIN1(-) */
#define AD7799_CH_AVDD_MONITOR	7	/* AVDD Monitor */

/* ID Register Bit Designations (AD7799_REG_ID) */
#define AD7799_ID		0x9
#define AD7799_ID_MASK	0xF

/* IO (Excitation Current Sources) Register Bit Designations (AD7799_REG_IO) */
#define AD7799_IOEN		(1 << 6)
#define AD7799_IO1(x)	(((x) & 0x1) << 4)
#define AD7799_IO2(x)	(((x) & 0x1) << 5)
//---------------------------------------------------------------
union PACKET
{
  struct
  {
    int32_t ch0;
    int32_t ch1;
    int32_t ch2;
  } data;
  uint8_t buf[sizeof(data)];
};
//--------------------------------------------------------------------------------
uint8_t convert_ansi_to_dec(char data)
{
  uint8_t temp = 0;
  switch(data)
  {
    case '0': temp = 0x00; break;
    case '1': temp = 0x01; break;
    case '2': temp = 0x02; break;
    case '3': temp = 0x03; break;
    case '4': temp = 0x04; break;
    case '5': temp = 0x05; break;
    case '6': temp = 0x06; break;
    case '7': temp = 0x07; break;
    case '8': temp = 0x08; break;
    case '9': temp = 0x09; break;
    case 'A': temp = 0x0A; break;
    case 'B': temp = 0x0B; break;
    case 'C': temp = 0x0C; break;
    case 'D': temp = 0x0D; break;
    case 'E': temp = 0x0E; break;
    case 'F': temp = 0x0F; break;
    default: break;
  }

  return temp;
}
//--------------------------------------------------------------------------------
char convert_dec_to_ansi(uint8_t data)
{
  char str = 0;
  switch(data)
  {
    case 0x00: str = '0'; break;
    case 0x01: str = '1'; break;
    case 0x02: str = '2'; break;
    case 0x03: str = '3'; break;
    case 0x04: str = '4'; break;
    case 0x05: str = '5'; break;
    case 0x06: str = '6'; break;
    case 0x07: str = '7'; break;
    case 0x08: str = '8'; break;
    case 0x09: str = '9'; break;
    case 0x0A: str = 'A'; break;
    case 0x0B: str = 'B'; break;
    case 0x0C: str = 'C'; break;
    case 0x0D: str = 'D'; break;
    case 0x0E: str = 'E'; break;
    case 0x0F: str = 'F'; break;
    default: break;
  }
  return str;
}
//---------------------------------------------------------------
int digitalPotWrite(int address, int value) 
{
  digitalWrite(slaveSelectPin,LOW);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(slaveSelectPin,HIGH); 
}
//--------------------------------------------------------------------------------
void setup(void)
{
  Serial.begin(9600);
  
  pinMode (slaveSelectPin, OUTPUT);
  SPI.begin(); 
  
  SPI.setBitOrder(SPI_2XCLOCK_MASK);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV128);
  /*разрешение spi,старший бит вперед,мастер, режим 0*/
  //SPCR = (1<<SPE)|(0<<DORD)|(1<<MSTR)|(0<<CPOL)|(0<<CPHA)|(1<<SPR1)|(0<<SPR0);
  //SPSR = (0<<SPI2X);
}
//--------------------------------------------------------------------------------
void get_ID(void)
{
  unsigned char cmd = AD7799_COMM_READ |  AD7799_COMM_ADDR(AD7799_REG_ID);
  
  digitalWrite(slaveSelectPin,LOW);
  unsigned char res = SPI.transfer(cmd);
  digitalWrite(slaveSelectPin,HIGH);
  
  Serial.print("ID = ");
  Serial.println(res & AD7799_ID_MASK);
}
//--------------------------------------------------------------------------------
void loop(void)
{
  int n=0;
  
  get_ID();
  while(1)
  {
    
  }

  while(1)
  {
    PACKET packet;
    packet.data.ch0 = double(308.0)*sin(double(n)*double(M_PI)/double(180.0));
    packet.data.ch1 = double(308.0)*sin(double(n)*double(M_PI)/double(180.0)) + 50;
    packet.data.ch2 = double(308.0)*sin(double(n)*double(M_PI)/double(180.0)) + 100;
    n++;
    if(n>360) n=0;

    Serial.print(":");
    for(int n =0; n<sizeof(PACKET); n++)
    {
      uint8_t data = packet.buf[n];
      uint8_t hi = (data >> 4) & 0x0F;
      uint8_t lo = (data & 0x0F);

      Serial.print(convert_dec_to_ansi(hi));
      Serial.print(convert_dec_to_ansi(lo));
    }
    Serial.println();
    delay(100);
  }
}
//--------------------------------------------------------------------------------


