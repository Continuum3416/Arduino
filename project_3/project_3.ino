#include "MicroBlue.h"
#include "SoftwareSerial.h"
#include "Arduino.h"
#include <SD.h>

// MOTOR LEFT PINS
#define ENA 10
#define IN1 9
#define IN2 8

// MOTOR RIGHT PINS
#define IN3 7
#define IN4 6
#define ENB 5

#define MINIMUM_MOTOR_SPEED 10

#define rXPin A4  // connect rx to tx
#define tXPin A3  // connect tx to rx
SoftwareSerial SSerial(rXPin, tXPin);
MicroBlueManager manager(SSerial);

// ultrasonic
#define ECHO_PIN 2
#define TRIG_PIN 3

// Potentiometer
#define DEADZONE 50  // Steering deadzone, adjust as needed
#define POT_PIN 2

// Line sensors
const int left_sensor = A0;
const int right_sensor = A1;

// SD CARD
#define CS_PIN 4

// COLLISION AVOIDANCE SETTINGS
#define COLLISION_DISTANCE 20.0  // Stop if obstacle is closer than 15cm
#define WARNING_DISTANCE 25.0    // Slow down if obstacle is closer than 25cm

// Global variables
bool drive_mode = true;
int leftLSValue, rightLSValue;
int potValue = 60;

// Forward declare functions
void setMotorPins();
void drive(int, int, int);
void motorBrake();
void motorSetForward();
void motorSetBackward();
void printCoords(int, int);
float readDistance();
void lineTracking(int);

void setup() {
  Serial.begin(9600);   // Initialize USB serial communication
  SSerial.begin(9600);  // Initialize software serial for BLE communication
  setMotorPins();       // Configure motor pins for output

  // Configure line sensor pins as inputs
  pinMode(left_sensor, INPUT);
  pinMode(right_sensor, INPUT);

  // Configure ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Set up card
  Serial.println("Initializing Card");
  //CS pin is an output
  pinMode(CS_PIN, OUTPUT);
  if (!SD.begin(CS_PIN)) {
    Serial.println("Card Failure");
    return;
  }
  Serial.println("Card Ready");
}

void loop() {
  // Read a message from BLE
  MicroBlueMessage msg = manager.read();

  static unsigned long lastToggleMs = 0;
  const unsigned long debounceMs = 200;

  // Debounce button
  if (msg.id == "b0") {
    if (millis() - lastToggleMs > debounceMs) {
      drive_mode = !drive_mode;
      Serial.println(drive_mode ? "MANUAL" : "AUTONOMOUS");
      lastToggleMs = millis();
    }
  }

  // Read the potentiometer value (0 to 1023)
  if (msg.id == "sl0") {
    potValue = msg.value.toInt();
    Serial.println(potValue);
  }

  if (drive_mode) {
    // Bluetooth Joystick
    if (msg.id == "d1") {
      int throttle, steering;
      sscanf(msg.value.c_str(), "%d,%d", &steering, &throttle);

      // Adjust values to center at 0 (assuming incoming range of 0-1023)
      throttle -= 512;
      steering -= 512;

      // printCoords(throttle, steering);
      drive(throttle, steering, potValue);
    }
  } else {
    lineTracking(potValue);
  }
}

// Debug function
void printCoords(int throttle, int steering) {
  Serial.print("X (Steering): ");
  Serial.print(steering + 512);
  Serial.print("\tY (Throttle): ");
  Serial.println(throttle + 512);
}

// Initialize motor pins as outputs
void setMotorPins() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

// Improved ultrasonic sensor function
float readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);  // Increased for better accuracy
  digitalWrite(TRIG_PIN, LOW);

  float duration = pulseIn(ECHO_PIN, HIGH, 30000);  // 30ms timeout

  if (duration == 0) {
    return 999.0;  // Return large distance if no echo received
  }

  float distance = (duration * 0.0343) / 2;  // More accurate speed of sound

  // Serial.print("Distance: ");
  // Serial.print(distance);
  // Serial.println(" cm");

  return distance;
}


// Modified drive function with collision avoidance
bool handleCollisionAvoidance(int throttle, float distance) {
  Serial.println(distance);

  if (throttle > 0 && distance < COLLISION_DISTANCE) {
    motorBrake();
    Serial.println("COLLISION AVOIDANCE - STOPPING!");
    return true;  // Stop driving
  }

  if (throttle > 0 && distance < WARNING_DISTANCE) {
    Serial.println("WARNING - Obstacle detected, reducing speed");
  }

  return false;  // No need to stop
}

float prevDistance = 0;
float prevVelocity = 0;
unsigned long prevPhysicsTime = 0;

