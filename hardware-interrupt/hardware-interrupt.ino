const int BUTTON_INT = 0; // Interrupt 0 (pin 2 on the Uno)
const int RED = 11;       // Red LED on pin 11
const int GREEN = 10;     // Green LED on pin 10
const int BLUE = 9;       // Blue LED on pin 9

// Volatile variables can change inside interrupts
volatile int selectedLED = RED;
void setup()
{
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  // The pin is inverted, so we want to look at the rising edge
  attachInterrupt(BUTTON_INT, swap, RISING);
}

void swap()
{
  analogWrite(selectedLED, 0);
  switch (selectedLED) {
    case RED:
      selectedLED = BLUE;
      break;
    case BLUE:
      selectedLED = GREEN;
      break;
    case GREEN:
      selectedLED = RED;
      break;
  }
}

void loop()
{
  for (int i = 0; i < 256; i++)
  {
    analogWrite(selectedLED, i);
    delay(10);
  }
  for (int i = 255; i >= 0; i--)
  {
    analogWrite(selectedLED, i);
    delay(10);
  }
}