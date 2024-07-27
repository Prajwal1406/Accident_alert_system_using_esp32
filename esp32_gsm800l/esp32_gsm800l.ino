//#include <SoftwareSerial.h>
//
////Create software serial object to communicate with SIM900
//SoftwareSerial mySerial(7, 8); //SIM900 Tx & Rx is connected to Arduino #7 & #8
#include <HardwareSerial.h>

HardwareSerial gsmSerial ( 1 );

void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  gsmSerial.begin ( 115200, SERIAL_8N1, 14, 12 );//14 is rx 12 in tx
  //Begin serial communication with Arduino and SIM900
//  Serial1.begin(115200);

  Serial.println("Initializing..."); 
  delay(1000);

  gsmSerial.println("AT"); //Handshaking with SIM900
  updateSerial();

  gsmSerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  gsmSerial.println("AT+CMGS=\"+919483088337\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  gsmSerial.print("Last Minute Engineers | lastminuteengineers.com"); //text content
  updateSerial();
  gsmSerial.write(26);
  delay(1000);
  // Call to the specific number
  gsmSerial.print("ATD+919483088337"); // Replace with the desired phone number
  delay(20000); // Wait for 20 seconds...
  gsmSerial.println("ATH"); // Hang up
  delay(100);
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
    gsmSerial.write(Serial.read()); // Forward what Serial received to Software Serial Port
  }
  delay(1000);
  while(gsmSerial.available()) 
  {
    Serial.write(gsmSerial.read()); // Forward what Software Serial received to Serial Port
  }
}
