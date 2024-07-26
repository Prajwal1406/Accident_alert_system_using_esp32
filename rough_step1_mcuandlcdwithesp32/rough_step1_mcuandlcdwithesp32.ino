#include <LiquidCrystal_I2C.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

LiquidCrystal_I2C lcd(0x68, 16, 2);

void displaylcd(String message) {
  lcd.clear(); 
  lcd.setCursor(1, 0);   
  lcd.print(message); 
}

void setup() {
  lcd.init();  
  lcd.backlight();  
  displaylcd("hello");
  Serial.begin(115200);
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  // set filter bandwidth to 21 Hz
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  // set gyro range to +- 500 deg/s
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  // set filter bandwidth to 21 Hz
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(100);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
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

  Serial.println("");
  delay(500);
}
