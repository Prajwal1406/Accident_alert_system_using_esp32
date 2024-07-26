#include <LiquidCrystal_I2C.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void displaylcd(String message) {
  lcd.clear(); 
  lcd.setCursor(1, 0);   
  lcd.print(message); 
}

void updateSerial() {
  delay(500);
  while (Serial.available()) {
    Serial1.write(Serial.read()); // Forward what Serial received to Software Serial Port
  }
  while (Serial1.available()) {
    Serial.write(Serial1.read()); // Forward what Software Serial received to Serial Port
  }
}

void sendsms() {
  Serial.println("Initiating Alert");
  displaylcd("Sending Help");
  Serial1.println("AT"); // Handshaking with SIM900
  updateSerial();
  Serial1.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  Serial1.println("AT+CMGS=\"+919731007397\""); // Change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  Serial1.print("Accident has occurred please help us"); // Text content
  updateSerial();
  Serial1.write(26);
  Serial.println("Sms Sent");
  displaylcd("Sms Sent");
}

void setup() {
  lcd.init();  
  lcd.backlight();  
  Serial1.begin(115200, SERIAL_8N1, 14, 12);
  Serial.begin(115200);

  Serial.println("Initializing...");
  delay(1000);
  
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(100);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Print out the values
  Serial.print("Acceleration X: ");
//  displaylcd("Acceleration X: ");
  Serial.print(a.acceleration.x);
//  displaylcd(String(a.acceleration.x));
  Serial.print(", Y: ");
//  displaylcd(", Y: ");
  Serial.print(a.acceleration.y);
//  displaylcd(String(a.acceleration.y));
  Serial.print(", Z: ");
//  displaylcd(", Z: ");
  Serial.print(a.acceleration.z);
//  displaylcd(String(a.acceleration.z));
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
//  displaylcd("Rotation X: ");
  Serial.print(g.gyro.x);
//  displaylcd(String(g.gyro.x));
  Serial.print(", Y: ");
//  displaylcd(", Y: ");
  Serial.print(g.gyro.y);
//  displaylcd(String(g.gyro.y));
  Serial.print(", Z: ");
//  displaylcd(", Z: ");
  Serial.print(g.gyro.z);
//  displaylcd(String(g.gyro.z));
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
//  displaylcd("Temperature: ");
  Serial.print(temp.temperature);
//  displaylcd(String(temp.temperature));
  Serial.println(" degC");

  // Check for accident condition (example threshold values)
  if (a.acceleration.x > 5.0 || a.acceleration.y > 1.0 || a.acceleration.z > 5.0 || a.acceleration.x < -5.0 || a.acceleration.y < -1.0 || a.acceleration.z < -5.0) {
    sendsms();
  }

  Serial.println("");
  delay(500);
}