void logAccelerationRealUnits() {
  unsigned long currentTime = millis();
  float currentDistance = readDistance() / 100.0;  // Convert cm to meters
  float deltaTime = (currentTime - prevPhysicsTime) / 1000.0;  // Convert ms to seconds

  if (deltaTime > 0.1) { // Only update every 100ms
    float velocity = (currentDistance - prevDistance) / deltaTime;
    float acceleration = (velocity - prevVelocity) / deltaTime;

    // Log to Serial
    Serial.print("Velocity: ");
    Serial.print(velocity, 4);
    Serial.print(" m/s\tAcceleration: ");
    Serial.print(acceleration, 4);
    Serial.println(" m/s^2");

    // Log to SD card
    File logFile = SD.open("LOG.csv", FILE_WRITE);
    if (logFile) {
      logFile.print(currentTime);
      logFile.print("\t");
      logFile.print(velocity, 4);
      logFile.print("\t");
      logFile.print(acceleration, 4);
      logFile.println();
      logFile.close();
    }

    // Update previous values
    prevDistance = currentDistance;
    prevVelocity = velocity;
    prevPhysicsTime = currentTime;
  }
}


void drive(int throttle, int steering, int potValue) {
  // Read distance for collision avoidance
  float distance = readDistance();

  // Handle collision
  if (handleCollisionAvoidance(throttle, distance)) {
    return;
  }

  // Map the potentiometer value to a range between 100 and 255 for max speed
  int maxSpeed = map(potValue, 0, 100, 20, 255);

  // Reduce speed if approaching obstacle while moving forward
  if (throttle > 0 && distance < WARNING_DISTANCE) {
    maxSpeed = maxSpeed * 0.5;
  }

  // Brake if throttle is zero
  if (throttle == 0) {
    motorBrake();
    return;
  }

  // Apply the deadzone to steering
  if (abs(steering) < DEADZONE) {
    steering = 0;  // Ignore small steering movements
  }

  // Set motor direction based on throttle value
  if (throttle > 0) {
    motorSetForward();
  } else {
    motorSetBackward();
  }

  // Map throttle to PWM range based on maxSpeed
  int mappedSpeed = map(abs(throttle), 0, 512, MINIMUM_MOTOR_SPEED, maxSpeed);

  // Map steering to PWM range
  int reducedSpeed = map(abs(steering), 0, 512, mappedSpeed, MINIMUM_MOTOR_SPEED);

  // Right motor is faster, so reduce its speed
  int leftMotorSpeed = mappedSpeed;
  int rightMotorSpeed = 0.65 * mappedSpeed;

  Serial.println(mappedSpeed);
  logAcceleration(leftMotorSpeed, rightMotorSpeed);

  // Adjust motor speeds based on steering
  if (steering > 0) {
    rightMotorSpeed = reducedSpeed;
    leftMotorSpeed = mappedSpeed;
  } else if (steering < 0) {
    leftMotorSpeed = reducedSpeed;
    rightMotorSpeed = mappedSpeed;
  }

  // Constrain motor speeds
  leftMotorSpeed = constrain(leftMotorSpeed, MINIMUM_MOTOR_SPEED, maxSpeed);
  rightMotorSpeed = constrain(rightMotorSpeed, MINIMUM_MOTOR_SPEED, maxSpeed);

  // Set motor speeds
  analogWrite(ENA, leftMotorSpeed);
  analogWrite(ENB, rightMotorSpeed);
}


// stop/brake
void motorBrake() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// move backward
void motorSetBackward() {
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// move forward
void motorSetForward() {
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Line tracking function with corrected logic
void lineTracking(int potValue) {
  leftLSValue = digitalRead(left_sensor);
  rightLSValue = digitalRead(right_sensor);

  // Use a medium constant speed
  int throttle = 300;  // Forward throttle
  int steering = 0;

  // 0 means white (no line), 1 means black (line detected)
  if (leftLSValue == 0 && rightLSValue == 0) {
    // Both sensors see white - robot is on the line, go straight
    drive(throttle, 0, potValue);
    Serial.println("Going straight - on line");
  } else if (leftLSValue == 1 && rightLSValue == 0) {
    // Left sensor sees black, right sees white
    // Robot has drifted LEFT off the line, need to turn RIGHT to get back on line
    steering = 400;
    drive(throttle, steering, potValue);
    Serial.println("Turning RIGHT (drifted left off line)");
  } else if (leftLSValue == 0 && rightLSValue == 1) {
    // Left sensor sees white, right sees black
    // Robot has drifted RIGHT off the line, need to turn LEFT to get back on line
    steering = -400;
    drive(throttle, steering, potValue);
    Serial.println("Turning LEFT (drifted right off line)");
  } else if (leftLSValue == 1 && rightLSValue == 1) {
    // Both sensors see black - could be intersection, end of line, or very wide line
    drive(throttle, 0, potValue);
    Serial.println("Both sensors on black - going straight");
  }
}