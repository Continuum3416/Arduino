// BT CAR control - using Joy BT Commander K Ash for ENGR6 at Mt SAC
// Line 145 has my turning algorithm.
// Note much the other detail is specific to a different app and H-bridge.
#define VERSION     "\n\nAndroTest V2.0 - @kas2014\ndemo for V5.x App"

// V2.0  changed to pure ASCII Communication Protocol ** not backward compatible **
// V1.4  improved communication errors handling
// V1.3  renamed for publishing, posted on 09/05/2014
// V1.2  Text display   ** not backward compatible **
// V1.1  Integer display
// V1.0  6 buttons + 4 data char implemented

// Demo setup:
// Button #1 controls pin #13 LED
// Button #4 toggle datafield display rate
// Button #5 configured as "push" button (momentary)
// Other buttons display demo message

// Arduino pin#2 to TX BlueTooth module
// Arduino pin#3 to RX BlueTooth module
// make sure your BT board is set @57600 bps
// better remove SoftSerial for PWM based projects

// For Mega 2560:
// remove   #include "SoftwareSerial.h", SoftwareSerial mySerial(2,3);
// search/replace  mySerial  >> Serial1
// pin#18 to RX bluetooth module, pin#19 to TX bluetooth module

#include "SoftwareSerial.h"

#define    STX          0x02
#define    ETX          0x03
#define    ledPin       13
#define    SLOW         750                            // Datafields refresh rate (ms)
#define    FAST         250                             // Datafields refresh rate (ms)

SoftwareSerial mySerial(2,3);                           // BlueTooth module: pin#2=TX pin#3=RX
byte cmd[8] = {0, 0, 0, 0, 0, 0, 0, 0};                 // bytes received
byte buttonStatus = 0;                                  // first Byte sent to Android device
long previousMillis = 0;                                // will store last time Buttons status was updated
long sendInterval = SLOW;                               // interval between Buttons status transmission (milliseconds)
String displayStatus = "xxxx";                          // message to Android device

// Car Motor variables and Setup
const int MR_1A = 11; //Motor Control Input A for right motor digital input pin
const int MR_1B = 10; //Motor Control Input B for right motor digital input pin
const int ML_1A = 6;  //Motor Control Input A for left motor digital input pin
const int ML_1B = 5;  //Motor Control Input B for left motor digital input pin

int ForwardRate = 0;  // Base movement rate (before turning is accounted for)
int TurnReductionL = 0;  // amount the given wheel is slowed down to implement turn in that direction
int TurnReductionR = 0;

int LMotorRate = 0;  // -100 to +100, with 0 being zero power applied - negative is reverse
int RMotorRate = 0;  // -100 to +100, with 0 being zero power applied - negative is reverse
int PrevLMRate = 0;
int PrevRMRate = 0;
int PWML, PWMR;  // Speed to be set 0 to 100 (% of power to the motor) left and right motors

void setup()  {
 Serial.begin(57600);
 mySerial.begin(9600);                                // 57600 = max value for softserial my BT required 9600
 pinMode(ledPin, OUTPUT);     
 Serial.println(VERSION);
 while(mySerial.available())  mySerial.read();         // empty RX buffer

 // Motor Setup
    pinMode(MR_1A, OUTPUT);
    pinMode(MR_1B, OUTPUT);
    pinMode(ML_1A, OUTPUT);
    pinMode(ML_1B, OUTPUT);
    stopallmotors();
}

void loop() {
 if(mySerial.available())  {                           // data received from smartphone
   delay(2);
   cmd[0] =  mySerial.read();  
   if(cmd[0] == STX)  {
     int i=1;      
     while(mySerial.available())  {
       delay(1);
       cmd[i] = mySerial.read();
       if(cmd[i]>127 || i>7)                 break;     // Communication error
       if((cmd[i]==ETX) && (i==2 || i==7))   break;     // Button or Joystick data
       i++;
     }
     if     (i==2)          getButtonState(cmd[1]);    // 3 Bytes  ex: < STX "C" ETX >
     else if(i==7)          getJoystickState(cmd);     // 6 Bytes  ex: < STX "200" "180" ETX >
   }
 } 
 sendBlueToothData(); 
}

