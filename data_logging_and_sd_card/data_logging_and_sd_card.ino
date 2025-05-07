// Write to SD card
#include <SD.h>
// Set by default for the SD card library
// MOSI = pin 11
// MISO = pin 12
// SCLK = pin 13
// We always need to set the CS Pin
const int CS_PIN = 10;
void setup()
{
  Serial.begin(9600);
  Serial.println("Initializing Card");
  // CS pin is an output
  pinMode(CS_PIN, OUTPUT);
  if (!SD.begin(CS_PIN))
  {
    Serial.println("Card Failure");
    return;
  }
  Serial.println("Card Ready");
}

void loop()
{
  long timeStamp = millis();
  String dataString = "Hello There!";
  // Open a file and write to it.
  File dataFile = SD.open("LOG.csv", FILE_WRITE);
  if (dataFile)
  {
    dataFile.print(timeStamp);
    dataFile.print(",");
    dataFile.println(dataString);
    dataFile.close(); // Data isn't actually written until we close the connection !
    // Print same thing to the screen for debugging
    Serial.print(timeStamp);
    Serial.print(",");
    Serial.println(dataString);
  }
  else
  {
    Serial.println("Couldn't open log file");
  }
  delay(5000);
}