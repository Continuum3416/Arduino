#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0, 0);
  Serial.println("Lcd is on");
  lcd.print("Hello World");
  lcd.setCursor(0, 1);
  lcd.print("hi");
}

void loop() {
  // Nothing here unless you want to update later
}
