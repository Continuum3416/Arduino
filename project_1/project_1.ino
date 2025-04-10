// Don, Vivian, Felix
// 04/08/2025
//
// Red Light, Green Light Game Simulation (Inspired by Squid Game)
//
// Overview:
// This program simulates the popular "Red Light, Green Light" game,
// as seen in Squid Game. The player must advance towards a target
// without being caught moving during a "Red Light" signal.
//
// Game Rules:
// - The player starts at a distance from the target (e.g., a button).
// - A signal light switches between RED and GREEN every 3 seconds.
// - When the signal is GREEN, the player is allowed to move forward.
// - When the signal turns RED, the player must freeze immediately.
//   Moving during RED results in elimination (being "shot").
// - The goal is to reach the target and press the button without
//   being eliminated.
//
// Victory Condition:
// - The player wins by successfully reaching the target and
//   pressing the button during a GREEN light phase.


// Pin definitions
const int RED_LEDS[]   = {13, 12, 11};
const int BLUE_LEDS[]  = {10, 9, 8};
const int SPEAKER_PIN  = 7;
const int SPEAKER2_PIN = 4;
const int BUTTON_PIN   = 2;
const int PIR_PIN      = 3;
const int RED_RGB_PIN  = 6;  // RGB LED red pin
const int GREEN_RGB_PIN = 5; // RGB LED green pin

const int NUM_LEDS     = 3;
const int SONG_LENGTH  = 10;
const unsigned long DEBOUNCE_DELAY = 50;

// Squid game theme
int song[SONG_LENGTH] = { 415, 554, 554, 554, 494, 554, 554, 415, 415, 494 };
int noteDurations[SONG_LENGTH];

// Variables used in loops to detect if the arduino is running
bool isRunning = false;
bool motionDetected = false;

// LED animation logic
unsigned long previousLedTime = 0;
int ledStep = 0;

// Variables for button state handling
bool lastButtonState = HIGH;
bool currentButtonState = HIGH;
unsigned long lastDebounceTime = 0;

// Song timing
unsigned long previousNoteTime = 0;
int currentNote = 0;

// Timer control variables
bool isGreenPhase = true;  // Green phase (go) or red phase (stop)
unsigned long lastPhaseTime = 0;
const unsigned long PHASE_DURATION = 5000; // 5 seconds for each phase

// Author: Don
// Function to fill an array with a value
// Input: pointer to array, array size, and value
// Used to fill noteDurations with one specific value
void initialize_array(int *arr, const int size, const int value) {
  for (int i = 0; i < size; i++) {
    arr[i] = value;
  }
}

// Author: Vivian and Don
// Setup components
void setup() {
  Serial.begin(9600);

  // Set note duration to 300ms
  initialize_array(noteDurations, SONG_LENGTH, 300);

  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(RED_LEDS[i], OUTPUT);
    pinMode(BLUE_LEDS[i], OUTPUT);
  }

  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(SPEAKER2_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(PIR_PIN, INPUT);
  
  // Set up RGB LED pins
  pinMode(RED_RGB_PIN, OUTPUT);
  pinMode(GREEN_RGB_PIN, OUTPUT);
  digitalWrite(RED_RGB_PIN, LOW);
  digitalWrite(GREEN_RGB_PIN, LOW);
}


// Author: Don
void loop() {
  handleButton();

  // If button was toggled on
  if (isRunning) {
    // Update timer for green/red phase switching
    unsigned long currentTime = millis();
    if (currentTime - lastPhaseTime >= PHASE_DURATION) {
      lastPhaseTime = currentTime;
      isGreenPhase = !isGreenPhase;  // Switch phase
    }

    // Handle movement (PIR detection)
    if (isGreenPhase) {
      // Green light - allow movement
      digitalWrite(GREEN_RGB_PIN, HIGH);
      digitalWrite(RED_RGB_PIN, LOW);
      motionDetected = false;  // Disable motion detection in "Go" phase
      Serial.println("Green light: You can move.");
    } else {
      // Red light - stop movement, enable PIR
      digitalWrite(GREEN_RGB_PIN, LOW);
      digitalWrite(RED_RGB_PIN, HIGH);
      motionDetected = digitalRead(PIR_PIN) == HIGH;

      if (motionDetected) {
        stopLeds();
        stopSong();
        playGunSound();
        Serial.println("Red light: Motion detected! Game over!");
      } else {
        updateSong();
        updateLeds();
        Serial.println("Red light: No motion detected. Continue...");
      }
    }
  } else {
    stopAll();
  }
}

