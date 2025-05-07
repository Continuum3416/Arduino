#include <SD.h>

// Set by default for the SD card library
// MOSI = pin 11
// MISO = pin 12
// SCLK = pin 13
// We always need to set the CS pin
const int CS_PIN = 10;
const int POW_PIN = 8;

// Default rate if file not found
int refresh_rate = 5000;

void setup()
{
    Serial.begin(9600);
    Serial.println("Initializing Card");

    pinMode(CS_PIN, OUTPUT);
    pinMode(POW_PIN, OUTPUT);
    digitalWrite(POW_PIN, HIGH);

    if (!SD.begin(CS_PIN))
    {
        Serial.println("Card Failure");
        return;
    }
    Serial.println("Card Ready");

    // Read the configuration information (speed.txt)
    File commandFile = SD.open("speed.txt");
    if (commandFile)
    {
        Serial.println("Reading Command File");
        while (commandFile.available())
        {
            refresh_rate = commandFile.parseInt();
        }
        Serial.print("Refresh Rate = ");
        Serial.print(refresh_rate);
        Serial.println("ms");
        commandFile.close();
    }
    else
    {
        Serial.println("Could not read command file.");
        return;
    }
}

void loop()
{
    long timeStamp = millis();
    String dataString = "Hello Don!";

    // Open a file and write to it.
    File dataFile = SD.open("LOG.csv", FILE_WRITE);
    if (dataFile)
    {
        dataFile.print(timeStamp);
        dataFile.print(",");
        dataFile.println(dataString);
        dataFile.close();

        // Debugging output to Serial Monitor
        Serial.print(timeStamp);
        Serial.print(",");
        Serial.println(dataString);
    }
    else
    {
        Serial.println("Couldn't open log file");
    }

    delay(refresh_rate);
}