void sendBlueToothData()  {
 static long previousMillis = 0;                             
 long currentMillis = millis();
 if(currentMillis - previousMillis > sendInterval) {   // send data back to smartphone
   previousMillis = currentMillis; 

// Data frame transmitted back from Arduino to Android device:
// < 0X02   Buttons state   0X01   DataField#1   0x04   DataField#2   0x05   DataField#3    0x03 >  
// < 0X02      "01011"      0X01     "120.00"    0x04     "-4500"     0x05  "Motor enabled" 0x03 >    // example

   mySerial.print((char)STX);                                             // Start of Transmission
   mySerial.print(getButtonStatusString());  mySerial.print((char)0x1);   // buttons status feedback
   mySerial.print(GetdataInt1());            mySerial.print((char)0x4);   // datafield #1
   mySerial.print(GetdataFloat2());          mySerial.print((char)0x5);   // datafield #2
   mySerial.print(displayStatus);                                         // datafield #3
   mySerial.print((char)ETX);                                             // End of Transmission
 }  
}

String getButtonStatusString()  {
 String bStatus = "";
 for(int i=0; i<6; i++)  {
   if(buttonStatus & (B100000 >>i))      bStatus += "1";
   else                                  bStatus += "0";
 }
 return bStatus;
}

int GetdataInt1()  {              // Data dummy values sent to Android device for demo purpose
 static int i= -30;              // Replace with your own code
 i ++;
 if(i >0)    i = -30;
 return i;  
}

float GetdataFloat2()  {           // Data dummy values sent to Android device for demo purpose
 static float i=50;               // Replace with your own code
 i-=.5;
 if(i <-50)    i = 50;
 return i;  
}

void getJoystickState(byte data[8])    {
 int joyX = (data[1]-48)*100 + (data[2]-48)*10 + (data[3]-48);       // obtain the Int from the ASCII representation
 int joyY = (data[4]-48)*100 + (data[5]-48)*10 + (data[6]-48);
 joyX = joyX - 200;                                                  // Offset to avoid
 joyY = joyY - 200;                                                  // transmitting negative numbers

 if(joyX<-100 || joyX>100 || joyY<-100 || joyY>100)     return;      // commmunication error
 
// Car control information
   // Forward Speed - percent of max
   ForwardRate=joyY;  // negative means reverse
   // Left and Right turn information (how much to slow a wheel)
   if (joyX >= 0) {TurnReductionR=joyX; TurnReductionL=0;}
   else {TurnReductionR=0; TurnReductionL=-joyX;}
   // Set the wheel speeds
   LMotorRate=ForwardRate*(100-TurnReductionL)/100;
   RMotorRate=ForwardRate*(100-TurnReductionR)/100;
   // Turn on the Motors
   PWML = map(LMotorRate,-100,100,-255,255);
   PWMR = map(RMotorRate,-100,100,-255,255);
   // checking if motor directions changing, stop motors if they are
   if ((PrevLMRate*LMotorRate<=0)||(PrevRMRate*RMotorRate<=0))
   {
      stopLmotor();
      stopRmotor();
      delay(10);
   }
   PrevLMRate=LMotorRate;
   PrevRMRate=RMotorRate;
   // Turn motors on at new rates
   if (PWML >= 0) forwardL(PWML);
   else reverseL(-PWML);
   if (PWMR >= 0) forwardR(PWMR);
   else reverseR(-PWMR);

   Serial.print("Joystick position:  ");
   Serial.print(joyX);  
   Serial.print(", ");  
   Serial.println(joyY); 
   Serial.print("Motor Rates:  ");
   Serial.print(LMotorRate);
   Serial.print(", ");
   Serial.println(RMotorRate);
}

