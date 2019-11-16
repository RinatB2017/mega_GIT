#include <Arduino.h>

// 5V
// 10 bit

int a0 = 0;
int a1 = 0;
int a2 = 0;
int a3 = 0;
int a4 = 0;
int a5 = 0;

void setup() {
  Serial.begin(57600);
}

void loop() {
  a0 = analogRead(A0);
  a1 = analogRead(A1);
  a2 = analogRead(A2);
  a3 = analogRead(A3);
  a4 = analogRead(A4);
  a5 = analogRead(A5);

  String temp = ":" + String(a0) + "|" + String(a1) + "|" + String(a2) + "|" + String(a3) + "|" + String(a4) + "|" + String(a5);
  Serial.println(temp);

  delay(100);
}
