#include <SoftwareSerial.h>
#include <TinyGPS.h>

long lat, lon; // create variable for latitude and longitude object
char checkchar;
int bytesavailable;

// Changed from pin 3 to A3 (analog pin used as digital)
SoftwareSerial gpsSerial(A3, 2); // create gps sensor connection: RX=A3, TX=2
TinyGPS gps;                     // create gps object

void setup()
{
  Serial.begin(9600);    // connect serial
  gpsSerial.begin(9600); // connect gps sensor
}

void loop()
{
  bytesavailable = gpsSerial.available();
  while (bytesavailable)
  { // check for gps data
    checkchar = gps.encode(gpsSerial.read());
    if (checkchar)
    {                               // decode gps data
      gps.get_position(&lat, &lon); // get latitude and longitude
      // display position
      Serial.print("Position: "); 
      Serial.print("lat: ");
      Serial.print(lat);
      Serial.print(" "); // print latitude
      Serial.print("lon: ");
      Serial.println(lon); // print longitude
    }
    bytesavailable = gpsSerial.available();
  }
}
