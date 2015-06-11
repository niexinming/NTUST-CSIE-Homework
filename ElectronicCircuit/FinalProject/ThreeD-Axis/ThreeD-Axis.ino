#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <SoftwareSerial.h>

SoftwareSerial *BT;

void setup_bt()
{
  pinMode(13, OUTPUT);
  pinMode(2, INPUT);
  BT = new SoftwareSerial(11, 10);
  BT->begin(9600);
}

void bt_print(char *data)
{
    while(*data) {
        BT->write(*data++);
    }
}

// enum byte MessageType = { Err = -1, Init = 1, DataX, DataY, DataZ };

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
}

void setup(void)
{
  setup_bt();

  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    bt_print("ERROR_NOT_DETECTED\n");
    while(1);
  } else {
    bt_print("GOOD_CONNECTED\n");
  }

  /* Set the range to whatever is appropriate for your project */
  // accel.setRange(ADXL345_RANGE_16_G);
  // accel.setRange(ADXL345_RANGE_8_G);
  // accel.setRange(ADXL345_RANGE_4_G);
  accel.setRange(ADXL345_RANGE_2_G);
}

void loop(void)
{
  /* Get a new sensor event */
  sensors_event_t event;
  accel.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  BT->println("#");
  BT->println(event.acceleration.x);
  BT->println(event.acceleration.y);
  BT->println(event.acceleration.z);\
  delay(60);
}
