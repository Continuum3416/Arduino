// for LCD
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

void lcdFunc() {
  lcd.clear();
  lcd.setCursor(0, 0);
  Serial.println("Lcd is on");
  lcd.print("Hello World");
  lcd.setCursor(2, 1);
  lcd.print("hi");
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(2000);
  lcdFunc();
}

void loop() {
  
}