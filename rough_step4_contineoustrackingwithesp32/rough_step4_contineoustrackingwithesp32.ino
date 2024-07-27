#define BLYNK_TEMPLATE_ID "TMPL3pxI_z6v3"
#define BLYNK_TEMPLATE_NAME "AccidentAlertSystem"
#define BLYNK_AUTH_TOKEN "GA3iqHmcCDyV77UFmWjntSQogc67BfoV"
#define mq3pin 34
#include <WiFi.h>
#include <WiFiClient.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <BlynkSimpleEsp32.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
HardwareSerial gsmSerial ( 1 );
int GPSBaud = 9600;

TinyGPSPlus gps;

Adafruit_MPU6050 mpu;
LiquidCrystal_I2C lcd(0x27, 16, 2);

char ssid[] = "Jio TP"; // Replace with your WiFi SSID
char pass[] = "Hanuman@3009"; // Replace with your WiFi Password

// Motor driver pins
const int ena = 2;
const int in1 = 23;
const int in2 = 25;
const int in3 = 26;
const int in4 = 27;
const int enb = 4;

int sensorval;

float gpslat, gpslon, latitude, longitude;
int speed;
void sendlocation(){
  while (Serial2.available() > 0) {
    if (gps.encode(Serial2.read())) {
      gpslat = gps.location.lat();
      gpslon = gps.location.lng();
      gsmSerial.begin ( 115200, SERIAL_8N1, 14, 12 );
      Serial.println("Sending Location");
      gsmSerial.println("AT");
      delay(1000);
      gsmSerial.print("AT+CMGF=1\r");
      delay(1000);
      gsmSerial.print("AT+CMGS=\"+919483088337\"\r");
      delay(1000);
      // The text of the message to be sent.
      gsmSerial.print("Google Maps Link: ");
      gsmSerial.print("https://www.google.com/maps/search/?api=1&query=");
      gsmSerial.print(gpslat, 6);
      gsmSerial.print(",");
      gsmSerial.println(gpslon, 6);
      delay(1000);
      gsmSerial.write(0x1A);
      delay(1000);
    }
  }
}



void displaylcd(String message) {
  lcd.clear(); 
  lcd.setCursor(1, 0);   
  lcd.print(message); 
}

void updateSerial() {
  delay(500);
  while (Serial.available()) {
    gsmSerial.write(Serial.read()); // Forward what Serial received to Software Serial Port
  }
  while (gsmSerial.available()) {
    gsmSerial.write(gsmSerial.read()); // Forward what Software Serial received to Serial Port
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

void alcohol_stop(){
  sensorval = analogRead(mq3pin);
  if(sensorval>400){
    stop();
    Serial.println(sensorval);
  }
  else{
    Serial.println(sensorval);
    displaylcd("The person is not Drunk");
    Serial.println("The person is not Drunk");
  }
}

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
void gpscheck() {
  displaylcd("GPS...");
  Serial.println("Connecting to GPS");

  while (true) {
    while (Serial2.available() > 0) {
      if (gps.encode(Serial2.read())) {
        if (gps.location.isValid()) {
          displaylcd("GPS Enabled");
          delay(500);
          Serial.println("GPS Enabled");
          return;
        }
      }
    }
    delay(1000); 
    displaylcd("GPS...");
    Serial.println("Connecting to GPS");
  }
}


void gpsdetect(){
  while (Serial2.available() > 0)
    if (gps.encode(Serial2.read()))
      displayInfo();

  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    displaylcd("No GPS detected");
    while(true);
  }
}


void sendsms() {
  gsmSerial.println("AT"); // Handshaking with SIM900
  updateSerial();
  gsmSerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  gsmSerial.println("AT+CMGS=\"+919483088337\""); // Change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  gsmSerial.print("Accident has occurred please help us"); // Text content
  updateSerial();
  gsmSerial.write(26);
  displaylcd("Sms Sent"); // Display message on LCD
}
void gsmCheck() {
  displaylcd("SIM NETWORK...");
  Serial.println("Connecting to SIM NETWORK");
  gsmSerial.println("AT");
  delay(1000);

  while (true) {
    if (gsmSerial.available()) {
      String response = gsmSerial.readString();
      if (response.indexOf("OK") != -1) {
        displaylcd("GSM Enabled");
        delay(500);
        Serial.println("GSM Enabled");
        return; 
      }
    }
    gsmSerial.println("AT");
    delay(1000); 
    displaylcd("SIM NETWORK...");
    Serial.println("Connecting to SIM NETWORK");
  }
}


void setup() {
  lcd.init();  
  lcd.backlight();  
  gsmSerial.begin ( 115200, SERIAL_8N1, 14, 12 );
  Serial.begin(115200);
  delay(10);
  displaylcd("Connecting to ...");
  delay(100);
  Serial.println("Connecting to ...");
  displaylcd(ssid);
  delay(500);
  Serial.println(ssid);
  delay(100);
  WiFi.begin(ssid,pass);
  delay(100);
  int wifi_ctr = 0;
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  displaylcd("Wifi Connected");
  delay(1000);
  Serial.println("Wifi Connected");
  Serial2.begin(GPSBaud);
  gpscheck();
  gsmCheck();
  // Initialize Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  // Initialize motor driver pins
  pinMode(ena, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enb, OUTPUT);
  pinMode(mq3pin,INPUT);
  
  Serial.println("Initializing...");
  delay(1000);
  
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  displaylcd("MPU6050 Found!");
  delay(500);
  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(100);
}

void loop() {
  Blynk.run();
  gpsdetect();
  alcohol_stop();
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Print out the values
  Serial.print("Acceleration X: ");
  displaylcd("Acceleration X: ");
  Serial.print(a.acceleration.x);
  displaylcd(String(a.acceleration.x));
  Serial.print(", Y: ");
  displaylcd(", Y: ");
  Serial.print(a.acceleration.y);
  displaylcd(String(a.acceleration.y));
  Serial.print(", Z: ");
  displaylcd(", Z: ");
  Serial.print(a.acceleration.z);
  displaylcd(String(a.acceleration.z));
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  displaylcd("Rotation X: ");
  Serial.print(g.gyro.x);
  displaylcd(String(g.gyro.x));
  Serial.print(", Y: ");
  displaylcd(", Y: ");
  Serial.print(g.gyro.y);
  displaylcd(String(g.gyro.y));
  Serial.print(", Z: ");
  displaylcd(", Z: ");
  Serial.print(g.gyro.z);
  displaylcd(String(g.gyro.z));
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  displaylcd("Temperature: ");
  Serial.print(temp.temperature);
  displaylcd(String(temp.temperature));
  Serial.println(" degC");

  if (a.acceleration.x > 5.0 || a.acceleration.y > 5.0 || a.acceleration.z > 5.0) {
    sendsms();
    sendlocation();
  }

  Serial.println("");
  delay(500);
}
BLYNK_WRITE(V1) {
  int motorSpeed = param.asInt();
  analogWrite(ena, motorSpeed);
  analogWrite(enb, motorSpeed);
  }
  
BLYNK_WRITE(V2) {
  int direction = param.asInt();
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
