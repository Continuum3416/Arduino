// HW 27 - Hardware Interrupts Version
// Author: [Your Name]
// Description: Replaces polling with hardware interrupts for LED color and delay control

const int LEDR = 11;
const int LEDG = 10;
const int LEDB = 9;

const int ColorBUTTON = 2; // Must be interrupt-capable pin
const int DelayBUTTON = 3; // Must be interrupt-capable pin

volatile int mode = 0;
volatile int delaytime = 100;
volatile bool LEDon = false;

int R = 0, G = 0, B = 0;

unsigned long startblink = 0;
unsigned long endblink = 100;

void setup() {
  Serial.begin(9600);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(ColorBUTTON, INPUT);
  pinMode(DelayBUTTON, INPUT);

  attachInterrupt(digitalPinToInterrupt(ColorBUTTON), changeColor, RISING);
  attachInterrupt(digitalPinToInterrupt(DelayBUTTON), changeDelay, RISING);

  setMode(mode);
  startblink = millis();
  endblink = startblink + delaytime;
}

void loop() {
  unsigned long now = millis();

  if (now > endblink) {
    LEDon = (delaytime == 0) ? true : !LEDon;
    startblink = now;
    endblink = startblink + delaytime;
  }

  if (LEDon) {
    analogWrite(LEDR, R);
    analogWrite(LEDG, G);
    analogWrite(LEDB, B);
  } else {
    analogWrite(LEDR, 0);
    analogWrite(LEDG, 0);
    analogWrite(LEDB, 0);
  }

  Serial.print("Delay: "); Serial.print(delaytime);
  Serial.print(" Mode: "); Serial.print(mode);
  Serial.print(" LEDon: "); Serial.println(LEDon);
}

void changeColor() {
  mode = (mode + 1) % 8;
  setMode(mode);
}

void changeDelay() {
  if (delaytime == 0) delaytime = 100;
  else delaytime <<= 3;
  if (delaytime > 6400) delaytime = 0;
}

void setMode(int k) {
  switch (k) {
    case 1: R = 255; G = 0; B = 0; break;
    case 2: R = 0; G = 255; B = 0; break;
    case 3: R = 0; G = 0; B = 255; break;
    case 4: R = 127; G = 0; B = 127; break;
    case 5: R = 0; G = 128; B = 128; break;
    case 6: R = 255; G = 165; B = 0; break;
    case 7: R = 255; G = 255; B = 255; break;
    default: R = 0; G = 0; B = 0; break;
  }
}
