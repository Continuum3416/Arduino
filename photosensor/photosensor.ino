const int LED=9;//The LED is connected to pin 9
const int PIR=2;//The PIR is
void setup()
{
  Serial.begin(9600);
  pinMode (LED, OUTPUT);
  pinMode (PIR, INPUT);
  digitalWrite(LED,HIGH);
  delay (500);
  digitalWrite (LED,LOW);
  delay (500);
}
void loop()
{
  if (digitalRead(PIR) == LOW)
  {
    digitalWrite(LED, LOW);
    Serial.println("High");
  }
  else
  {
    digitalWrite(LED, HIGH);
    Serial.println("Low");
  }
}