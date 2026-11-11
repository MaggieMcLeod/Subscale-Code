#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <SD.h>
#include <SPI.h>

File myFile;
int count = 0;

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

float AccelMinX = 0;
float AccelMaxX = 0;
float AccelMinY = 0;
float AccelMaxY = 0;
float AccelMinZ = 0;
float AccelMaxZ = 0;

String getFilename() {
    int fileNum = 1;
    String filename;
    do {
        char name[20];
        sprintf(name, "FLIGHT%03d.csv", fileNum);
        filename = name;
        fileNum++;
    } while (SD.exists(filename.c_str()));
    return filename;
}

void setup(void)
{
  Serial.begin(9600);
  while (!Serial) delay(10);
  Serial.println("ADXL345 Accelerometer Calibration");
  Serial.println("");
  Wire1.begin();

/* Initialise the sensor */
if(!accel.begin(0x53, &Wire1))
  {
/* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
    }

Serial.print("Initializing SD card...");

if (!SD.begin(BUILTIN_SDCARD)) {
  Serial.println("initialization failed!");
  while (1);
}
Serial.println("initialization done.");

String filename = getFilename();

myFile = SD.open(filename.c_str(), FILE_WRITE);
//myFile = SD.open("work_11_10.txt", FILE_WRITE);
// if the file opened okay, write to it:
if (myFile) {
  Serial.print("Writing to test.txt...");
  myFile.println("millis, accel_x (m/s^2), accel_y (m/s^2), accel_z (m/s^2)");
} else {
  // if the file didn't open, print an error:
  Serial.println("error opening test.txt");
}

}

void loop(void)
{
/* Get a new sensor event */
    sensors_event_t accelEvent;
    accel.getEvent(&accelEvent);
    
    if (accelEvent.acceleration.x < AccelMinX) AccelMinX = accelEvent.acceleration.x;
    if (accelEvent.acceleration.x > AccelMaxX) AccelMaxX = accelEvent.acceleration.x;
    
    if (accelEvent.acceleration.y < AccelMinY) AccelMinY = accelEvent.acceleration.y;
    if (accelEvent.acceleration.y > AccelMaxY) AccelMaxY = accelEvent.acceleration.y;
    
    if (accelEvent.acceleration.z < AccelMinZ) AccelMinZ = accelEvent.acceleration.z;
    if (accelEvent.acceleration.z > AccelMaxZ) AccelMaxZ = accelEvent.acceleration.z;

/* Print the live reading */
    Serial.print("X: "); Serial.print(accelEvent.acceleration.x);
    Serial.print("  Y: "); Serial.print(accelEvent.acceleration.y);
    Serial.print("  Z: "); Serial.print(accelEvent.acceleration.z);
    Serial.println(" m/s^2");

/* Print to myFile */
   // myFile.print("X: "); myFile.print(accelEvent.acceleration.x);
   // myFile.print("  Y: "); myFile.print(accelEvent.acceleration.y);
   // myFile.print("  Z: "); myFile.print(accelEvent.acceleration.z);
   // myFile.println(" m/s^2");

    myFile.print(millis());
    myFile.print(",");
    myFile.print(accelEvent.acceleration.x);
    myFile.print(",");
    myFile.print(accelEvent.acceleration.y);
    myFile.print(",");
    myFile.println(accelEvent.acceleration.z);
    
    count++;

    if (count >= 10) {
        myFile.flush();
        count = 0;
    }

    delay(100); /* 10 readings per second */
}
