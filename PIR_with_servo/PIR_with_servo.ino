#include <Servo.h>

const int SERVO_PIN = 9;
const int PIR_PIN = 3;
const int BUZZER_PIN = 6;

Servo servo;
bool isOccupied = false;
unsigned long lastMotionTime = 0;
const unsigned long emptyTimeout = 5000; // milliseconds

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  servo.attach(SERVO_PIN);
  Serial.begin(9600);
  
  servo.write(0); // Start at "Empty" position
}

void loop() {
  if (digitalRead(PIR_PIN) == HIGH) { 
    Serial.println("Motion detected! Room is occupied.");
    
    if (!isOccupied) {
      isOccupied = true;
      servo.write(45); // Move servo to "Occupied" sign
      tone(BUZZER_PIN, 1000); // Play warning tone
      delay(500);
      noTone(BUZZER_PIN);
    }
    
    lastMotionTime = millis();
  }

  // Check if 30 seconds have passed with no motion
  unsigned long currentTime = millis();
  if (isOccupied && currentTime - lastMotionTime > emptyTimeout) {
    Serial.println("No motion for 30 seconds. Room is empty.");
    servo.write(0); // Move servo to "Empty" sign
    isOccupied = false;
  }

  delay(100);
}
