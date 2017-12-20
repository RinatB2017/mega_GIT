
#include <SPI.h>

#include "nRF24L01P.h"
/*                              ce, cs, SPI */
nRF24L01P rf24l01p(8, 7, SPI);

void setup()
{
    Serial.begin(115200);
  
    Serial.println("rxdemo");
    rf24l01p.nRF24L01_HW_Init();
    
    rf24l01p.nRF24L01_Set_RX_Address(0x00, 0x00, 0x00, 0x00, 0x00);
    
    rf24l01p.nRF24L01_Config(120, P0dBm, R250kbps);
    rf24l01p.RX_Mode();
}
void loop()
{
    unsigned char rec_buffer[RX_PLOAD_WIDTH]={0};
    if(rf24l01p.nRF24L01_RxPacket(rec_buffer))
    {
        Serial.println("Received(HEX):");
        for(int i=0;i<RX_PLOAD_WIDTH;i++)
        {
            Serial.print(rec_buffer[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }
    delay(100);
}
