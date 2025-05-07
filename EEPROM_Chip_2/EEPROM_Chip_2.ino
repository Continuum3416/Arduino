#include <Wire.h>
#define disk1 0x50 // Change this if your EEPROM's I2C address is different

void setup() {
  Serial.begin(9600);
  Wire.begin();

  int valueToWrite = -25;
  unsigned int baseAddress = 0xA2;

  Serial.print("Writing integer value: ");
  Serial.println(valueToWrite);
  
  // Write integer to EEPROM
  writeIntEEPROM(disk1, baseAddress, valueToWrite);

  // Read it back
  int readValue = readIntEEPROM(disk1, baseAddress);

  Serial.print("Read back value: ");
  Serial.println(readValue);
}

void loop() {}

void writeIntEEPROM(int deviceaddress, unsigned int eeaddress, int value) {
  byte msb = (value >> 8) & 0xFF;
  byte lsb = value & 0xFF;

  // Write MSB
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // EEPROM address MSB
  Wire.write((int)(eeaddress & 0xFF)); // EEPROM address LSB
  Wire.write(msb);
  Wire.endTransmission();
  delay(10);

  // Write LSB to next address
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)((eeaddress + 1) >> 8));
  Wire.write((int)((eeaddress + 1) & 0xFF));
  Wire.write(lsb);
  Wire.endTransmission();
  delay(10);
}

int readIntEEPROM(int deviceaddress, unsigned int eeaddress) {
  byte msb = 0xFF;
  byte lsb = 0xFF;

  // Read MSB
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));
  Wire.write((int)(eeaddress & 0xFF));
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress, 1);
  if (Wire.available()) msb = Wire.read();

  // Read LSB
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)((eeaddress + 1) >> 8));
  Wire.write((int)((eeaddress + 1) & 0xFF));
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress, 1);
  if (Wire.available()) lsb = Wire.read();

  int value = ((int)msb << 8) | lsb;
  return value;
}
