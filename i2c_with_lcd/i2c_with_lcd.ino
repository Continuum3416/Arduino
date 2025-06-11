#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define BUTTON_PIN 2
const int temp_address = 72; // 0x48

LiquidCrystal_I2C lcd(0x27, 16, 2);

bool showCelsius = true;
bool buttonPressed = false;
unsigned long lastButtonTime = 0;
const unsigned long debounceDelay = 200;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();

  pinMode(BUTTON_PIN, INPUT); // No pull-up
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == HIGH && !buttonPressed) {
    if (millis() - lastButtonTime > debounceDelay) {
      showCelsius = !showCelsius;
      buttonPressed = true;
      lastButtonTime = millis();
      Serial.println("Toggled");
    }
  } else if (buttonState == LOW) {
    buttonPressed = false;
  }

  // Read temperature
  Wire.beginTransmission(temp_address);
  Wire.write(0);
  Wire.endTransmission();

  Wire.requestFrom(temp_address, 1);
  while (Wire.available() == 0);
  int c = Wire.read();
  int f = round(c * 9.0 / 5.0 + 32.0);

  lcd.setCursor(0, 0);
  lcd.print("Temp:       ");
  lcd.setCursor(6, 0);

  if (showCelsius) {
    lcd.print(c);
    lcd.print((char)223);
    lcd.print("C");
  } else {
    lcd.print(f);
    lcd.print((char)223);
    lcd.print("F");
  }

  delay(500);
}