// Author: Don
// Button toggle function with debounce
// When pressed, everything on the circuit gets turned on/off
// Controls isRunning
void handleButton() {
  // read button state
  int reading = digitalRead(BUTTON_PIN);

  // Reset debounce timer if state changed
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  // If stable for longer than debounce delay
  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != currentButtonState) {
      currentButtonState = reading;
      if (currentButtonState == LOW) {
        isRunning = !isRunning;
        currentNote = 0;
        ledStep = 0;
        stopAll();
      }
    }
  }

  lastButtonState = reading;
}

// Author: Vivian
// Music player
void updateSong() {
  unsigned long currentTime = millis();

  if (currentNote < SONG_LENGTH) {
    int duration = noteDurations[currentNote];

    // Only play next note if enough time has passed
    if (currentTime - previousNoteTime >= duration * 1.3) {
      if (song[currentNote] != 0) {
        tone(SPEAKER_PIN, song[currentNote], duration);
      } else {
        noTone(SPEAKER_PIN);
      }

      previousNoteTime = currentTime;
      currentNote++;
    }
  } else {
    currentNote = 0;  // Loop song
  }
}

void stopSong() {
  noTone(SPEAKER_PIN);
}

// Author: Vivian
// LED animation
void updateLeds() {
  unsigned long currentTime = millis();

  if (currentTime - previousLedTime >= 100) {
    previousLedTime = currentTime;

    // Turn on/off LEDs based on animation step
    switch (ledStep) {
      case 0:
        digitalWrite(RED_LEDS[0], HIGH);
        digitalWrite(RED_LEDS[1], HIGH);
        digitalWrite(BLUE_LEDS[0], HIGH);
        digitalWrite(BLUE_LEDS[1], HIGH);
        break;
      case 1:
        digitalWrite(RED_LEDS[0], LOW);
        digitalWrite(RED_LEDS[1], LOW);
        digitalWrite(BLUE_LEDS[0], LOW);
        digitalWrite(BLUE_LEDS[1], LOW);
        break;
      case 2:
        digitalWrite(RED_LEDS[2], HIGH);
        digitalWrite(RED_LEDS[1], HIGH);
        digitalWrite(BLUE_LEDS[2], HIGH);
        digitalWrite(BLUE_LEDS[1], HIGH);
        break;
      case 3:
        digitalWrite(RED_LEDS[2], LOW);
        digitalWrite(RED_LEDS[1], LOW);
        digitalWrite(BLUE_LEDS[2], LOW);
        digitalWrite(BLUE_LEDS[1], LOW);
        break;
    }
	
    // to alternate pattern
    ledStep = (ledStep + 1) % 4; // Loop animation steps
  }
}

// Author: Don
// Stop LED animation
// Set all LEDs to LOW
void stopLeds() {
  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(RED_LEDS[i], LOW);
    digitalWrite(BLUE_LEDS[i], LOW);
  }
}

// Author: Don
// Gun sound effect
void playGunSound() {
  tone(SPEAKER2_PIN, 200, 60);
  delay(60);
  tone(SPEAKER2_PIN, 100, 40);
  delay(40);
  noTone(SPEAKER2_PIN);
}

// Author: Don
// Turn off everything
void stopAll() {
  stopLeds();
  stopSong();
  noTone(SPEAKER2_PIN);
  digitalWrite(RED_RGB_PIN, LOW);
  digitalWrite(GREEN_RGB_PIN, LOW);
}
