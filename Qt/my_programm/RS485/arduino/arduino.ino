//-------------------------------------------------------
//
//-------------------------------------------------------
int pin_485 = 8;
int led_0 = 10;
int led_1 = 11;
int led_2 = 12;
//-------------------------------------------------------
void setup() {
  Serial.begin(9600);

  pinMode(pin_485, OUTPUT);
  pinMode(led_0, OUTPUT);
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
}
//-------------------------------------------------------
void loop() {
  digitalWrite(led_0, HIGH);  
  digitalWrite(led_1, HIGH);  
  digitalWrite(led_2, HIGH);  
  delay(1000);
  digitalWrite(led_0, LOW);  
  digitalWrite(led_1, LOW);  
  digitalWrite(led_2, LOW);  
  delay(1000);

  digitalWrite(pin_485, HIGH);
  Serial.println("HIGH");
  digitalWrite(pin_485, LOW);  
  Serial.println("LOW");
}
//-------------------------------------------------------
