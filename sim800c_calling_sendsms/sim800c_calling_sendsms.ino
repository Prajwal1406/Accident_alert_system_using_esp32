#include <SoftwareSerial.h>

// Define SIM800C module connections
#define SIM800C_TX_PIN 9 
#define SIM800C_RX_PIN 8  

// Create a software serial object 
SoftwareSerial sim800c(SIM800C_RX_PIN, SIM800C_TX_PIN);

// Phone number
char phoneNumber[] = "+919483088337"; 

// Function to send AT commands and check responses (improved)
bool sendATCommand(char* command, char* expectedResponse, unsigned int timeout) {
  sim800c.println(command); 
  unsigned long startTime = millis();

  // Buffer to store the incoming response
  char responseBuffer[64] = ""; 
  int bufferIndex = 0; 

  while (millis() - startTime < timeout) {
    if (sim800c.available()) {
      char receivedChar = sim800c.read();

      // Store response in the buffer (preventing overflow)
      if (bufferIndex < sizeof(responseBuffer) - 1) { 
        responseBuffer[bufferIndex++] = receivedChar;
        responseBuffer[bufferIndex] = '\0'; 
      }

      // Check for the expected response anywhere in the buffer
      if (strstr(responseBuffer, expectedResponse) != NULL) {
        return true; 
      }
    }
  }
  return false; // Command timed out
}

void setup() {
  Serial.begin(9600); 
  sim800c.begin(9600); 

  Serial.println("Initializing SIM800C...");
  delay(1000); // Wait for SIM800C to initialize

  // Send a test AT command to check for a response
  Serial.print("Waiting for module response... ");
  if (sendATCommand("AT", "OK", 2000)) {
    Serial.println("SIM800C Ready");
  } else {
    Serial.println("SIM800C Not Responding!");
    // You can choose to halt the program here or add error handling
    while(true);
  }

  Serial.println("Type:\n s) Send SMS \n r) Receive SMS\n c) Make a Call");
}

void loop() {
  if (Serial.available() > 0) {
    switch (Serial.read()) {
      case 's':
        SendMessage();
        break;
      case 'r':
        ReceiveMessage();
        break;
      case 'c':
        CallNumber();
        break;
    }
  }

  // Check for incoming messages or call status
  if (sim800c.available()) {
    Serial.write(sim800c.read()); // Print to serial monitor
  }
}

void CallNumber() {
  // Set the speaker volume (optional)
  sim800c.println("AT+CLVL=80"); // Set speaker volume (0-100)
  delay(500);

  // Dial the number
  sim800c.print("ATD");
  sim800c.print(phoneNumber);
  sim800c.println(";"); // The semicolon is important for voice call
  Serial.println("Calling...");

  // Wait for a while to let the call go through
  delay(30000); // Adjust call duration as needed

  // Hang up the call
  sim800c.println("ATH");
  Serial.println("Call Ended");
}


void SendMessage() {
  sim800c.print("AT+CMGF=1\r"); // Text mode
  delay(500);
  sim800c.print("AT+CMGS=\""); 
  sim800c.print(phoneNumber);
  sim800c.print("\"\r"); 
  delay(500);
  sim800c.print("Hello from Arduino!\r");  // Message content
  delay(100);
  sim800c.write(26); // Ctrl+Z (ASCII 26) to send
  delay(500); 
  Serial.println("SMS Sent"); 
}

void ReceiveMessage() {
  sim800c.println("AT+CMGF=1\r"); // Text mode
  sim800c.println("AT+CNMI=2,2,0,0,0\r"); // Set message indication 
  Serial.println("Waiting for SMS..."); 
}
