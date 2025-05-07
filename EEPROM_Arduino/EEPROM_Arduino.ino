/* This code will read or write a random number sequence to the
 * Arduino onboard EEPROM.
 * Set you Serial monitor up to send no extra characters.
 */
#include <EEPROM.h>

byte zz;
int EEstart = 0; // the memory location to start reading
// it should be smaller than 1023-EEsize
int EEsize = 1024; // size in bytes of your board's EEPROM
int NofNum = 6;
char input;

void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop()
{
  Serial.println("Enter a R or W for reading or writing to the EEPROM");
  Serial.print(NofNum);
  Serial.println(" random numbers will be read or written");
  // Wait here for a character to be entered
  while (!Serial.available())
    ;
  // continue once a character is available.
  input = ' '; // reset input to a space character
  input = Serial.read();
  switch (input)
  {
  case 'R': // read stored numbers
    for (int r = 0; r < NofNum; r++)
    {
      zz = EEPROM.read(r + EEstart);
      if (r + EEstart >= EEsize)
        Serial.println("memory address wrapped around");
      Serial.print("EEPROM position: ");
      Serial.print((r + EEstart) % EEsize);
      Serial.print(" contains ");
      Serial.println(zz);
    }
    break;
  case 'W': // write random numbers
    for (int w = 0; w < NofNum; w++)
    {
      zz = random(255);
      EEPROM.write(w + EEstart, zz);
      if (w + EEstart >= EEsize)
        Serial.println("memory address wrapped around");
      Serial.print("EEPROM position: ");
      Serial.print((w + EEstart) % EEsize);
      Serial.print(" should now contain ");
      Serial.println(zz);
    }
    break;
  default:
    break;
  }
}
