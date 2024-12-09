```cpp
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
int co2Sensor = A1;
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
  lcd.print("Monitoring...");
}

void loop() {
  float t = dht.readTemperature();
  int co2Level = analogRead(co2Sensor);

  digitalWrite(buzzer, (t < 20 || t > 40 || co2Level > 800) ? HIGH : LOW);
  digitalWrite(fan, (t > 35 || co2Level > 600) ? HIGH : LOW);

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print(" C  ");

  lcd.setCursor(0, 1);
  lcd.print("CO2: ");
  lcd.print(co2Level);
  lcd.print(" ppm  ");

  int irValue = digitalRead(IRpin);
  digitalWrite(led, irValue == 0 ? HIGH : LOW);

  if (Serial.available() > 0) {
    data = Serial.read();
    digitalWrite(led, (data == '1') ? HIGH : (data == '0') ? LOW : digitalRead(led));
    digitalWrite(lamp, (data == '2') ? HIGH : (data == '3') ? LOW : digitalRead(lamp));
    digitalWrite(fan, (data == '4') ? HIGH : (data == '5') ? LOW : digitalRead(fan));
  }

  delay(1000);
}
```
