const int xInput = A0;
const int yInput = A1;
const int zInput = A2;
const int buttonPin = 2;
unsigned long timenow;
// Raw Ranges:
// Min and Max will be initialized in the calibration
// routine, which will find the true minimum and
// maximum for each axis
int xRawMin;
int xRawMax;
int xRaw, xNorm;
int yRawMin;
int yRawMax;
int yRaw, yNorm;
int zRawMin;
int zRawMax;
int zRaw, zNorm;
// Take multiple samples to reduce noise
const int sampleSize = 10;
void setup()
{
    Serial.begin(9600);
    pinMode(buttonPin, INPUT);
    delay(500);
    Calibrate();
    Serial.print(xRawMin);
    Serial.print(" ");
    Serial.print(xRawMax);
    Serial.print(" ");
    Serial.print(yRawMin);
    Serial.print(" ");
    Serial.print(yRawMax);
    Serial.print(" ");
    Serial.print(zRawMin);
    Serial.print(" ");
    Serial.print(zRawMax);
    Serial.print("\n");
}
void loop()
{
    ReadAllAxis(&xRaw, &yRaw, &zRaw);
    timenow = millis();
    Serial.print(timenow);
    Serial.print(", ");
    Serial.print(xRaw);
    Serial.print(", ");
    Serial.print(yRaw);
    Serial.print(", ");
    Serial.print(zRaw);
    // Convert raw values to 'milli-Gs"
    long xScaled = map(xRaw, xRawMin, xRawMax, -1000, 1000);
    long yScaled = map(yRaw, yRawMin, yRawMax, -1000, 1000);
    long zScaled = map(zRaw, zRawMin, zRawMax, -1000, 1000);
    // re-scale to fractional Gs
    float xAccel = xScaled / 1000.0;
    float yAccel = yScaled / 1000.0;
    float zAccel = zScaled / 1000.0;
    Serial.print(" :: ");
    Serial.print(xAccel);
    Serial.print("G, ");
    Serial.print(yAccel);
    Serial.print("G, ");
    Serial.print(zAccel);
    Serial.println("G");
    // delay(1000);
}
// Read "sampleSize" samples and report the average
int ReadAxis(int axisPin)
{
    long reading = 0;
    analogRead(axisPin);
    delay(10); // I found without this delay, the first set of
    readings would be inaccurate, low for (int i = 0; i < sampleSize; i++)
    {
        reading += analogRead(axisPin);
    }
    return reading / sampleSize;
}
void ReadAllAxis(int *xNewptr, int *yNewptr, int *zNewptr)
{
    *xNewptr = ReadAxis(xInput);
    *yNewptr = ReadAxis(yInput);
    *zNewptr = ReadAxis(zInput);
}
void Calibrate()
{
    // initialize all min and maxs with one read
    ReadAllAxis(&xRawMin, &yRawMin, &zRawMin);
    xRawMax = xRawMin;
    yRawMax = yRawMin;
    zRawMax = zRawMin;
    // Start main calibration
    Serial.println("Calibrate");
    Serial.println("Align x-axis up. When ready press button.");
    while (digitalRead(buttonPin) != LOW)
    {
    } // wait until the button
    is pressed.ReadAllAxis(&xRaw, &yRaw, &zRaw);
    AutoCalibrate(xRaw, yRaw, zRaw);
    delay(1000);
    Serial.println("Align x-axis down. When ready press button.");
    while (digitalRead(buttonPin) != LOW)
    {
    } // wait until the button
    is pressed.ReadAllAxis(&xRaw, &yRaw, &zRaw);
    AutoCalibrate(xRaw, yRaw, zRaw);
    delay(1000);
    Serial.println("Align y-axis up. When ready press button.");
    while (digitalRead(buttonPin) != LOW)
    {
    } // wait until the button
    is pressed.ReadAllAxis(&xRaw, &yRaw, &zRaw);
    AutoCalibrate(xRaw, yRaw, zRaw);
    delay(1000);
    Serial.println("Align y-axis down. When ready press button.");
    while (digitalRead(buttonPin) != LOW)
    {
    } // wait until the button
    is pressed.ReadAllAxis(&xRaw, &yRaw, &zRaw);
    AutoCalibrate(xRaw, yRaw, zRaw);
    delay(1000);
    Serial.println("Align z-axis up. When ready press button.");
    while (digitalRead(buttonPin) != LOW)
    {
    } // wait until the button
    is pressed.ReadAllAxis(&xRaw, &yRaw, &zRaw);
    AutoCalibrate(xRaw, yRaw, zRaw);
    delay(1000);
    Serial.println("Align z-axis down. When ready press button.");
    while (digitalRead(buttonPin) != LOW)
    {
    } // wait until the button
    is pressed.ReadAllAxis(&xRaw, &yRaw, &zRaw);
    AutoCalibrate(xRaw, yRaw, zRaw);
    Serial.println("Calibration completed. \n");
}
// Find the extreme raw readings from each axis
void AutoCalibrate(int xNew, int yNew, int zNew)
{
    if (xNew < xRawMin) // update x Min and Max
    {
        xRawMin = xNew;
    }
    if (xNew > xRawMax)
    {
        xRawMax = xNew;
    }
    if (yNew < yRawMin) // update y Min and Max
    {
        yRawMin = yNew;
    }
    if (yNew > yRawMax)
    {
        yRawMax = yNew;
    }
    if (zNew < zRawMin) // update z Min and Max
    {
        zRawMin = zNew;
    }
    if (zNew > zRawMax)
    {
        zRawMax = zNew;
    }
}