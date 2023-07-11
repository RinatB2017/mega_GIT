//---------------------------------------------------------------
// проверка температуры по восьми датчикам
// Автор: Бикбао Ринат Зинорович
//---------------------------------------------------------------
// https://arduinomaster.ru/datchiki-arduino/arduino-ds18b20/
// http://wiki.amperka.ru/%D0%BF%D1%80%D0%BE%D0%B4%D1%83%D0%BA%D1%82%D1%8B:ds18b20

#include <OneWire.h>
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

DeviceAddress deviceAddress_0;
DeviceAddress deviceAddress_1;
DeviceAddress deviceAddress_2;
DeviceAddress deviceAddress_3;
DeviceAddress deviceAddress_4;
DeviceAddress deviceAddress_5;
DeviceAddress deviceAddress_6;
DeviceAddress deviceAddress_7;

void init_curves()
{
  Serial.println(":curves|Температура 0|Температура 1|Температура 2|Температура 3|Температура 4|Температура 5|Температура 6|Температура 7"); 
} 

void setup()
{
  Serial.begin(57600);
  init_curves();

  dallasSensors_0.begin();
  dallasSensors_0.setResolution(12);

  dallasSensors_1.begin();
  dallasSensors_1.setResolution(12);
  
  dallasSensors_2.begin();
  dallasSensors_2.setResolution(12);
  
  dallasSensors_3.begin();
  dallasSensors_3.setResolution(12);
  
  dallasSensors_4.begin();
  dallasSensors_4.setResolution(12);
  
  dallasSensors_5.begin();
  dallasSensors_5.setResolution(12);
  
  dallasSensors_6.begin();
  dallasSensors_6.setResolution(12);
  
  dallasSensors_7.begin();
  dallasSensors_7.setResolution(12);
  
}

void loop(void)
{
  dallasSensors_0.requestTemperatures();
  dallasSensors_1.requestTemperatures();
  dallasSensors_2.requestTemperatures();
  dallasSensors_3.requestTemperatures();
  dallasSensors_4.requestTemperatures();
  dallasSensors_5.requestTemperatures();
  dallasSensors_6.requestTemperatures();
  dallasSensors_7.requestTemperatures();

  float temp_0 = dallasSensors_0.getTempCByIndex(0);
  float temp_1 = dallasSensors_1.getTempCByIndex(0);
  float temp_2 = dallasSensors_2.getTempCByIndex(0);
  float temp_3 = dallasSensors_3.getTempCByIndex(0);
  float temp_4 = dallasSensors_4.getTempCByIndex(0);
  float temp_5 = dallasSensors_5.getTempCByIndex(0);
  float temp_6 = dallasSensors_6.getTempCByIndex(0);
  float temp_7 = dallasSensors_7.getTempCByIndex(0);

  Serial.println(":data|" + 
                  String(temp_0) + "|" + 
                  String(temp_1) + "|"+ 
                  String(temp_2) + "|" + 
                  String(temp_3) + "|"+ 
                  String(temp_4) + "|" + 
                  String(temp_5) + "|"+ 
                  String(temp_6) + "|" + 
                  String(temp_7));
  //delay(1000);

  cnt++;
  if(cnt > 10)
  {
    cnt = 0;
    init_curves();
  }
}
