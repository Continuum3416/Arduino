/*
  ===============================================
  Author      : Don D. Le
  Date        : June 12, 2025
  Description : Arduino Rover with Bluetooth Joystick Control,
                Line Tracking, Collision Avoidance, and 
                Acceleration Calculation and Logging via 
                Ultrasonic Sensor.

  How it works:
    - MANUAL Mode: Controlled using Bluetooth joystick (MicroBlue).
    - AUTONOMOUS Mode: Uses line sensors to follow a path.
    - Ultrasonic sensor detects obstacles and prevents collisions.
    - Distance readings are used to compute velocity and acceleration,
      which are logged to an SD card as CSV.
  ===============================================
*/

#include "MicroBlue.h"
#include "SoftwareSerial.h"
#include "Arduino.h"
#include <SD.h>

// ========== Motor Pins ==========
#define ENA 10     // Left motor enable
#define IN1 9      // Left motor input 1
#define IN2 8      // Left motor input 2
#define IN3 7      // Right motor input 1
#define IN4 6      // Right motor input 2
#define ENB 5      // Right motor enable

#define MINIMUM_MOTOR_SPEED 20  // Prevents motors from stalling

// ========== Bluetooth ==========
#define rXPin A4   // Bluetooth RX pin
#define tXPin A3   // Bluetooth TX pin
SoftwareSerial SSerial(rXPin, tXPin);
MicroBlueManager manager(SSerial);

// ========== Ultrasonic Sensor ==========
#define TRIG_PIN 3
#define ECHO_PIN 2

// ========== Line Sensors ==========
#define left_sensor A0
#define right_sensor A1
#define LINE_THRESHOLD 500  // Analog value below this is considered "on line"

// ========== SD Card ==========
#define CS_PIN 4

// ========== Collision Avoidance Settings ==========
#define COLLISION_DISTANCE 20.0   // cm, emergency stop distance
#define WARNING_DISTANCE 25.0     // cm, reduce speed threshold

// ========== Control Deadzone ==========
#define DEADZONE 50  // Ignore small steering input

// ========== Global State ==========
bool drive_mode = true;    // True = manual, False = autonomous (line tracking)
int potValue = 60;         // Default potentiometer value for speed scaling

float prevDistance = 0;       // Previous distance for velocity calc
float prevVelocity = 0;       // Previous velocity for acceleration calc
unsigned long prevPhysicsTime = 0;  // Timestamp of last physics update

// ========== Function Prototypes ==========
void setMotorPins();
void drive(int throttle, int steering, int potValue);
void motorBrake();
void motorSetForward();
void motorSetBackward();
float readDistance();
void logAcceleration();
bool handleCollisionAvoidance(int throttle, float distance);
void lineTracking(int potValue);
void printCoords(int throttle, int steering);

void setup() {
  Serial.begin(9600);
  SSerial.begin(9600);
  setMotorPins();  // Set motor pins as outputs

  // Set sensor pin modes
  pinMode(left_sensor, INPUT);
  pinMode(right_sensor, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Initialize SD card
  pinMode(CS_PIN, OUTPUT);
  Serial.println("Initializing SD Card...");
  if (!SD.begin(CS_PIN)) {
    Serial.println("Card Failure");
    return;
  }
  Serial.println("Card Ready");

  // Create or open the log file and write header if it's new
  File logFile = SD.open("LOG.csv", FILE_WRITE);
  if (logFile && logFile.size() == 0) {
    logFile.println("Time(ms)\tVelocity(m/s)\tAcceleration(m/s^2)\tMaxAccelerationSoFar(m/s^2)");
    logFile.close();
  }
}

void loop() {
  MicroBlueMessage msg = manager.read();  // Read BLE input

  // Debounce mode toggle between manual and autonomous driving
  static unsigned long lastToggleMs = 0;
  const unsigned long debounceMs = 200;
  if (msg.id == "b0" && millis() - lastToggleMs > debounceMs) {
    drive_mode = !drive_mode;
    Serial.println(drive_mode ? "MANUAL" : "AUTONOMOUS");
    lastToggleMs = millis();
  }

  // Update potValue (speed) from slider
  if (msg.id == "sl0") {
    potValue = msg.value.toInt();
    Serial.println(potValue);
  }

  // MANUAL driving using joystick
  if (drive_mode && msg.id == "d1") {
    int throttle, steering;
    sscanf(msg.value.c_str(), "%d,%d", &steering, &throttle);
    throttle -= 512;  // Center to 0
    steering -= 512;
    drive(throttle, steering, potValue);
  } 
  // AUTONOMOUS mode using line sensors
  else if (!drive_mode) {
    lineTracking(potValue);
  }
}

// Debug: print joystick coords
void printCoords(int throttle, int steering) {
  Serial.print("X (Steering): ");
  Serial.print(steering + 512);
  Serial.print("\tY (Throttle): ");
  Serial.println(throttle + 512);
}

// Set all motor control pins as outputs
void setMotorPins() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

// Ultrasonic sensor distance in cm
float readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  float duration = pulseIn(ECHO_PIN, HIGH, 30000);  // Timeout after 30ms
  if (duration == 0) return 999.0;

  float distance = (duration * 0.0343) / 2;  // Convert to cm
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  return distance;
}

