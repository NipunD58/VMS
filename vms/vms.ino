#include "arduino_secrets.h"
#include "thingProperties.h"
#include <DHT.h>
#include <Wire.h>
#include <Servo.h>
#define DHTPIN 8
#define DHTTYPE DHT11
#define WATER_LEVEL_PIN A2
#define TRIG_PIN 3
#define ECHO_PIN 5
#define RED_PIN 9
#define GREEN_PIN 10
// #define BLUE_PIN 11
#define gaspin 11
#include <NewPing.h>
#define MAX_DISTANCE 2000
bool status = false;

DHT dht(DHTPIN, DHTTYPE);
Servo myservo;

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 
  
void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  myservo.attach(6);
  myservo.write(115);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);  
  // pinMode(BLUE_PIN, OUTPUT);
  pinMode(WATER_LEVEL_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(gaspin, INPUT);
  dht.begin();
  digitalWrite(GREEN_PIN, HIGH);
    
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
  
  gas = digitalRead(gaspin);
  temp = dht.readTemperature();
  humidity = dht.readHumidity();
  wlevel = analogRead(WATER_LEVEL_PIN);
  myservo.write(50);
  delay(250);
  myservo.write(115);
  delay(250);
  myservo.write(170);
  delay(250);
  myservo.write(115);
  delay(100);
  // digitalWrite(TRIGGER_PIN, LOW);
  // delayMicroseconds(2);
  // digitalWrite(TRIGGER_PIN, HIGH);
  // delayMicroseconds(10);
  // digitalWrite(TRIGGER_PIN, LOW);
  // float duration = pulseIn(ECHO_PIN, HIGH);
  // distance = duration * 0.034 / 2;
  // delay(1000);
  
  distance = readPing();
  delay(100);
  
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print("°C, Humidity: ");
  Serial.print(humidity);
  Serial.print("%, Water Level: ");
  Serial.print(wlevel);
  Serial.print(", Gas: ");
  Serial.print(gas);
  Serial.print(", Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Alert system
  if (temp > 60 || humidity > 85 || wlevel > 800 || distance < 18 || gas == false) {
    // Red indication
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(RED_PIN, HIGH);
   //digitalWrite(BLUE_PIN, LOW);
    delay(500);
    status = false;
  }
  else {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, HIGH);
    delay(500);
    //digitalWrite(BLUE_PIN, LOW);
    status = true;
  }
}


int readPing() { 
  delay(70);
  int cm = sonar.ping_cm();
  if(cm==0)
  {
    cm = 250;
  }
  return cm;
}
