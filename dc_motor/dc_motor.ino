//Simple Motor Speed Control Program
const int MOTOR=9; //Motor on Digital Pin 9
void setup()
{
    pinMode (MOTOR, OUTPUT);
}

void loop()
{
    // ramp up the speed, delay 10 ms after write for motor to react
    delay(2000);
    // ramp down the speed, delay 10 ms after write for motor to react
    delay(2000);
}