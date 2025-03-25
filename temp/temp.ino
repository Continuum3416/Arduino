// Potentiometer-Based Temperature Reading Program with RGB LED Control
const int POT = 0;   // Potentiometer (or temperature sensor) on analog pin 0

// RGB LED Pins
const int BLUE = 9;
const int GREEN = 10;
const int RED = 11;

int val = 0;         // Variable to hold the analog reading
float voltage_mV = 0.f;
float temp_C = 0.f;

void setup() {
  Serial.begin(9600);

  // Set RGB LED pins as output
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
}

// Custom print function
void printInfo(String label, float value, String unit) {
  Serial.print(label);
  Serial.print(": ");
  Serial.print(value);
  Serial.print(" ");
  Serial.println(unit);
}

// Function to control RGB LED
void setLED(int red, int green, int blue) {
  analogWrite(RED, red);
  analogWrite(GREEN, green);
  analogWrite(BLUE, blue);
}

void loop() {
  // Read analog value and convert to temperature
  val = analogRead(POT);
  voltage_mV = (val * 5000.0f) / 1023.0f;
  temp_C = (voltage_mV - 500.f) / 10.f;  // Convert mV to °C (assuming LM35 sensor)

  // Print temperature info
  printInfo("Voltage", voltage_mV, "mV");
  printInfo("Temperature", temp_C, "C");

  // LED control logic
  if (temp_C < 20) {
    setLED(0, 0, 255);  // Cold: Blue ON
    Serial.println("Status: Cold - Blue LED ON");
  } 
  else if (temp_C >= 20 && temp_C <= 28) {
    setLED(0, 255, 0);  // Moderate: Green ON
    Serial.println("Status: Moderate - Green LED ON");
  } 
  else {
    Serial.println("Status: Hot - Red LED Blinking");
    for (int i = 0; i < 3; i++) { // Blink Red 3 times
      setLED(255, 0, 0);
      delay(250);
      setLED(0, 0, 0);
      delay(250);
    }
  }

  Serial.print('\n');
  delay(500);
}