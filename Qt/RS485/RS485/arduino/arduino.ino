
#include "Timer.h"

Timer t;

int ledPin0 = A1;
int ledPin1 = A2;
int ledPin2 = A3;
int ledPin3 = A4;
int ledPin4 = A5;
int ledPin5 = A6;
int ledPin6 = A7;
int ledPin7 = A8;

void setup()
{
  pinMode(ledPin0, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  pinMode(ledPin5, OUTPUT);
  pinMode(ledPin6, OUTPUT);
  pinMode(ledPin7, OUTPUT);
  
  Serial1.begin(9600);
  
  t.every(500, doSomething);
}


char flag = 0;
void doSomething()
{
  flag =!flag;
  if(flag)
    set_all_ON();
  else
    set_all_OFF();  
}

void set_all_ON()
{
  digitalWrite(ledPin0, HIGH);
  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, HIGH);
  digitalWrite(ledPin3, HIGH);
  digitalWrite(ledPin4, HIGH);
  digitalWrite(ledPin5, HIGH);
  digitalWrite(ledPin6, HIGH);
  digitalWrite(ledPin7, HIGH);
}  

void set_all_OFF()
{
  digitalWrite(ledPin0, LOW);
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);
  digitalWrite(ledPin5, LOW);
  digitalWrite(ledPin6, LOW);
  digitalWrite(ledPin7, LOW);
}

void test()
{
  set_all_ON(); 
  delay(500);
  set_all_OFF(); 
  delay(500);
}

void loop()
{
  //Serial1.println("test");
  //delay(500);
  
  Serial1.println("loop");
   t.update();
}

