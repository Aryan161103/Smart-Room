#include <LiquidCrystal.h>
#include "DHT.h"
#define DHTTYPE DHT11
DHT dht(A0, DHTTYPE);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int buzzer = 9;
int led = 7;
int fan = 10;
int IRpin = 8;
int lamp = 13;
char data = 0;
void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(lamp, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(IRpin, INPUT);
  dht.begin();
  lcd.begin(16, 2); 
  lcd.clear();       
  lcd.print("Temperature:");
}
void loop() {
  float t = dht.readTemperature();
  if (t < 20 || t > 40)
    digitalWrite(buzzer, HIGH);
  else
    digitalWrite(buzzer, LOW);
  if (t > 35)
    digitalWrite(fan, HIGH);
  else if (t < 25)
    digitalWrite(fan, LOW);
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(t, 1);  
  lcd.print(" C");
  int irvalue = digitalRead(IRpin);
  if (irvalue == 0) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
  if (Serial.available() > 0) {
    data = Serial.read();
    if (data == '1')
      digitalWrite(led, HIGH);
    else if (data == '0')
      digitalWrite(led, LOW);
    else if (data == '3')
      digitalWrite(lamp, LOW);
    else if (data == '2')
      digitalWrite(lamp, HIGH);
    else if (data == '5')
      digitalWrite(fan, LOW);
    else if (data == '4')
      digitalWrite(fan, HIGH);
  }
  delay(1000);
}
