//---------------------------------------------------------------
//
//---------------------------------------------------------------
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP4725.h>
//---------------------------------------------------------------
Adafruit_MCP4725 dac;
//---------------------------------------------------------------
int imcomingByte = 0;
String application_command;
//---------------------------------------------------------------
void init_serial()
{
  Serial.begin(57600);
}
//---------------------------------------------------------------
void init_mcp4725()
{
  // For Adafruit MCP4725A1 the address is 0x62 (default) or 0x63 (ADDR pin tied to VCC)
  // For MCP4725A0 the address is 0x60 or 0x61
  // For MCP4725A2 the address is 0x64 or 0x65
  dac.begin(0x62);
}
//---------------------------------------------------------------
int set_voltage(float voltage)
{
  return voltage * 4096.0f / 5.0f;
}
//---------------------------------------------------------------
//
// https://stackoverflow.com/questions/9072320/split-string-into-string-array
//
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
//---------------------------------------------------------------
void analize()
{
  String marker = getValue(application_command, '|', 0);
  if(marker != "value")
  {
    Serial.println("Bad marker");
    return;
  }

  String value = getValue(application_command, '|', 1);
  dac.setVoltage(set_voltage(value.toFloat()), false);
  //Serial.print("Set voltage ");
  //Serial.println(value);
}
//---------------------------------------------------------------
void setup(void)
{
  init_serial();
  init_mcp4725();

  Serial.println("Init MCP4725");

  //5 - 4096
  //1 - x
  //dac.setVoltage(set_voltage(0.5), false);
}

void loop(void)
{
  while (Serial.available())
  {
    imcomingByte = Serial.read();
    switch(imcomingByte)
    {
      case ':':
        application_command = "";
        break;

        case '\r':
        case '\n':
          analize();
          application_command = "";
          break;

        default:
          application_command.concat((char)imcomingByte);
          break;
    }
  }
}