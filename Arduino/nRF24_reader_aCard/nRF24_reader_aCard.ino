#include <SPI.h>
#include <Mirf.h>
#include <Mirf_nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

#include <SPI.h>

void setup()
{
  Serial.begin(9600);
  
  uint16_t addr = 0x0000000000;
  
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)addr);
  Mirf.payload = sizeof(unsigned long);
  Mirf.config();
  
  byte rf_data = 0;
  Mirf.writeRegister( EN_RXADDR, &rf_data, sizeof(rf_data) );

  rf_data = 0x8C;
  Mirf.writeRegister( CONFIG, &rf_data, sizeof(rf_data) );
  
  rf_data = 0x00;
  Mirf.writeRegister( SETUP_RETR, &rf_data, sizeof(rf_data) );

  rf_data = 0x03;
  Mirf.writeRegister( SETUP_AW, &rf_data, sizeof(rf_data) );
  
  Serial.println("Reading..."); 
}

void loop()
{
  byte data[Mirf.payload];
  
  if(Mirf.dataReady())
  {
    Serial.println("Got packet");
    Mirf.getData(data);
  }
}

