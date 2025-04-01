//Specify digital pin on the Arduino that the positive lead of the speaker is attached.
int speakerPin = 9;
void setup()
{
  pinMode (speakerPin, OUTPUT);
}

void loop()
{
  int melody[] = {262, 294, 330, 349, 392};
  int noteDuration = 500; 

  for (int i = 0; i < 5; i++) {
    tone(speakerPin, melody[i], noteDuration);
    delay(noteDuration + 100);
  }

  delay(2000);
}