// If too close, brake and stop driving
bool handleCollisionAvoidance(int throttle, float distance) {
  if (throttle > 0 && distance < COLLISION_DISTANCE) {
    motorBrake();
    Serial.println("COLLISION AVOIDANCE - STOPPING!");
    return true;
  }
  if (throttle > 0 && distance < WARNING_DISTANCE) {
    Serial.println("WARNING - Obstacle detected, reducing speed");
  }
  return false;
}

// Log velocity and acceleration to SD card
void logAcceleration() {
  static float maxAcceleration = 0;
  unsigned long currentTime = millis();
  float currentDistance = readDistance() / 100.0;  // meters
  float deltaTime = (currentTime - prevPhysicsTime) / 1000.0;  // seconds

  if (deltaTime > 0.1) {  // Log every 100ms
    float velocity = (currentDistance - prevDistance) / deltaTime;
    float acceleration = (velocity - prevVelocity) / deltaTime;

    if (abs(acceleration) > abs(maxAcceleration))
      maxAcceleration = acceleration;

    // Print to serial
    Serial.print("Velocity: ");
    Serial.print(velocity, 4);
    Serial.print(" m/s\tAcceleration: ");
    Serial.print(acceleration, 4);
    Serial.print(" m/s^2\tMax Accel: ");
    Serial.print(maxAcceleration, 4);
    Serial.println(" m/s^2");

    // Save to SD card
    File logFile = SD.open("LOG.csv", FILE_WRITE);
    if (logFile) {
      logFile.print(currentTime);
      logFile.print("\t");
      logFile.print(velocity, 4);
      logFile.print("\t");
      logFile.print(acceleration, 4);
      logFile.print("\t");
      logFile.println(maxAcceleration, 4);
      logFile.close();
    }

    // Update for next loop
    prevDistance = currentDistance;
    prevVelocity = velocity;
    prevPhysicsTime = currentTime;
  }
}

// Main drive function with speed/steering logic
void drive(int throttle, int steering, int potValue) {
  const int THROTTLE_DEADZONE = 20;

  float distance = readDistance();
  if (handleCollisionAvoidance(throttle, distance)) return;

  // Stop if throttle is low
  if (abs(throttle) < THROTTLE_DEADZONE) {
    motorBrake();
    return;
  }

  if (abs(steering) < DEADZONE) steering = 0;

  // Set motor direction
  throttle > 0 ? motorSetForward() : motorSetBackward();

  // Determine max speed based on slider
  int maxSpeed = map(potValue, 0, 100, 30, 150);
  if (throttle > 0 && distance < WARNING_DISTANCE)
    maxSpeed *= 0.5; // reduced max speed by half to prepare for collision

  // normal speed
  int baseSpeed = map(abs(throttle), 0, 512, MINIMUM_MOTOR_SPEED, maxSpeed);

  // turn speed is slower
  int turnSpeed = map(abs(steering), 0, 512, baseSpeed, MINIMUM_MOTOR_SPEED);

  // asign both motors speed to base speed
  int leftMotorSpeed = baseSpeed;
  int rightMotorSpeed = baseSpeed;

  // Apply turn correction
  if (steering > 0) rightMotorSpeed = turnSpeed;     // Right turn
  else if (steering < 0) leftMotorSpeed = turnSpeed; // Left turn

  // Clamp to valid PWM range
  leftMotorSpeed = constrain(leftMotorSpeed, MINIMUM_MOTOR_SPEED, 255);
  rightMotorSpeed = constrain(rightMotorSpeed, MINIMUM_MOTOR_SPEED, 255);

  analogWrite(ENA, leftMotorSpeed);
  analogWrite(ENB, rightMotorSpeed);

  Serial.print("Left PWM: ");
  Serial.print(leftMotorSpeed);
  Serial.print(" | Right PWM: ");
  Serial.println(rightMotorSpeed);

  logAcceleration();  // Log motion physics
}

// Stop all motors
void motorBrake() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// Set motors to move backward
void motorSetBackward() {
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Set motors to move forward
void motorSetForward() {
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Line following behavior
void lineTracking(int potValue) {
  int leftVal = analogRead(left_sensor);
  int rightVal = analogRead(right_sensor);

  bool leftOnLine = leftVal < LINE_THRESHOLD;
  bool rightOnLine = rightVal < LINE_THRESHOLD;

  if (leftOnLine && rightOnLine) {
    drive(512, 0, potValue);  // Go straight
    Serial.println("Straight (both sensors on line)");
  } else if (leftOnLine) {
    drive(512, 400, potValue);  // Veer left
    Serial.println("Left Turn (left sensor on line)");
  } else if (rightOnLine) {
    drive(512, -400, potValue);  // Veer right
    Serial.println("Right Turn (right sensor on line)");
  } else {
    motorBrake();  // Stop if line is lost
    Serial.println("Line lost — braking");
  }
}
