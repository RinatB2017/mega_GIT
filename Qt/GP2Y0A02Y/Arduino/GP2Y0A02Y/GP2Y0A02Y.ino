//-------------------------------------------------------
//
//-------------------------------------------------------
#include <SharpIR.h>
//-------------------------------------------------------
int addr[8] = { A0, A1, A2, A3, A4, A5, A6, A7 };
int dist[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
//-------------------------------------------------------
#define model 20150 //модель датчика. 1080 для GP2Y0A21Y, 20150 для GP2Y0A02Y
//-------------------------------------------------------
SharpIR sharpIR_0(addr[0], model);
SharpIR sharpIR_1(addr[1], model);
SharpIR sharpIR_2(addr[2], model);
SharpIR sharpIR_3(addr[3], model);
SharpIR sharpIR_4(addr[4], model);
SharpIR sharpIR_5(addr[5], model);
SharpIR sharpIR_6(addr[6], model);
SharpIR sharpIR_7(addr[7], model);
//-------------------------------------------------------
union SHORT {
  unsigned short value;
  struct {
    unsigned char a;
    unsigned char b;
  } bytes;
};
//-------------------------------------------------------
void convert_data_to_ascii(unsigned char data)
{
    unsigned char hi = (data >> 4) & 0x0F;
    unsigned char lo = (data & 0x0F);

    switch(hi)
    {
    case 0x00: Serial.print("0"); break;
    case 0x01: Serial.print("1"); break;
    case 0x02: Serial.print("2"); break;
    case 0x03: Serial.print("3"); break;
    case 0x04: Serial.print("4"); break;
    case 0x05: Serial.print("5"); break;
    case 0x06: Serial.print("6"); break;
    case 0x07: Serial.print("7"); break;
    case 0x08: Serial.print("8"); break;
    case 0x09: Serial.print("9"); break;
    case 0x0A: Serial.print("A"); break;
    case 0x0B: Serial.print("B"); break;
    case 0x0C: Serial.print("C"); break;
    case 0x0D: Serial.print("D"); break;
    case 0x0E: Serial.print("E"); break;
    case 0x0F: Serial.print("F"); break;
    default: break;
    }

    switch(lo)
    {
    case 0x00: Serial.print("0"); break;
    case 0x01: Serial.print("1"); break;
    case 0x02: Serial.print("2"); break;
    case 0x03: Serial.print("3"); break;
    case 0x04: Serial.print("4"); break;
    case 0x05: Serial.print("5"); break;
    case 0x06: Serial.print("6"); break;
    case 0x07: Serial.print("7"); break;
    case 0x08: Serial.print("8"); break;
    case 0x09: Serial.print("9"); break;
    case 0x0A: Serial.print("A"); break;
    case 0x0B: Serial.print("B"); break;
    case 0x0C: Serial.print("C"); break;
    case 0x0D: Serial.print("D"); break;
    case 0x0E: Serial.print("E"); break;
    case 0x0F: Serial.print("F"); break;
    default: break;
    }
}
//-------------------------------------------------------
void send_packet() {
  SHORT data;
  Serial.print(":");
  for(int n=0; n<8; n++) {
    data.value = dist[n];
    convert_data_to_ascii(data.bytes.b);
    convert_data_to_ascii(data.bytes.a);
  }
  Serial.println();
}
//-------------------------------------------------------
void setup() {
  Serial.begin(115200);
}
//-------------------------------------------------------
void loop() {
  delay(100);   
  dist[0]=sharpIR_0.distance();
  dist[1]=sharpIR_1.distance();
  dist[2]=sharpIR_2.distance();
  dist[3]=sharpIR_3.distance();
  dist[4]=sharpIR_4.distance();
  dist[5]=sharpIR_5.distance();
  dist[6]=sharpIR_6.distance();
  dist[7]=sharpIR_7.distance();
  
  send_packet();
}
//-------------------------------------------------------

