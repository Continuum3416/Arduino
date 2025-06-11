#include "MicroBlue.h"
#include "SoftwareSerial.h"
#include "Arduino.h"

// MOTOR LEFT PINS
#define ENA 10
#define IN1 9
#define IN2 8

// MOTOR RIGHT PINS
#define IN3 7
#define IN4 6
#define ENB 5

#define MINIMUM_MOTOR_SPEED 10
#define MAXIMUM_MOTOR_SPEED 140

#define rXPin 4 // connect rx to tx
#define tXPin 3 // connect tx to rx
SoftwareSerial SSerial(rXPin, tXPin);
MicroBlueManager manager(SSerial);


// Potentiometer
#define DEADZONE 50 // Steering deadzone, adjust as needed
#define POT_PIN A4

// Predeclare functions
void setMotorPins();
void drive(int throttle, int steering);
void motorBrake();
void motorSetForward();
void motorSetBackward();


void setup()
{
  Serial.begin(9600);  // Initialize USB serial communication
  SSerial.begin(9600); // Initialize software serial for BLE communication
  setMotorPins();      // Configure motor pins for output
}

void loop() {
  // Read a message from BLE
  MicroBlueMessage msg = manager.read();
  // Check for a specific message ID to control drive system
  if (msg.id == "d1"){
    int throttle, steering;
    sscanf(msg.value.c_str(), "%d,%d", &steering, &throttle);

    // Adjust values to center at 0 (assuming incoming range of 0-1023)
    throttle -= 512;
    steering -= 512;

    // Print the raw and centered values
    // Serial.print("X (Steering): ");
    // Serial.print(steering + 512);
    // Serial.print("\tY (Throttle): ");
    // Serial.println(throttle + 512);
    drive(throttle, steering);
  }
}


// Initialize motor pins as outputs
void setMotorPins()
{
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void drive(int throttle, int steering)
{
  // Read the potentiometer value (0 to 1023)
  int potValue = analogRead(POT_PIN);

  // Map the potentiometer value to a range between 100 and 255 for max speed
  int maxSpeed = map(potValue, 0, 1023, 30, 255);
  Serial.println(maxSpeed);

  // Brake if throttle is zero
  if (throttle == 0)
  {
    motorBrake();
    return;
  }

  // Apply the deadzone to steering
  if (abs(steering) < DEADZONE) {
    steering = 0;  // Ignore small steering movements
  }

  // Set motor direction based on throttle value
  if (throttle > 0) {
    motorSetForward();  // Both motors move forward if throttle is positive
  } else {
    motorSetBackward();  // Both motors move backward if throttle is negative
  }

  // Map throttle to PWM range based on maxSpeed.
  int mappedSpeed = map(abs(throttle), 0, 512, MINIMUM_MOTOR_SPEED, maxSpeed);

  // Map steering to PWM range.
  int reducedSpeed = map(abs(steering), 0, 512, mappedSpeed, MINIMUM_MOTOR_SPEED);

  int leftMotorSpeed = mappedSpeed;
  int rightMotorSpeed = mappedSpeed;

  if (steering > 0) {
    // Turning right: reduce right motor speed
    rightMotorSpeed = reducedSpeed;
    leftMotorSpeed = mappedSpeed;
  }
  else if (steering < 0) {
    // Turning left: reduce left motor speed
    leftMotorSpeed = reducedSpeed;
    rightMotorSpeed = mappedSpeed;
  }

  // Ensure the speeds are within the PWM range
  leftMotorSpeed = constrain(leftMotorSpeed, MINIMUM_MOTOR_SPEED, maxSpeed);
  rightMotorSpeed = constrain(rightMotorSpeed, MINIMUM_MOTOR_SPEED, maxSpeed);

  // Set motor speeds
  analogWrite(ENA, leftMotorSpeed);
  analogWrite(ENB, rightMotorSpeed);
}


// stop/brake
void motorBrake()
{
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// move backward
void motorSetBackward()
{
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// move forward
void motorSetForward()
{
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
