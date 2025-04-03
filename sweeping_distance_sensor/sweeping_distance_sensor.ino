#include <Servo.h>

const int SERVO = 9; // Servo on Pin 9
const int IR = A0; // IR Distance Sensor on Analog Pin A0
const int LED1 = 3; // LED Output 1
const int LED2 = 5; // LED Output 2
const int LED3 = 6; // LED Output 3
const int LED4 = 11; // LED Output 4

Servo myServo; // Servo Object
int dist1 = 0; // Quadrant 1 Distance
int dist2 = 0; // Quadrant 2 Distance
int dist3 = 0; // Quadrant 3 Distance
int dist4 = 0; // Quadrant 4 Distance

void setup() {
  Serial.begin(9600); // Initialize Serial communication
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  myServo.attach(SERVO);
}

void loop() {
  dist1 = readDistance(15);
  analogWrite(LED1, dist1);
  Serial.print("Distance 1: ");
  Serial.println(dist1);
  delay(500);

  dist2 = readDistance(65);
  analogWrite(LED2, dist2);
  Serial.print("Distance 2: ");
  Serial.println(dist2);
  delay(500);

  dist3 = readDistance(115);
  analogWrite(LED3, dist3);
  Serial.print("Distance 3: ");
  Serial.println(dist3);
  delay(500);
  
  dist4 = readDistance(165);
  analogWrite(LED4, dist4);
  Serial.print("Distance 4: ");
  Serial.println(dist4);
  delay(500);
}

int readDistance(int pos) {
  myServo.write(pos);
  delay(750); // Slightly increased delay for servo stabilization
  
  int rawValue = analogRead(IR);
  float voltage = rawValue * (5.0 / 1023.0); // Convert ADC value to voltage

  Serial.print("Raw Value: ");
  Serial.print(rawValue);
  Serial.print(" | Voltage: ");
  Serial.print(voltage);
  Serial.println("V");

  // Convert raw sensor value to LED brightness
  int distance = map(rawValue, 0, 1023, 0, 255);
  distance = constrain(distance, 0, 255); // Ensure valid range

  return distance;
}

