//---------------------------------------------------------------
// проверка температуры по восьми датчикам
// Автор: Бикбао Ринат Зинорович
//---------------------------------------------------------------
// https://arduinomaster.ru/datchiki-arduino/arduino-ds18b20/
// http://wiki.amperka.ru/%D0%BF%D1%80%D0%BE%D0%B4%D1%83%D0%BA%D1%82%D1%8B:ds18b20

#include <OneWire.h>
#include <DallasTemperature.h>

#define MAX_SENSORS 5

OneWire oneWire[MAX_SENSORS];
DallasTemperature dallasSensors[MAX_SENSORS];   //setOneWire

int cnt = 0; // счётчик для обновления названий графиков
String text;

void init_curves()
{
    text = ":curves|";
    for(int n=0; n<MAX_SENSORS; n++)
    {
        text += ("Температура " + String(n));
        if(n<MAX_SENSORS-1)
        {
            text += "|";
        }
    }
    Serial.println(text);
} 

void setup()
{
  Serial.begin(57600);
  init_curves();

  for(int n=0; n<MAX_SENSORS; n++)
  {
    oneWire[n].begin(n + 2);  // считаем от 2 пина
    dallasSensors[n].setOneWire(&oneWire[n]);

    dallasSensors[n].begin();
    dallasSensors[n].setResolution(12);
  }
}

void loop(void)
{
    text = ":data|";
    for(int n=0; n<MAX_SENSORS; n++)
    {
      dallasSensors[n].requestTemperatures();
      float temp = dallasSensors[n].getTempCByIndex(0);
      text += String(temp);
      if(n<MAX_SENSORS-1)
      {
          text += "|";
      }
    }
    Serial.println(text);
    delay(1000);

    cnt++;
    if(cnt > 10)
    {
        cnt = 0;
        init_curves();
    }
}
