#include <Wire.h>
#include "RTClib.h"
#include "SparkFunBME280.h"

// RTC and BME280 objects
RTC_DS1307 RTC;
BME280 mySensor;

// Timing
int refresh_rate = 1000;  // milliseconds

void setup() {
  Serial.begin(9600);
  Serial.println(F("Initializing System..."));

  // Initialize I2C
  Wire.begin();
  Wire.setClock(400000);  // Fast I2C

  // Initialize RTC
  RTC.begin();
  Serial.println(F("RTC is RUNNING!"));
  RTC.adjust(DateTime(__DATE__, __TIME__));

  // Initialize BME280
  mySensor.setI2CAddress(0x77);
  if (!mySensor.beginI2C()) {
    Serial.println("BME280 Sensor connection failed!");
  }

  mySensor.setFilter(1);
  mySensor.setStandbyTime(0);
  mySensor.setTempOverSample(1);
  mySensor.setPressureOverSample(1);
  mySensor.setHumidityOverSample(1);
  mySensor.setMode(MODE_NORMAL);
}

void loop() {
  // Get current date and time
  DateTime now = RTC.now();
  String date = String(now.year()) + "/" + String(now.month()) + "/" + String(now.day());
  String time = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());

  // Read BME280 sensor data
  float humidity = mySensor.readFloatHumidity();
  float pressure = mySensor.readFloatPressure();
  float altitude = mySensor.readFloatAltitudeFeet();
  float temperature = mySensor.readTempF();

  // Print to Serial Monitor
  Serial.print(date); Serial.print(" ");
  Serial.print(time); Serial.print(" | ");
  Serial.print("Humidity: "); Serial.print(humidity, 1); Serial.print("% | ");
  Serial.print("Pressure: "); Serial.print(pressure, 1); Serial.print(" Pa | ");
  Serial.print("Altitude: "); Serial.print(altitude, 1); Serial.print(" ft | ");
  Serial.print("Temp: "); Serial.print(temperature, 2); Serial.println(" °F");

  delay(refresh_rate);
}
