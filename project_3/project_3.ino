#include "MicroBlue.h"
#include "SoftwareSerial.h"
#include "Arduino.h"

// MOTOR LEFT PINS
#define ENA 10
#define IN1 8
#define IN2 9

// MOTOR RIGHT PINS
#define ENB 3
#define IN3 7
#define IN4 4

#define MINIMUM_MOTOR_SPEED 40

#define rXPin 12
#define tXPin 13
SoftwareSerial SSerial(rXPin, tXPin);
MicroBlueManager manager(SSerial);

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
  Serial.println("Hello");
  
  // Print message details if both ID and Value are valid
  if (msg.hasId() && msg.hasValue()) {
    Serial.println(msg.toString());
  }

  // Check for a specific message ID to control drive system
  if (msg.id == "d1"){
    int throttle, steering;
    sscanf(msg.value.c_str(), "%d,%d", &steering, &throttle);

    // Adjust values to center at 0 (assuming incoming range of 0-1023)
    throttle -= 512;
    steering -= 512;
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

// Converts throttle and steering inputs into PWM signals for motors
void drive(int throttle, int steering)
{
  // Print Debug Info
  Serial.print("throttle: ");
  Serial.print(throttle);
  Serial.print("\tsteering: ");
  Serial.print(steering);

  // Brake if throttle is zero
  if (throttle == 0)
  {
    motorBrake();
    return;
  }

  // Set motor direction based on throttle value
  if (throttle > 0)
    motorSetForward();
  else
    motorSetBackward();

  // Map throttle to PWM range.
  int mappedSpeed = map(abs(throttle), 0, 512, MINIMUM_MOTOR_SPEED, 255);

  // Map steering to PWM range.
  int reducedSpeed = map(abs(steering), 0, 512, mappedSpeed, MINIMUM_MOTOR_SPEED);

  int leftMotorSpeed, rightMotorSpeed;
  if (steering > 0)
  {
    Serial.println("Turning right");
    // Turn Right: reduce right motor speed
    leftMotorSpeed = mappedSpeed;
    rightMotorSpeed = reducedSpeed;
  }
  else
  {
    // Turn Left: reduce left motor speed
     Serial.println("Turning left");
    leftMotorSpeed = reducedSpeed;
    rightMotorSpeed = mappedSpeed;
  }

  // Set motor speeds
  analogWrite(ENA, leftMotorSpeed);
  analogWrite(ENB, rightMotorSpeed);

  // Print Debug Info
  Serial.print("\tmappedSpeed: ");
  Serial.print(mappedSpeed);
  Serial.print("\treducedSpeed: ");
  Serial.print(reducedSpeed);
  Serial.print("\tleftMotorSpeed: ");
  Serial.print(leftMotorSpeed);
  Serial.print("\trightMotorSpeed: ");
  Serial.println(rightMotorSpeed);
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

// move forward
void motorSetForward()
{
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// move backward
void motorSetBackward()
{
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
