//---------------------------------------------------------------
//
//---------------------------------------------------------------
#define serial_WIFI  Serial
//---------------------------------------------------------------
int incomingByte = 0;
//---------------------------------------------------------------
boolean stringComplete = false;  
//---------------------------------------------------------------
#define MAX_LEN 200
//---------------------------------------------------------------
int buf_ascii[MAX_LEN];
int len_ascii = 0;
//---------------------------------------------------------------
void serialEvent()
{
  while (serial_WIFI.available()) 
  {
    // считываем байт
    incomingByte = serial_WIFI.read();
    switch(incomingByte)
    {
    case ':':
      stringComplete = false;
      len_ascii = 0;
      break;
  
    case '\n':
      stringComplete = true;
      break;
  
    default:
      if(len_ascii < MAX_LEN)
      {
        buf_ascii[len_ascii] = incomingByte;
        len_ascii++;
      }
      else
      {
        stringComplete = false;
        len_ascii = 0;
      }      
      break;  
    }
  }
}
//---------------------------------------------------------------
void setup()
{
  serial_WIFI.begin(9600);
}
//---------------------------------------------------------------
void loop()
{
  if(stringComplete)
  {
    for(int n=0; n<len_ascii; n++)
    {
      serial_WIFI.print(buf_ascii[n]);
    }
    stringComplete = false;
    serial_WIFI.println();
  }
}
//---------------------------------------------------------------

