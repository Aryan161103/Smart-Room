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
int co2Pin = A1;  // Pin for CO2 sensor (MG811)
char data = 0;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(lamp, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(IRpin, INPUT);
  dht.begin();
  lcd.begin(16, 2);  // Adjust these numbers for your LCD size
  lcd.clear();       // Clear the LCD initially
  lcd.print("Temperature:");
}

void loop() {
  // Read temperature from DHT sensor
  float t = dht.readTemperature();

  if (t < 20 || t > 40)
    digitalWrite(buzzer, HIGH);
  if (20 < t || t < 40)
    digitalWrite(buzzer, LOW);

  if (t > 35)
    digitalWrite(fan, HIGH);
  if (t < 25)
    digitalWrite(fan, LOW);

  // Read CO2 sensor value (analog)
  int co2Value = analogRead(co2Pin);
  float co2Voltage = co2Value * (5.0 / 1023.0);  // Convert the analog value to voltage
  
  // Display CO2 voltage on LCD
  lcd.setCursor(0, 1);
  lcd.print("CO2: ");
  lcd.print(co2Voltage, 2);  // Show voltage with 2 decimal places

  // If CO2 level is high, turn on buzzer and fan
  if (co2Voltage > 3.0) {  // Threshold value for high CO2
    digitalWrite(buzzer, HIGH);
    digitalWrite(fan, HIGH);
  } else {
    digitalWrite(buzzer, LOW);
    digitalWrite(fan, LOW);
  }

  // Read IR sensor value
  int irvalue = digitalRead(IRpin);
  if (irvalue == 0) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }

  // Check for incoming data from serial communication
  if (Serial.available() > 0) {
    data = Serial.read();
    // Control components based on received data
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

  // Add a delay to prevent rapid updates
  delay(1000);
}
