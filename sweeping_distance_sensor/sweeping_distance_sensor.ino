/* Sweeping Distance Sensor
The program employs a simple function that rotates the servo to the
requested degree, takes the distance measurement, scales it, and
then turns on an LED with a brightness proportional to the
distance. */
#include <Servo.h>
const int SERVO =9; //Servo on Pin 9
const int IR =0; //IR Distance Sensor on Analog Pin 0
const int LED1 =3; //LED Output 1
const int LED2 =5; //LED Output 2
const int LED3 =6; //LED Output 3
const int LED4 =11; //LED Output 4
Servo myServo; //Servo Object
int dist1 = 0; //Quadrant 1 Distance
int dist2 = 0; //Quadrant 2 Distance
int dist3 = 0; //Quadrant 3 Distance
int dist4 = 0; //Quadrant 4 Distance

void setup()
{
        // Write your setup loop.
}
void loop()
{
    //Sweep the Servo into 4 regions and change the LEDs
    //Measure IR Distance at 15 degrees
    //Adjust LED Brightness
    //delay before next measurement
    //Measure IR Distance at 65 degrees
    //Adjust LED Brightness
    //delay before next measurement
    //Measure IR Distance at 115 degrees
    //Adjust LED Brightness
    //delay before next measurement
    //Measure IR Distance at 165 degrees
    //Adjust LED Brightness
    //delay before next measurement
}
int readDistance(int pos)
{
    //Move to given position
    //Wait for Servo to move
    //Read IR Sensor
    //scale it to LED range
    //Constrain it
    //Return scaled distance
}