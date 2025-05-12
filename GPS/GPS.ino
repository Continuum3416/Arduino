#include <SoftwareSerial.h>
#include <TinyGPS.h>

long lat, lon; // create variable for latitude and longitude object
char checkchar;
int bytesavailable;

SoftwareSerial gpsSerial(3, 2); // create gps sensor connection
TinyGPS gps;                    // create gps object

void setup()
{
  Serial.begin(9600);    // connect serial
  gpsSerial.begin(9600); // connect gps sensor
}

void loop()
{
  bytesavailable = gpsSerial.available();
  // Serial.print("Bytes Available = "); Serial.println(bytesavailable);
  while (bytesavailable)
  { // check for gps data
    checkchar = gps.encode(gpsSerial.read());
    // Serial.println(checkchar);
    if (checkchar)
    {                               // decode gps data
      gps.get_position(&lat, &lon); // get latitude and longitude
      // display position
      Serial.print("Position: ");
      Serial.print("lat: ");
      Serial.print(lat);
      Serial.print(" "); //
      print latitude
          Serial.print("lon: ");
      Serial.println(lon); // print longitude
    }
    bytesavailable = gpsSerial.available();
  }
}