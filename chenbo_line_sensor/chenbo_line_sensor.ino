const int pinIRd_1 = 8;
const int pinIRa_1 = A0;
const int pinLED_1 = 9;
int IRvalueA_1 = 0;
int IRvalueD_1 = 0;

const int pinIRd_2 = 7;
const int pinIRa_2 = A2;
const int pinLED_2 = 10;
int IRvalueA_2 = 0;
int IRvalueD_2 = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(pinIRd_1,INPUT);
  pinMode(pinIRa_1,INPUT);
  pinMode(pinLED_1,OUTPUT);

  pinMode(pinIRd_2,INPUT);
  pinMode(pinIRa_2,INPUT);
  pinMode(pinLED_2,OUTPUT);
  Serial.print("Aanalog Reading\t Digital Reading\n");
}

void loop()
{
  Serial.print("A1=");
  Serial.print(IRvalueA_1);
  Serial.print(" D1=");
  Serial.println(IRvalueD_1);

  Serial.print("A2=");
  Serial.print(IRvalueA_2);
  Serial.print(" D2=");
  Serial.println(IRvalueD_2);
  delay(1000);
  
  IRvalueA_1 = analogRead(pinIRa_1);
  IRvalueD_1 = digitalRead(pinIRd_1);
  IRvalueA_2 = analogRead(pinIRa_2);
  IRvalueD_2 = digitalRead(pinIRd_2);
}
