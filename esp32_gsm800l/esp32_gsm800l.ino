//#include <SoftwareSerial.h>
//
////Create software serial object to communicate with SIM900
//SoftwareSerial mySerial(7, 8); //SIM900 Tx & Rx is connected to Arduino #7 & #8

void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  
  //Begin serial communication with Arduino and SIM900
  Serial2.begin(9600);

  Serial.println("Initializing..."); 
  delay(1000);
//
//  Serial2.println("AT"); //Handshaking with SIM900
//  updateSerial();
//
//  Serial2.println("AT+CMGF=1"); // Configuring TEXT mode
//  updateSerial();
// Serial2.println("AT+CMGS=\"+919986306133\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
//  updateSerial();
//  Serial2.print("Last Minute Engineers | lastminuteengineers.com"); //text content
//  updateSerial();
//  Serial2.write(26);

  Serial2.print("ATD + "); Serial2.print("+919986306133"); Serial2.println(";"); //Call to the specific number, ends with semi-colon,replace X with mobile number
  delay(20000);       // wait for 20 seconds...
//  Serial2.println("ATH"); //hang up
//  delay(100);
}

void loop()
{
  updateSerial();
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    Serial2.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  delay(1000);
  while(Serial2.available()) 
  {
    Serial.write(Serial2.read());//Forward what Software Serial received to Serial Port
  }
}