void getButtonState(int bStatus)  {
 switch (bStatus) {
// -----------------  BUTTON #1  -----------------------
   case 'A':
     buttonStatus |= B000001;        // ON
     Serial.println("\n** Button_1: ON **");
     // your code...      
     displayStatus = "LED <ON>";
     Serial.println(displayStatus);
     digitalWrite(ledPin, HIGH);
     break;
   case 'B':
     buttonStatus &= B111110;        // OFF
     Serial.println("\n** Button_1: OFF **");
     // your code...      
     displayStatus = "LED <OFF>";
     Serial.println(displayStatus);
     digitalWrite(ledPin, LOW);
     break;

// -----------------  BUTTON #2  -----------------------
   case 'C':
     buttonStatus |= B000010;        // ON
     Serial.println("\n** Button_2: ON **");
     // your code...      
     displayStatus = "Button2 <ON>";
     Serial.println(displayStatus);
     break;
   case 'D':
     buttonStatus &= B111101;        // OFF
     Serial.println("\n** Button_2: OFF **");
     // your code...      
     displayStatus = "Button2 <OFF>";
     Serial.println(displayStatus);
     break;

// -----------------  BUTTON #3  -----------------------
   case 'E':
     buttonStatus |= B000100;        // ON
     Serial.println("\n** Button_3: ON **");
     // your code...      
     displayStatus = "Motor #1 enabled"; // Demo text message
     Serial.println(displayStatus);
     break;
   case 'F':
     buttonStatus &= B111011;      // OFF
     Serial.println("\n** Button_3: OFF **");
     // your code...      
     displayStatus = "Motor #1 stopped";
     Serial.println(displayStatus);
     break;

// -----------------  BUTTON #4  -----------------------
   case 'G':
     buttonStatus |= B001000;       // ON
     Serial.println("\n** Button_4: ON **");
     // your code...      
     displayStatus = "Datafield update <FAST>";
     Serial.println(displayStatus);
     sendInterval = FAST;
     break;
   case 'H':
     buttonStatus &= B110111;    // OFF
     Serial.println("\n** Button_4: OFF **");
     // your code...      
     displayStatus = "Datafield update <SLOW>";
     Serial.println(displayStatus);
     sendInterval = SLOW;
    break;

// -----------------  BUTTON #5  -----------------------
   case 'I':           // configured as momentary button
//      buttonStatus |= B010000;        // ON
     Serial.println("\n** Button_5: ++ pushed ++ **");
     // your code...      
     displayStatus = "Button5: <pushed>";
     break;
//   case 'J':
//     buttonStatus &= B101111;        // OFF
//     // your code...      
//     break;

// -----------------  BUTTON #6  -----------------------
   case 'K':
     buttonStatus |= B100000;        // ON
     Serial.println("\n** Button_6: ON **");
     // your code...      
      displayStatus = "Button6 <ON>"; // Demo text message
    break;
   case 'L':
     buttonStatus &= B011111;        // OFF
     Serial.println("\n** Button_6: OFF **");
     // your code...      
     displayStatus = "Button6 <OFF>";
     break;
 }
// ---------------------------------------------------------------
}

// ***************************************************************
// Motor Functions
void forwardR (int ispeed) // ispeed between 0 and 255
{
  if ((ispeed >= 0) and (ispeed <= 255))
  {
    stopRmotor ();  // ensure control pins at 0,0 before changing
    analogWrite (MR_1A, ispeed);
    analogWrite(MR_1B, LOW);
  }
}

void forwardL (int ispeed) // ispeed between 0 and 255
{
  if ((ispeed >= 0) and (ispeed <= 255))
  {
    stopLmotor ();  // ensure control pins at 0,0 before changing
    analogWrite (ML_1A, ispeed);
    analogWrite(ML_1B, LOW);
  }
}

void reverseR (int ispeed) //ispeed between 0 and 255
{
  if ((ispeed >= 0) and (ispeed <= 255))
  {
    stopRmotor ();  // ensure control pins at 0,0 before changing
    analogWrite(MR_1A, LOW);
    analogWrite (MR_1B, ispeed);
  }
}

void reverseL (int ispeed) //ispeed between 0 and 255
{
  if ((ispeed >= 0) and (ispeed <= 255))
  {
    stopLmotor ();  // ensure control pins at 0,0 before changing
    analogWrite(ML_1A, LOW);
    analogWrite (ML_1B, ispeed);
  }
}

void stopallmotors()
{
    digitalWrite(MR_1A, LOW);
    digitalWrite(MR_1B, LOW);
    digitalWrite(ML_1A, LOW);
    digitalWrite(ML_1B, LOW);
}

void stopRmotor()
{
  digitalWrite(MR_1A, LOW);
  digitalWrite(MR_1B, LOW);
}

void stopLmotor()
{
  digitalWrite(ML_1A, LOW);
  digitalWrite(ML_1B, LOW);
}
