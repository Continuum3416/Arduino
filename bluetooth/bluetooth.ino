/*
Program that uses Arduino Blue to send text to turn on and off an
LED
connected to pin 13. A 1 will turn the LED on and a 0 will turn it
off. All other text is ignored. There is also code that will send
information typed on the serial monitor to the connected device
(phone)in most cases.
Based on code at
https://sites.google.com/stonybrook.edu/arduinoble/home?authuser=0
*/
#include <SoftwareSerial.h>
#include <ArduinoBlue.h>
#include <string.h>
// The bluetooth tx and rx pins must be supported by software serial.
// Visit https://www.arduino.cc/en/Reference/SoftwareSerial for unsupported pins.
  // Bluetooth TX -> Arduino D8
const int BLUETOOTH_TX = 8;
// Bluetooth RX -> Arduino D7
const int BLUETOOTH_RX = 7;
// LED on breadboard and on Arduino
const int LED = 13;
int strlength;
SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoBlue phone(bluetooth);  // pass reference of bluetooth object to ArduinoBlue constructor.


// Setup code runs once after program starts.
void
  setup() {
  // Set up LED pin
  pinMode(LED, OUTPUT);
  // Start serial monitor at 9600 bps.
  Serial.begin(9600);
  // Start bluetooth serial at 9600 bps.
  bluetooth.begin(9600);
  // delay just in case bluetooth module needs time to "get ready".
  delay(100);
  Serial.println("setup complete");
}

// Put your main code here, to run repeatedly:
void loop() {
  // Returns the text data sent from the phone.
  // After it returns the latest data, empty string "" is sent in subsequent.
  // calls until text data is sent again.
  String str = phone.getText();  // str is a C++ String object
  strlength = str.length();
  if (strlength != 0) {
    for (int m = 0; m < strlength; m++) {
      Serial.print(str[m]);
      if (str[m] == '0')
        digitalWrite(LED, LOW);
      if (str[m] == '1')
        digitalWrite(LED, HIGH);
    }
  }
  // Send string from serial command line to the phone. This will
  // alert the user.
  if (Serial.available()) {
    Serial.write("send: ");
    String str = Serial.readString();
    phone.sendMessage(str);  // phone.sendMessage(str) sends the text to the phone.Serial.print(str);
    Serial.write('\n');
  }
}