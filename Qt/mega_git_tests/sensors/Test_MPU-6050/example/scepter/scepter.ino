//---------------------------------------------------------------
#include "FastLED.h"
//---------------------------------------------------------------
#include <ESP8266WiFi.h>

const char* ssid     = "RT-WiFi_DBA0";
const char* password = "CUDRM67c";

int port = 15000;
//---------------------------------------------------------------
#define LEDS_PER_STRIP  32
#define LED_PIN D8
//---------------------------------------------------------------
#define DELAY_MS  1
#define MIN_BRIGHTNESS  0
#define MAX_BRIGHTNESS  127
#define DEFAULT_BRIGHTNESS  64
//---------------------------------------------------------------
CRGB line0[LEDS_PER_STRIP];
CLEDController *controllers_0;
//---------------------------------------------------------------
#define MAX_X 32
#define MAX_Y 32
CRGB matrix[MAX_X + 1][MAX_Y + 1];
int index_y = 0;
//---------------------------------------------------------------
String default_array[MAX_Y] = {
  "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR",
  " RRRRRRRRRRRRRRRRRRRRRRRRRRRRRR ",
  "  RRRRRRRRRRRRRRRRRRRRRRRRRRRR  ",
  "   RRRRRRRRRRRRRRRRRRRRRRRRRR   ",
  "    RRRRRRRRRRRRRRRRRRRRRRRR    ",
  "     RRRRRRRRRRRRRRRRRRRRRR     ",
  "      RRRRRRRRRRRRRRRRRRRR      ",
  "       RRRRRRRRRRRRRRRRRR       ",
  "        RRRRRRRRRRRRRRRR        ",
  "         RRRRRRRRRRRRRR         ",
  "          RRRRRRRRRRRR          ",
  "           RRRRRRRRRR           ",
  "            RRRRRRRR            ",
  "             RRRRRR             ",
  "              RRRR              ",
  "               RR               ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                "
};
//---------------------------------------------------------------
int gBrightness = DEFAULT_BRIGHTNESS;
int index_color = 0;
CRGB color = 0;
//---------------------------------------------------------------
// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(port);
WiFiClient client;
//---------------------------------------------------------------
void pause_ms(int time_ms)
{
  if (time_ms <= 0)
  {
    return;
  }

  delay(time_ms);
}
//---------------------------------------------------------------
void init_WIFI(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}
//---------------------------------------------------------------
void init_leds(void)
{
  controllers_0 = &FastLED.addLeds<WS2812, LED_PIN, GRB>(line0, LEDS_PER_STRIP);
}
//---------------------------------------------------------------
void show_leds(void)
{
  controllers_0->show(line0, LEDS_PER_STRIP, gBrightness);
}
//---------------------------------------------------------------
void clear_leds(void)
{
  int x = 0;
  for (x = 0; x < LEDS_PER_STRIP; x++) line0[x] = 0;
}
//---------------------------------------------------------------
bool set_pixel(unsigned int addr, CRGB color)
{
  if ((addr < 0) || (addr >= LEDS_PER_STRIP))
  {
    Serial.print("set_pixel: bad address ");
    Serial.println(addr, DEC);
    return false;
  }
  line0[addr] = color;
}
//---------------------------------------------------------------
void f_01(void)
{
  for (int index_color = 0; index_color < 4; index_color++)
  {
    switch (index_color)
    {
      case 0:
        color = CRGB::Red;
        break;
      case 1:
        color = CRGB::Green;
        break;
      case 2:
        color = CRGB::Blue;
        break;
      case 3:
        color = CRGB::White;
        break;

      default:
        index_color = 0;
        color = CRGB::Red;
        break;
    }

    for (int n = 0; n < LEDS_PER_STRIP; n++)
    {
      set_pixel(n - 1,  CRGB::Black);
      set_pixel(n,  color);
      show_leds();
      pause_ms(DELAY_MS);
    }
    for (int n = LEDS_PER_STRIP; n > 0; n--)
    {
      set_pixel(n + 1,  CRGB::Black);
      set_pixel(n,  color);
      show_leds();
      pause_ms(DELAY_MS);
    }
  }
}
//---------------------------------------------------------------
void Question(String text)
{
  String temp = text;
  Serial.println("Question: " + temp);
  //char *str0 = strtok(temp, " ");
  int pos_0 = temp.indexOf(" ");
  if (pos_0 < 0)
  {
    return;
  }

  String temp2 = temp.substring(pos_0 + 1);
  Serial.println("pos_0: " + String(pos_0));
  Serial.println("temp2: " + temp2);

  int pos_1 = temp2.indexOf(" ");
  if (pos_0 < 0)
  {
    return;
  }

  Serial.println("pos_1: " + String(pos_1));
  String temp3 = temp2.substring(0, pos_1);
  Serial.println("temp3: " + temp3);

  String temp4 = temp3.substring(5);
  Serial.println("temp4: " + temp4);

  int value_pwm = temp4.toInt();
  Serial.println("value_pwm = " + String(value_pwm));
}
//---------------------------------------------------------------
void loop_server(void)
{
  // Check if a client has connected
  client = server.available();
  if (!client)
  {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  int err_cnt = 0;
  while (!client.available())
  {
    err_cnt++;
    if (err_cnt > 1000)
    {
      Serial.println("... failed");
      return;
    }
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Question(req);

  Serial.println(req);
  client.flush();

  // GET /gpio/0 HTTP/1.1

  // Match the request
  int val = 0;
  if (req.indexOf("/gpio/0") != -1)
  {
    val = 0;
    //    digitalWrite(DIGITAL_PIN_1, val);
  }
  else if (req.indexOf("/gpio/1") != -1)
  {
    val = 1023;
    //    digitalWrite(DIGITAL_PIN_1, val);
  }
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += (val) ? "high" : "low";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected
  // when the function returns and 'client' object is detroyed
}
//---------------------------------------------------------------
void init_picture(void)
{
  for (int y = 0; y < MAX_Y; y++)
  {
    for (int x = 0; x < MAX_X; x++)
    {
      CRGB color = CRGB::Black;
      matrix[x][y] = color;
    }
  }

  for (int y = 0; y < MAX_Y; y++)
  {
    String temp = default_array[y];
    for (int x = 0; x < MAX_X; x++)
    {
      CRGB color = CRGB::Black;
      char s = temp[x];
      switch (s)
      {
        case 'R':
          color = CRGB::Red;
          break;

        case 'G':
          color = CRGB::Green;
          break;

        case 'B':
          color = CRGB::Blue;
          break;
      }
      matrix[x][y] = color;
    }
  }

  //  for (int y = 0; y < MAX_Y; y++)
  //  {
  //    if (y == 0)
  //    {
  //      for (int x = 0; x < MAX_X; x++)
  //      {
  //        CRGB color = CRGB::Red;
  //        matrix[x][y] = color;
  //      }
  //    } else {
  //      matrix[0][y] = CRGB::Red;
  //      matrix[MAX_X - 1][y] = CRGB::Red;
  //    }
  //  }
}
//---------------------------------------------------------------
void show_picture(void)
{
  for (int y = 0; y < MAX_Y; y++)
  {
    for (int x = 0; x < MAX_X; x++)
    {
      CRGB color;
      color = matrix[x][y];
      set_pixel(x, color);
    }
    show_leds();
    pause_ms(DELAY_MS);
  }
}
//---------------------------------------------------------------
void setup()
{
  Serial.begin(57600);
  init_leds();

  //init_WIFI();
  init_picture();
}
//---------------------------------------------------------------
void loop(void)
{
  //f_01();
  //loop_server();
  show_picture();
}
//---------------------------------------------------------------
