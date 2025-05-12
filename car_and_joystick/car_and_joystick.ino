int joyXpin = A0;
int joyYpin = A1;

int xVal;
int yVal;

int ENApin = 5;        // speed for right motor
int IN1pin = 6;        // motor right forward
int IN2pin = 7;        // motor right backward

int IN3pin = 8;        // motor left forward
int IN4pin = 9;        // motor left backward
int ENBpin = 10;       // speed for left motor


void setup() {
  pinMode(joyXpin, INPUT);
  pinMode(joyYpin, INPUT);
  pinMode(IN1pin, OUTPUT);
  pinMode(IN2pin, OUTPUT);
  pinMode(IN3pin, OUTPUT);
  pinMode(IN4pin, OUTPUT);
  pinMode(ENBpin, OUTPUT);
  pinMode(ENApin, OUTPUT);
}

void loop() {
  xVal = analogRead(joyXpin); 
  yVal = analogRead(joyYpin);

  // Reset all motors (default to off)
  digitalWrite(IN1pin, LOW);
  digitalWrite(IN2pin, LOW);
  digitalWrite(IN3pin, LOW);
  digitalWrite(IN4pin, LOW);
  analogWrite(ENApin, 0);
  analogWrite(ENBpin, 0);

  // Joystick - forward (up)
  if (yVal < 480) {
    int motorSpeed = map(yVal, 480, 0, 70, 255);
    digitalWrite(IN1pin, HIGH);
    digitalWrite(IN2pin, LOW);
    digitalWrite(IN3pin, HIGH);
    digitalWrite(IN4pin, LOW);
    analogWrite(ENApin, motorSpeed);
    analogWrite(ENBpin, motorSpeed);
  }
  // Joystick - backward (down)
  else if (yVal > 540) {
    int motorSpeed = map(yVal, 540, 1023, 70, 255);
    digitalWrite(IN1pin, LOW);
    digitalWrite(IN2pin, HIGH);
    digitalWrite(IN3pin, LOW);
    digitalWrite(IN4pin, HIGH);
    analogWrite(ENApin, motorSpeed);
    analogWrite(ENBpin, motorSpeed);
  }
  // Joystick - left (sharp turn with differential steering)
  else if (xVal < 480) {
    int motorSpeed = map(xVal, 480, 0, 150, 255);   // Higher minimum speed for sharper turn
    digitalWrite(IN1pin, LOW);                       // Right motor backward
    digitalWrite(IN2pin, HIGH);
    digitalWrite(IN3pin, HIGH);                      // Left motor forward
    digitalWrite(IN4pin, LOW);
    analogWrite(ENApin, motorSpeed);                 // Right motor at increased speed
    analogWrite(ENBpin, motorSpeed);                 // Left motor at increased speed
  }
  // Joystick - right (sharp turn with differential steering)
  else if (xVal > 540) {
    int motorSpeed = map(xVal, 540, 1023, 150, 255); // Higher minimum speed for sharper turn
    digitalWrite(IN1pin, HIGH);                      // Right motor forward
    digitalWrite(IN2pin, LOW);
    digitalWrite(IN3pin, LOW);                       // Left motor backward
    digitalWrite(IN4pin, HIGH);
    analogWrite(ENApin, motorSpeed);                 // Right motor at increased speed
    analogWrite(ENBpin, motorSpeed);                 // Left motor at increased speed
  }
  

}