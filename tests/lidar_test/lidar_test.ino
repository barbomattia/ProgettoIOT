#include <Wire.h>
#include <stdio.h>
#include <VL53L0X.h>

#define LIDARV6 P6_1                // lidar sensor 0
#define LIDARV1 P3_2                // lidar sensor 1
#define LIDARV2 P4_0                // lidar sensor 2
#define LIDARV3 P4_2                // lidar sensor 3
#define LIDARV4 P4_1                // lidar sensor 4
#define LIDARV5 P3_3                // lidar sensor 5
#define LIDARV0 P6_0                // lidar sensor 6 - Height

//define instances for the lidar sensors: 6 lateral and 1 height
VL53L0X sensorV6;
VL53L0X sensorV1;
VL53L0X sensorV2;
VL53L0X sensorV3;
VL53L0X sensorV4;
VL53L0X sensorV5;
VL53L0X sensorV0;

void setup() {
    delay(400);            // wait 0.4 s
    Serial.begin(9600);    // Initialize serial communication 
    while ( !Serial );     // wait for the serial communication to activate properly
    Wire.begin();

    // lidar sensor & beeper
    pinMode(LIDARV0, OUTPUT); 
    pinMode(LIDARV1, OUTPUT); 
    pinMode(LIDARV2, OUTPUT);
    pinMode(LIDARV3, OUTPUT);
    pinMode(LIDARV4, OUTPUT);
    pinMode(LIDARV5, OUTPUT);
    pinMode(LIDARV6, OUTPUT);

        
    // turn off the lidars to allow re-addressing
    digitalWrite(LIDARV0, LOW); 
    digitalWrite(LIDARV1, LOW);
    digitalWrite(LIDARV2, LOW); 
    digitalWrite(LIDARV3, LOW); 
    digitalWrite(LIDARV4, LOW); 
    digitalWrite(LIDARV5, LOW);  
    digitalWrite(LIDARV6, LOW); 

    
    // lidar 1 addressing procedure
    delay(500);
    digitalWrite(LIDARV6,HIGH);  // turn on the first lidar
    delay(100);

    if(sensorV6.init()){
      delay(100);
      sensorV6.setAddress(0x29);   // set the address of the first lidar
      delay(100);
      Serial.print("Sensor 0 init with I2C adress (0x");
      Serial.print( sensorV6.getAddress(), HEX);
      Serial.println(")");
    
      // Set the sensor to long-range mode
      sensorV6.setSignalRateLimit(0.1);
      sensorV6.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
      sensorV6.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
      sensorV6.setMeasurementTimingBudget(200000);
      
      sensorV6.startContinuous(); 
    }else{
      Serial.println("Sensor 0 Not Init");
    }
}

void loop () {
  delay(2000); // Allow time for measurement
  // Read distance in millimeters
  uint16_t distance = sensorV6.readRangeContinuousMillimeters();
  // Print distance value
  Serial.print("Distance (mm): ");
  Serial.println(distance);
}

    
