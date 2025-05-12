#include <Wire.h>
#include <SD.h>
#include "RTClib.h"
#include "SparkFunBME280.h"

// --- Pin Definitions ---
const int CS_PIN = 10;      // SD card Chip Select

// --- Objects ---
RTC_DS1307 RTC;
BME280 mySensor;

// --- Configuration ---
const int refresh_rate = 5000;  // in milliseconds

void setup() {
  Serial.begin(9600);
  Serial.println(F("Initializing System..."));

  // --- Initialize I2C ---
  Wire.begin();
  Wire.setClock(400000);

  // --- RTC Setup ---
  RTC.begin();

  // Uncomment this line ONCE to set the RTC time to the current compile time
  // RTC.adjust(DateTime(__DATE__, __TIME__));

  if (!RTC.isrunning()) {
    Serial.println(F("RTC is NOT running! Set it using RTC.adjust()."));
  }

  // --- BME280 Setup ---
  mySensor.setI2CAddress(0x77);
  if (!mySensor.beginI2C()) {
    Serial.println(F("BME280 connection failed!"));
  }
  mySensor.setFilter(1);
  mySensor.setStandbyTime(0);
  mySensor.setTempOverSample(1);
  mySensor.setPressureOverSample(1);
  mySensor.setHumidityOverSample(1);
  mySensor.setMode(MODE_NORMAL);

  // --- SD Card Setup ---
  pinMode(CS_PIN, OUTPUT);
  if (!SD.begin(CS_PIN)) {
    Serial.println(F("SD card initialization failed!"));
    return;
  }
  Serial.println(F("SD card ready."));

  // --- Create Header in log.csv ---
  File dataFile = SD.open("log.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println(F("Date,Time,Humidity (%),Pressure (Pa),Altitude (ft),Temperature (F)"));
    dataFile.close();
  } else {
    Serial.println(F("Couldn't open log.csv for writing."));
  }
}

void loop() {
  // --- Get Current Time ---
  DateTime now = RTC.now();
  String date = String(now.year()) + "/" + String(now.month()) + "/" + String(now.day());
  String time = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());

  // --- Read Sensor Data ---
  float humidity = mySensor.readFloatHumidity();
  float pressure = mySensor.readFloatPressure();
  float altitude = mySensor.readFloatAltitudeFeet();
  float temperature = mySensor.readTempF();

  // --- Format CSV Line ---
  String csvLine = date + "," + time + "," +
                   String(humidity, 1) + "," +
                   String(pressure, 1) + "," +
                   String(altitude, 1) + "," +
                   String(temperature, 2);

  // --- Print to Serial ---
  Serial.println(csvLine);

  // --- Log to SD ---
  File dataFile = SD.open("log.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println(csvLine);
    dataFile.close();
  } else {
    Serial.println(F("Couldn't open log.csv to write data."));
  }

  delay(refresh_rate);
}
