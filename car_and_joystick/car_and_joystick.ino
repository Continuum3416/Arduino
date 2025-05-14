// Motor A pins
#define ENA 10
#define IN1 9
#define IN2 8

// Motor B pins
#define ENB 5
#define IN3 6
#define in4 7

// Joystick pins
#define joyX A0
#define joyY A1

const int deadZone = 20;      // Joystick dead zone
float speedMultiplier = 0.8;  // Global speed control (0.0 to 1.0)

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(in4, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  int rawX = analogRead(joyX);
  int rawY = analogRead(joyY);
  int x = rawX - 512;
  int y = rawY - 512;

  if (abs(x) < deadZone) x = 0;
  if (abs(y) < deadZone) y = 0;

  int speedA = 0;
  int speedB = 0;

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
  }
  else if (x != 0) {
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
    speedA = 0;
    speedB = 0;
    stopMotors();
  }

  analogWrite(ENA, speedA);
  analogWrite(ENB, speedB);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void setMotorA(bool forward) {
  digitalWrite(IN1, forward ? HIGH : LOW);
  digitalWrite(IN2, forward ? LOW : HIGH);
}

void setMotorB(bool forward) {
  digitalWrite(IN3, forward ? HIGH : LOW);
  digitalWrite(in4, forward ? LOW : HIGH);
}
