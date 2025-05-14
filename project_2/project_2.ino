// Author: Don D. Le
// 5/13/2025

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "SparkFunBME280.h"
#include <SD.h>
#include <RTClib.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

// ------------------ Motor & Joystick ------------------
#define ENA 10
#define IN1 9
#define IN2 8
#define ENB 5
#define IN3 6
#define in4 7
#define joyX A0
#define joyY A1
const int deadZone = 20;
float speedMultiplier = 0.5;

// ------------------ Sensor & Logging ------------------
BME280 Sensor;
RTC_DS1307 RTC;
LiquidCrystal_I2C lcd(0x27, 16, 2);
unsigned long lastSensorUpdate = 0;
const unsigned long sensorInterval = 1000;

// ------------------ GPS ------------------
SoftwareSerial gpsSerial(A3, 2);  // RX=A3, TX=2
TinyGPS gps;
long lat = 0, lon = 0;
bool gpsFix = false;

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  Wire.begin();

  // --- Motor Pins ---
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(in4, OUTPUT);

  // --- LCD Setup ---
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");

  // --- RTC Setup ---
  RTC.begin();
  RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));

  // --- BME280 Setup ---
  Sensor.setI2CAddress(0x77);
  if (!Sensor.beginI2C()) {
    lcd.setCursor(0, 1);
    lcd.print("Sensor FAIL");
    while (1);
  }
  Sensor.setFilter(0);
  Sensor.setStandbyTime(0);
  Sensor.setTempOverSample(1);
  Sensor.setPressureOverSample(1);
  Sensor.setHumidityOverSample(1);
  Sensor.setMode(MODE_NORMAL);

  // --- SD Setup ---
  if (!SD.begin(3)) {
    lcd.setCursor(0, 1);
    lcd.print("SD FAIL");
    while (1);
  }
  if (!SD.exists("LOG.csv")) {
    File dataFile = SD.open("LOG.csv", FILE_WRITE);
    if (dataFile) {
      dataFile.println(F("Date,Time,Hum(%),Press(Pa),Alt(ft),Temp(F),Lat,Lon"));
      dataFile.close();
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  delay(1000);
}

void loop() {
  // ---- Joystick Car Control ----
  int rawX = analogRead(joyX);
  int rawY = analogRead(joyY);
  int x = rawX - 512;
  int y = rawY - 512;

  if (abs(x) < deadZone) x = 0;
  if (abs(y) < deadZone) y = 0;

  int speedA = 0, speedB = 0;

  if (y != 0) {
    bool forward = (y > 0);
    int baseSpeed = map(abs(y), 0, 512, 0, 255) * speedMultiplier;

    if (x < 0) {
      speedA = map(abs(x), 0, 512, baseSpeed, 0);
      speedB = baseSpeed;
    } else if (x > 0) {
      speedA = baseSpeed;
      speedB = map(abs(x), 0, 512, baseSpeed, 0);
    } else {
      speedA = baseSpeed;
      speedB = baseSpeed;
    }

    setMotorA(forward);
    setMotorB(forward);
  } else if (x != 0) {
    int turnSpeed = map(abs(x), 0, 512, 0, 255) * speedMultiplier;
    if (x < 0) {
      setMotorA(false);
      setMotorB(true);
    } else {
      setMotorA(true);
      setMotorB(false);
    }
    speedA = turnSpeed;
    speedB = turnSpeed;
  }
  else {
    stopMotors();
  }

  analogWrite(ENA, speedA);
  analogWrite(ENB, speedB);

  // ---- GPS Decode Loop (non-blocking) ----
  while (gpsSerial.available()) {
    if (gps.encode(gpsSerial.read())) {
      gps.get_position(&lat, &lon);
      gpsFix = (lat != 0 || lon != 0);
    }
  }

  // ---- Sensor Reading + Logging every 1s ----
  if (millis() - lastSensorUpdate >= sensorInterval) {
    lastSensorUpdate = millis();

    DateTime now = RTC.now();
    float humidity = Sensor.readFloatHumidity();
    float pressure = Sensor.readFloatPressure();
    float altitude = Sensor.readFloatAltitudeFeet();
    float tempF = Sensor.readTempF();

    // Serial output
    printDateTime(now);
    Serial.print("H: "); Serial.print(humidity);
    Serial.print(" P: "); Serial.print(pressure);
    Serial.print(" A: "); Serial.print(altitude);
    Serial.print(" T: "); Serial.print(tempF);
    if (gpsFix) {
      Serial.print(" LAT: "); Serial.print(lat);
      Serial.print(" LON: "); Serial.println(lon);
    } else {
      Serial.println(" GPS: N/A");
    }

    // LCD (for environmental data only)
    lcd.setCursor(0, 0); lcd.print("                ");
    lcd.setCursor(0, 1); lcd.print("                ");
    lcd.setCursor(0, 0);
    printTimeToLCD(lcd, now);
    lcd.print(" H");
    lcd.print((int)humidity);
    lcd.print(" P");
    lcd.print((int)(pressure / 100.0 / 10));

    lcd.setCursor(0, 1);
    lcd.print("A");
    lcd.print((int)altitude);
    lcd.print(" T");
    lcd.print(tempF, 1);

    // SD Logging
    File dataFile = SD.open("LOG.csv", FILE_WRITE);
    if (dataFile) {
      dataFile.print(now.year()); dataFile.print('/');
      printPaddedNumber(dataFile, now.month()); dataFile.print('/');
      printPaddedNumber(dataFile, now.day()); dataFile.print(',');
      printPaddedNumber(dataFile, now.hour()); dataFile.print(':');
      printPaddedNumber(dataFile, now.minute()); dataFile.print(':');
      printPaddedNumber(dataFile, now.second()); dataFile.print(',');
      dataFile.print(humidity); dataFile.print(',');
      dataFile.print(pressure); dataFile.print(',');
      dataFile.print(altitude); dataFile.print(',');
      dataFile.print(tempF); dataFile.print(',');
      if (gpsFix) {
        dataFile.print(lat); dataFile.print(',');
        dataFile.println(lon);
      } else {
        dataFile.println("N/A,N/A");
      }
      dataFile.close();
    } else {
      Serial.println(F("Log err"));
    }
  }
}

// ------------------ Motor Functions ------------------
void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(in4, LOW);
  analogWrite(ENA, 0); analogWrite(ENB, 0);
}

void setMotorA(bool forward) {
  digitalWrite(IN1, forward ? HIGH : LOW);
  digitalWrite(IN2, forward ? LOW : HIGH);
}

void setMotorB(bool forward) {
  digitalWrite(IN3, forward ? HIGH : LOW);
  digitalWrite(in4, forward ? LOW : HIGH);
}

// ------------------ Utility Functions ------------------
void printDateTime(DateTime dt) {
  Serial.print(dt.year()); Serial.print('/');
  printPaddedNumber(Serial, dt.month()); Serial.print('/');
  printPaddedNumber(Serial, dt.day()); Serial.print(' ');
  printPaddedNumber(Serial, dt.hour()); Serial.print(':');
  printPaddedNumber(Serial, dt.minute()); Serial.print(':');
  printPaddedNumber(Serial, dt.second()); Serial.print(" | ");
}

void printTimeToLCD(LiquidCrystal_I2C& lcd, DateTime dt) {
  printPaddedNumber(lcd, dt.hour()); lcd.print(':');
  printPaddedNumber(lcd, dt.minute()); lcd.print(':');
  printPaddedNumber(lcd, dt.second());
}

void printPaddedNumber(Print& output, int value) {
  if (value < 10) output.print('0');
  output.print(value);
}