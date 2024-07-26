#define BLYNK_TEMPLATE_ID "TMPL3pxI_z6v3"
#define BLYNK_TEMPLATE_NAME "AccidentAlertSystem"
#define BLYNK_AUTH_TOKEN "GA3iqHmcCDyV77UFmWjntSQogc67BfoV"
#include <WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <BlynkSimpleEsp32.h>
#include <TinyGPS++.h>
int GPSBaud = 9600;

// The TinyGPSPlus object
float gpslat, gpslon, latitude, longitude;
int speed;

TinyGPSPlus gps;

char ssid[] = "Jio TP"; // Replace with your WiFi SSID
char pass[] = "Hanuman@3009"; // Replace with your WiFi Password

const int ena = 2;
const int in1 = 23;
const int in2 = 25;
const int in3 = 26;
const int in4 = 27;
const int enb = 4;


void displayInfo()
{
  if (gps.location.isValid())
  {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
    latitude = (gps.location.lat()); 
    longitude = (gps.location.lng());
    speed = gps.speed.kmph(); 
    Blynk.virtualWrite(V3, String(latitude, 6));
    Blynk.virtualWrite(V4, String(longitude, 6));
    Blynk.virtualWrite(V5, speed);
  }
  else
  {
    Serial.println("Location: Not Available");
  }
  
  Serial.print("Date: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.println(gps.date.year());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.print("Time: ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.println(gps.time.centisecond());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.println();
  Serial.println();
  delay(1000);
}

void gpsdetect(){
  while (Serial1.available() > 0)
    if (gps.encode(Serial1.read()))
      displayInfo();

  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    while(true);
  }
}

void setup() {
  Serial1.begin(GPSBaud, SERIAL_8N1, 14, 12);
  Serial.begin(115200);
  delay(10);
  Serial.println("Connecting to ...");
  Serial.println(ssid);
  delay(100);
  WiFi.begin(ssid,pass);
  delay(100);
  int wifi_ctr = 0;
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi Connected");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  delay(3000);


}


void loop() {
  Blynk.run();
  gpsdetect();
}
BLYNK_WRITE(V1) {
  
  int motorSpeed = param.asInt();
  Serial.println(motorSpeed);
  analogWrite(ena, motorSpeed);
  analogWrite(enb, motorSpeed);
  }
  
BLYNK_WRITE(V2) {
  int direction = param.asInt();
  Serial.println(direction);
  if (direction == 1) {
    forward();
  } else if (direction == 2) {
    backward();
  } else if (direction == 3) {
    left();
  } else if (direction == 4) {
    right();
  } else {
    stop();
  }
}

void forward(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void backward(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
void left(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void right(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
void stop(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
