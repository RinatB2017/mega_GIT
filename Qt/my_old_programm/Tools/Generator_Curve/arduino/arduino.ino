//---------------------------------------------------------------
#define MAX_SLIDER  32
//---------------------------------------------------------------
int pin_out = 9;
int incomingByte = 0;

int index = 0;
int buf[MAX_SLIDER] = { 0 };
//---------------------------------------------------------------
#define MAX_LEN_ASCII   (MAX_SLIDER * 2)
#define MAX_LEN_MODBUS  MAX_SLIDER
//---------------------------------------------------------------
int buf_ascii[MAX_LEN_ASCII];
int len_ascii = 0;
//---------------------------------------------------------------
unsigned char convert_ascii_to_value(char hi, char lo)
{
  char b_hi = 0;
  char b_lo = 0;

  //---
  switch (hi)
  {
    case '0':  b_hi = 0x0;  break;
    case '1':  b_hi = 0x1;  break;
    case '2':  b_hi = 0x2;  break;
    case '3':  b_hi = 0x3;  break;
    case '4':  b_hi = 0x4;  break;
    case '5':  b_hi = 0x5;  break;
    case '6':  b_hi = 0x6;  break;
    case '7':  b_hi = 0x7;  break;
    case '8':  b_hi = 0x8;  break;
    case '9':  b_hi = 0x9;  break;
    case 'A':  b_hi = 0xA;  break;
    case 'B':  b_hi = 0xB;  break;
    case 'C':  b_hi = 0xC;  break;
    case 'D':  b_hi = 0xD;  break;
    case 'E':  b_hi = 0xE;  break;
    case 'F':  b_hi = 0xF;  break;
    default:
      break;
  }
  //---
  switch (lo)
  {
    case '0':  b_lo = 0x0;  break;
    case '1':  b_lo = 0x1;  break;
    case '2':  b_lo = 0x2;  break;
    case '3':  b_lo = 0x3;  break;
    case '4':  b_lo = 0x4;  break;
    case '5':  b_lo = 0x5;  break;
    case '6':  b_lo = 0x6;  break;
    case '7':  b_lo = 0x7;  break;
    case '8':  b_lo = 0x8;  break;
    case '9':  b_lo = 0x9;  break;
    case 'A':  b_lo = 0xA;  break;
    case 'B':  b_lo = 0xB;  break;
    case 'C':  b_lo = 0xC;  break;
    case 'D':  b_lo = 0xD;  break;
    case 'E':  b_lo = 0xE;  break;
    case 'F':  b_lo = 0xF;  break;
    default:
      break;
  }
  //---
  return ((b_hi << 4) | b_lo);
}
//---------------------------------------------------------------
void analize(void)
{
  if (len_ascii % 2) return;
  if (len_ascii != MAX_LEN_ASCII) return;

  //---
  for (int n = 0; n < len_ascii; n += 2)
  {
    buf[n] = convert_ascii_to_value(buf_ascii[n],
                                    buf_ascii[n + 1]);
  }
}
//---------------------------------------------------------------
void serialEvent()
{
  while (Serial.available())
  {
    // считываем байт
    incomingByte = Serial.read();
    switch (incomingByte)
    {
      case ':':
        len_ascii = 0;
        break;

      case '\n':
        analize();
        break;

      default:
        if (len_ascii < MAX_LEN_ASCII)
        {
          buf_ascii[len_ascii] = incomingByte;
          len_ascii++;
        }
        else
        {
          len_ascii = 0;
        }
        break;
    }
  }
}
//---------------------------------------------------------------
void init_buf(void)
{
  int delta = 256 / MAX_SLIDER;
  int value = 0;
  //Serial.println(delta);
  for (int n = 0; n < MAX_SLIDER; n++)
  {
    buf[n] = value;
    value += delta;
    Serial.println(buf[n]);
  }
}
//---------------------------------------------------------------
void setup()
{
  Serial.begin(115200);
  pinMode(pin_out, OUTPUT);

  init_buf();
}
//---------------------------------------------------------------
void loop()
{
  if (index < MAX_SLIDER)
    index++;
  else
    index = 0;
  analogWrite(pin_out, buf[index]);

  //Serial.println(buf[index]);
}
//---------------------------------------------------------------

