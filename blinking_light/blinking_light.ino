#include <stdint.h>

const int BLED = 9;
const int GLED = 10;
const int RLED = 11;
const int BUTTON = 3;
const int BUTTON2 = 2;

boolean lastButton = LOW;
boolean currentButton = LOW;
boolean lastButton2 = LOW;
boolean currentButton2 = LOW;

struct Color {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

Color PURPLE = {127, 0, 127};
Color TEAL = {0, 188, 188};
Color ORANGE = {255, 127, 80};
Color WHITE = {255, 255, 255};

void setup() {
  pinMode(BLED, OUTPUT);
  pinMode(GLED, OUTPUT);
  pinMode(RLED, OUTPUT);
  pinMode(BUTTON, INPUT);
  pinMode(BUTTON2, INPUT);
  Serial.begin(9600);  // Enable serial debugging
}

void setMode(int mode) {
  switch (mode) {
    case 1:
      digitalWrite(RLED, HIGH);
      digitalWrite(GLED, LOW);
      digitalWrite(BLED, LOW);
      break;
    case 2:
      digitalWrite(RLED, LOW);
      digitalWrite(GLED, HIGH);
      digitalWrite(BLED, LOW);
      break;
    case 3:
      digitalWrite(RLED, LOW);
      digitalWrite(GLED, LOW);
      digitalWrite(BLED, HIGH);
      break;
    case 4:
      analogWrite(RLED, PURPLE.red);
      analogWrite(GLED, PURPLE.green);
      analogWrite(BLED, PURPLE.blue);
      break;
    case 5:
      analogWrite(RLED, TEAL.red);
      analogWrite(GLED, TEAL.green);
      analogWrite(BLED, TEAL.blue);
      break;
    case 6:
      analogWrite(RLED, ORANGE.red);
      analogWrite(GLED, ORANGE.green);
      analogWrite(BLED, ORANGE.blue);
      break;
    case 7:
      analogWrite(RLED, WHITE.red);
      analogWrite(GLED, WHITE.green);
      analogWrite(BLED, WHITE.blue);
      break;
    default:
      digitalWrite(RLED, LOW);
      digitalWrite(GLED, LOW);
      digitalWrite(BLED, LOW);
      break;
  }
}

int delayTimes[6] = {0, 500, 1000, 1500, 2000, 2500};

int LED_mode = 0;
int blinkDelayIndex = 0;
long previous_time = 0;
boolean ledOn = true;

void loop() {
  // Handle first button (LED mode change)
  currentButton = digitalRead(BUTTON);
  if (lastButton == LOW && currentButton == HIGH) {
    delay(50);  // Debounce
    ++LED_mode;
    if (LED_mode > 7) LED_mode = 0;
  }
  lastButton = currentButton;

  // Handle second button (Blink delay change)
  currentButton2 = digitalRead(BUTTON2);
  if (lastButton2 == LOW && currentButton2 == HIGH) {
    delay(50);  // Debounce
    ++blinkDelayIndex;
    if (blinkDelayIndex > 5) blinkDelayIndex = 0;
  }
  lastButton2 = currentButton2;

  // Blink logic
  long time_elapsed = millis();
  int blink_delay = delayTimes[blinkDelayIndex];

  long deltaTime = time_elapsed - previous_time;
  if (blink_delay == 0) {
    ledOn = true;  // No blinking, LED stays ON
  } else if (deltaTime >= static_cast<long>(blink_delay)) {
    previous_time = time_elapsed;
    ledOn = !ledOn;  // Toggle LED state
  }

  // Update LEDs
  if (ledOn) {
    setMode(LED_mode);
  } else {
    setMode(0);  // Turn off LEDs if blinking
  }
}
