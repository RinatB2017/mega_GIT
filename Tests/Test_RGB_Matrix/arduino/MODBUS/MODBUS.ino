//-------------------------------------------------------
int PIN_2  = 2;
int PIN_3  = 3;
int PIN_4  = 4;
int PIN_5  = 5;
int PIN_6  = 6;
int PIN_7  = 7;
int PIN_8  = 8;
int PIN_9  = 9;
int PIN_10 = 10;
int PIN_11 = 11;
int PIN_12 = 12;
int PIN_13 = 13;
//---------------------------------------------------------------
typedef struct
{
  uint8_t address;
  uint8_t cmd;
  uint8_t count_data;
} 
HEADER;
//---------------------------------------------------------------
union CMD_0x01_QUESTION
{
  struct {
    HEADER header;
    uint8_t led;
    uint8_t state;
    uint8_t crc8;
  } body;
  uint8_t buf[sizeof(body)];
};

union CMD_0x01_ANSWER
{
  struct {
    HEADER header;
    uint16_t crc8;
  } body;
  uint8_t buf[sizeof(body)];
};
//-------------------------------------------------------
#define MAX_SIZE  100
uint8_t in_buffer[MAX_SIZE]  = { 0 };
uint8_t out_buffer[MAX_SIZE] = { 0 };
int buf_len = 0;
//-------------------------------------------------------
void convert_data_to_ascii(uint8_t data)
{
    uint8_t hi = (data >> 4) & 0x0F;
    uint8_t lo = (data & 0x0F);

    switch(hi)
    {
    case 0x00: Serial3.print("0"); break;
    case 0x01: Serial3.print("1"); break;
    case 0x02: Serial3.print("2"); break;
    case 0x03: Serial3.print("3"); break;
    case 0x04: Serial3.print("4"); break;
    case 0x05: Serial3.print("5"); break;
    case 0x06: Serial3.print("6"); break;
    case 0x07: Serial3.print("7"); break;
    case 0x08: Serial3.print("8"); break;
    case 0x09: Serial3.print("9"); break;
    case 0x0A: Serial3.print("A"); break;
    case 0x0B: Serial3.print("B"); break;
    case 0x0C: Serial3.print("C"); break;
    case 0x0D: Serial3.print("D"); break;
    case 0x0E: Serial3.print("E"); break;
    case 0x0F: Serial3.print("F"); break;
    default: break;
    }

    switch(lo)
    {
    case 0x00: Serial3.print("0"); break;
    case 0x01: Serial3.print("1"); break;
    case 0x02: Serial3.print("2"); break;
    case 0x03: Serial3.print("3"); break;
    case 0x04: Serial3.print("4"); break;
    case 0x05: Serial3.print("5"); break;
    case 0x06: Serial3.print("6"); break;
    case 0x07: Serial3.print("7"); break;
    case 0x08: Serial3.print("8"); break;
    case 0x09: Serial3.print("9"); break;
    case 0x0A: Serial3.print("A"); break;
    case 0x0B: Serial3.print("B"); break;
    case 0x0C: Serial3.print("C"); break;
    case 0x0D: Serial3.print("D"); break;
    case 0x0E: Serial3.print("E"); break;
    case 0x0F: Serial3.print("F"); break;
    default: break;
    }
}
//-------------------------------------------------------
bool convert_ascii_to_data(uint8_t data_0, uint8_t data_1, uint8_t *result)
{
  int hi = -1;
  int lo = -1;
  
  switch(char(data_0))
  {
    case '0':  hi = 0x00; break;
    case '1':  hi = 0x01; break;
    case '2':  hi = 0x02; break;
    case '3':  hi = 0x03; break;
    case '4':  hi = 0x04; break;
    case '5':  hi = 0x05; break;
    case '6':  hi = 0x06; break;
    case '7':  hi = 0x07; break;
    case '8':  hi = 0x08; break;
    case '9':  hi = 0x09; break;
    case 'a':
    case 'A':  hi = 0x0A; break;
    case 'b':
    case 'B':  hi = 0x0B; break;
    case 'c':
    case 'C':  hi = 0x0C; break;
    case 'd':
    case 'D':  hi = 0x0D; break;
    case 'e':
    case 'E':  hi = 0x0E; break;
    case 'f':
    case 'F':  hi = 0x0F; break;
    default:  break;
  }
  if(hi < 0) 
  {
    return false;
  }
  
  switch(char(data_1))
  {
    case '0':  lo = 0x00; break;
    case '1':  lo = 0x01; break;
    case '2':  lo = 0x02; break;
    case '3':  lo = 0x03; break;
    case '4':  lo = 0x04; break;
    case '5':  lo = 0x05; break;
    case '6':  lo = 0x06; break;
    case '7':  lo = 0x07; break;
    case '8':  lo = 0x08; break;
    case '9':  lo = 0x09; break;
    case 'a':
    case 'A':  lo = 0x0A; break;
    case 'b':
    case 'B':  lo = 0x0B; break;
    case 'c':
    case 'C':  lo = 0x0C; break;
    case 'd':
    case 'D':  lo = 0x0D; break;
    case 'e':
    case 'E':  lo = 0x0E; break;
    case 'f':
    case 'F':  lo = 0x0F; break;
    default:  break;
  }
  if(lo < 0)
  {
    return false;
  }
  
  *result = (hi << 4) | lo;
  return true;
}
//-------------------------------------------------------
void analize(void)
{
  bool ok = false;
  uint8_t res = 0;
  int index = 0;
  for(int n=0; n<buf_len; n+=2)
  {
    ok = convert_ascii_to_data(in_buffer[n], in_buffer[n+1], &out_buffer[index]);
    if(!ok)
    {
      return;
    }
    index++;
  }
  CMD_0x01_QUESTION *question = (CMD_0x01_QUESTION *)out_buffer;
  char cmd = question->body.header.cmd;
  char x = 0;
  char state = 0;
  char v = 0;
  switch(cmd)
  {
    case 1:
      x = question->body.led;
      state = question->body.state;
      switch(x)
      {
        case 2:  digitalWrite(PIN_2,  state); break;
        case 3:  digitalWrite(PIN_3,  state); break;
        case 4:  digitalWrite(PIN_4,  state); break;
        case 5:  digitalWrite(PIN_5,  state); break;
        case 6:  digitalWrite(PIN_6,  state); break;
        case 7:  digitalWrite(PIN_7,  state); break;
        case 8:  digitalWrite(PIN_8,  state); break;
        case 9:  digitalWrite(PIN_9,  state); break;
        case 10: digitalWrite(PIN_10, state); break;
        case 11: digitalWrite(PIN_11, state); break;
        case 12: digitalWrite(PIN_12, state); break;
        case 13: digitalWrite(PIN_13, state); break;
        default: 
          break;
      }
      break;
      
    case 2:
      v = question->body.state;
      analogWrite(10, v);
      break;
  }
}
//-------------------------------------------------------
void modbus_receive(void)
{
  int s = 0;  
  if(Serial3.available())
  {
    s = Serial3.read();
    switch(s)
    {
      case ':':
        buf_len = 0;
        break;
        
      case '\n':
        analize();
        buf_len = 0;
        break;
        
      default:
        in_buffer[buf_len] = char(s);
        if(buf_len < MAX_SIZE) buf_len++;
        break;
    }
  }
}
//-------------------------------------------------------
void setup() 
{
  pinMode(PIN_2,  OUTPUT);
  pinMode(PIN_3,  OUTPUT);
  pinMode(PIN_4,  OUTPUT);
  pinMode(PIN_5,  OUTPUT);
  pinMode(PIN_6,  OUTPUT);
  pinMode(PIN_7,  OUTPUT);
  pinMode(PIN_8,  OUTPUT);
  pinMode(PIN_9,  OUTPUT);
  pinMode(PIN_10, OUTPUT);
  pinMode(PIN_11, OUTPUT);
  pinMode(PIN_12, OUTPUT);
  pinMode(PIN_13, OUTPUT);
  
  Serial3.begin(9600);
}
//-------------------------------------------------------
void loop() 
{
#if 0
  digitalWrite(PIN_3,  HIGH);
  digitalWrite(PIN_4,  HIGH);
  digitalWrite(PIN_5,  HIGH);
  digitalWrite(PIN_6,  HIGH);
  digitalWrite(PIN_7,  HIGH);
  digitalWrite(PIN_8,  HIGH);
  digitalWrite(PIN_9,  HIGH);
#endif  

#if 0
  digitalWrite(PIN_5,  0x0);
  delay(1000);
  digitalWrite(PIN_5,  0x1);
  delay(1000);
#endif  

#if 1
  modbus_receive();  
#endif

#if 0
  digitalWrite(PIN_10,  HIGH);
#endif  
}
//-------------------------------------------------------
