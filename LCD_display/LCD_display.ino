#include <LiquidCrystal.h>
// Initialize the LCD
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
void setup() {
  lcd.begin(16, 2);
  lcd.print("Pushed button: ");
  Serial.begin(9600);
}
void loop() {
  // Move cursor to second line, first position
  lcd.setCursor(0, 1);
  // Read the input on analog pin 0
  int sensorValue = analogRead(A0);
  lcd.print("                "); // Clear the previous button label
  lcd.setCursor(0, 1);
  // Determine which button is pressed based on sensor values
  if (sensorValue < 50) {
    lcd.print("RIGHT");
  } else if (sensorValue >= 100 && sensorValue < 200) {
    lcd.print("UP");
  } else if (sensorValue >= 250 && sensorValue < 300) {
    lcd.print("DOWN");
  } else if (sensorValue >= 400 && sensorValue < 450) {
    lcd.print("LEFT");
  } else if (sensorValue >= 600 && sensorValue < 700) {
    lcd.print("SELECT");
  } else {
    lcd.print("None");
  }
  Serial.println(sensorValue);
  delay(200);  // Delay for debounce
}
