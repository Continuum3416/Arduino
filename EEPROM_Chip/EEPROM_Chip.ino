#include <Wire.h>
#define disk1 0x50 // I2C address of 24LC256 EEPROM

void setup(void) {
  Serial.begin(9600);
  Wire.begin();

  // Start writing at EEPROM memory address 0x00A2 (162 decimal)
  unsigned int baseAddress = 0x00A2;
  byte valuesToWrite[4] = {232, 19, 158, 38};

  Serial.println("Writing to EEPROM...");
  for (int i = 0; i < 4; i++) {
    writeEEPROM(disk1, baseAddress + i, valuesToWrite[i]);
    Serial.print("Wrote ");
    Serial.print(valuesToWrite[i]);
    Serial.print(" to address 0x");
    Serial.println(baseAddress + i, HEX);
  }

  Serial.println("Reading back from EEPROM...");
  for (int i = 0; i < 4; i++) {
    byte val = readEEPROM(disk1, baseAddress + i);
    Serial.print("Address 0x");
    Serial.print(baseAddress + i, HEX);
    Serial.print(" contains: ");
    Serial.println(val);
  }
}

void loop() {}

void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data) {
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();
  delay(5); // Allow time for EEPROM write cycle
}

byte readEEPROM(int deviceaddress, unsigned int eeaddress) {
  byte rdata = 0xFF;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress, 1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}
