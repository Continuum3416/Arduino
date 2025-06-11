// Motor Speed Control using a Potentiometer

const int MOTOR = 9;       // Motor on PWM-capable Digital Pin 9
const int POT_PIN = A0;    // Potentiometer connected to Analog Pin A0

void setup() {
  pinMode(MOTOR, OUTPUT);
  Serial.begin(9600); 
}

void loop() {
  int potValue = analogRead(POT_PIN);              // Read potentiometer value (0 - 1023)
  int motorSpeed = map(potValue, 0, 1023, 0, 255);  // Map it to PWM range (0 - 255)

  analogWrite(MOTOR, motorSpeed);                  // Set motor speed

  Serial.print("Potentiometer: ");
  Serial.print(potValue);
  Serial.print("  =>  Motor Speed (PWM): ");
  Serial.println(motorSpeed); 
  delay(100);                                        // Small delay to stabilize
}
