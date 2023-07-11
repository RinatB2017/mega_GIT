// https://arduinomaster.ru/datchiki-arduino/arduino-ds18b20/
#include <DallasTemperature.h>

#define TEMP_0 2
#define TEMP_1 3
#define TEMP_2 4
#define TEMP_3 5
#define TEMP_4 6
#define TEMP_5 7
#define TEMP_6 8
#define TEMP_7 9

OneWire oneWire_0(TEMP_0);
OneWire oneWire_1(TEMP_1);
OneWire oneWire_2(TEMP_2);
OneWire oneWire_3(TEMP_3);
OneWire oneWire_4(TEMP_4);
OneWire oneWire_5(TEMP_5);
OneWire oneWire_6(TEMP_6);
OneWire oneWire_7(TEMP_7);

DallasTemperature dallasSensors_0(&oneWire_0);
DallasTemperature dallasSensors_1(&oneWire_1);
DallasTemperature dallasSensors_2(&oneWire_2);
DallasTemperature dallasSensors_3(&oneWire_3);
DallasTemperature dallasSensors_4(&oneWire_4);
DallasTemperature dallasSensors_5(&oneWire_5);
DallasTemperature dallasSensors_6(&oneWire_6);
DallasTemperature dallasSensors_7(&oneWire_7);

int cnt = 0; // счётчик для обновления названий графиков

void init_curves()
{
  Serial.println(":curves|Температура 0|Температура 1|Температура 2|Температура 3|Температура 4|Температура 5|Температура 6|Температура 7"); 
} 

void setup()
{
  Serial.begin(57600);
  init_curves();
}

void loop(void)
{
  //allasSensors_0.requestTemperatures();

  float temp_0 = dallasSensors_0.getTempC(0);
  float temp_1 = dallasSensors_1.getTempC(0);
  float temp_2 = dallasSensors_2.getTempC(0);
  float temp_3 = dallasSensors_3.getTempC(0);
  float temp_4 = dallasSensors_4.getTempC(0);
  float temp_5 = dallasSensors_5.getTempC(0);
  float temp_6 = dallasSensors_6.getTempC(0);
  float temp_7 = dallasSensors_7.getTempC(0);

  Serial.println(":data|" + 
                  String(temp_0) + "|" + 
                  String(temp_1) + "|"+ 
                  String(temp_2) + "|" + 
                  String(temp_3) + "|"+ 
                  String(temp_4) + "|" + 
                  String(temp_5) + "|"+ 
                  String(temp_6) + "|" + 
                  String(temp_7));
  delay(1000);

  cnt++;
  if(cnt > 10)
  {
    cnt = 0;
    init_curves();
  }
}
