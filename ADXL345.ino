#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);



float AccelMinX = 0;
float AccelMaxX = 0;
float AccelMinY = 0;
float AccelMaxY = 0;
float AccelMinZ = 0;
float AccelMaxZ = 0;



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
    Serial.print("Y: "); Serial.print(accelEvent.acceleration.y);
    Serial.print("Z: "); Serial.print(accelEvent.acceleration.z);
    Serial.println(" m/s^2");
    
    delay(100); /* 10 readings per second */
}
