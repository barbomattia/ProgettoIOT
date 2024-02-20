#include "spider_sense_HD.h"
#include <VL53L0X.h>
#include <Wire.h>
#include <stdio.h>
#include "spider_sense.h"

void shotDownAllBotton();

void initSpiderSense(VL53L0X (&array)[7], bool (&initialized_sensors)[7]){
    
    // initialize the pins modes
    // vibration button pins
    pinMode(VIBRATION_BUTTON_0, OUTPUT);
    pinMode(VIBRATION_BUTTON_1, OUTPUT);
    pinMode(VIBRATION_BUTTON_2, OUTPUT);
    pinMode(VIBRATION_BUTTON_3, OUTPUT);
    pinMode(VIBRATION_BUTTON_4, OUTPUT);
    pinMode(VIBRATION_BUTTON_5, OUTPUT);
    
    // beeper pin
    pinMode(BEEPER, OUTPUT);
    
    // button pin
    pinMode(BUTTON_PIN, INPUT_PULLUP);
       
    
    // lidar sensor & beeper
    pinMode(LIDAR0, OUTPUT); 
    pinMode(LIDAR1, OUTPUT); 
    pinMode(LIDAR2, OUTPUT);
    pinMode(LIDAR3, OUTPUT);
    pinMode(LIDAR4, OUTPUT);
    pinMode(LIDAR5, OUTPUT);
    pinMode(LIDAR_HEIGHT, OUTPUT);

        
    // turn off the lidars to allow re-addressing
    digitalWrite(LIDAR0, LOW); 
    digitalWrite(LIDAR1, LOW);
    digitalWrite(LIDAR2, LOW); 
    digitalWrite(LIDAR3, LOW); 
    digitalWrite(LIDAR4, LOW); 
    digitalWrite(LIDAR5, LOW);  
    digitalWrite(LIDAR_HEIGHT, LOW); 

    
    // lidar 1 addressing procedure
    /*
    delay(500);
    digitalWrite(LIDAR0,HIGH);  // turn on the first lidar
    delay(100);

    if(array[0].init()){
      delay(100);
      array[0].setAddress(0x30);   // set the address of the first lidar
      delay(100);
      Serial.print("Sensor 0 Front init with I2C adress (0x");
      Serial.print( array[0].getAddress(), HEX);
      Serial.println(")");
      initialized_sensors[0] = true;
    }else{
      Serial.println("Sensor 0 Not Init");
      initialized_sensors[0] = false;
    }
    */
    initialized_sensors[0] = false;
    
    
    // lidar 1 addressing procedure
    delay(500);
    digitalWrite(LIDAR1,HIGH);  // turn on the first lidar
    delay(100);
    
    if(array[1].init()){
      delay(100);
      array[1].setAddress(0x39);   // set the address of the first lidar
      delay(100);
      Serial.print("Sensor 1 Right Front init with I2C adress (0x");
      Serial.print( array[1].getAddress(), HEX);
      Serial.println(")");
      initialized_sensors[1] = true;
    }else{
      Serial.println("Sensor 1 Not Init");
      initialized_sensors[1] = false;
    }
    


    // lidar 2 addressing procedure
    delay(500);
    digitalWrite(LIDAR2,HIGH);  // turn on the first lidar
    delay(100);

    if(array[2].init()){
      delay(100);
      array[2].setAddress(0x31);   // set the address of the first lidar
      delay(100);
      Serial.print("Sensor 2 Right Back init with I2C adress (0x");
      Serial.print( array[2].getAddress(), HEX);
      Serial.println(")");
      initialized_sensors[2] = true;
    }else{
      Serial.println("Sensor 2 Not Init");
      initialized_sensors[2] = false;
    }


    // lidar 3 addressing procedure
    delay(500);
    digitalWrite(LIDAR3,HIGH);  // turn on the first lidar
    delay(100);

    if(array[3].init()){
      delay(100);
      array[3].setAddress(0x32);   // set the address of the first lidar
      delay(100);
      Serial.print("Sensor 3 Back init with I2C adress (0x");
      Serial.print( array[3].getAddress(), HEX);
      Serial.println(")");
      initialized_sensors[3] = true;
    }else{
      Serial.println("Sensor 3 Not Init");
      initialized_sensors[3] = false;
    }


    // lidar 4 addressing procedure
    delay(500);
    digitalWrite(LIDAR4,HIGH);  // turn on the first lidar
    delay(100);

    if(array[4].init()){
      delay(100);
      array[4].setAddress(0x33);   // set the address of the first lidar
      delay(100);
      Serial.print("Sensor 4 Left Back init with I2C adress (0x");
      Serial.print( array[4].getAddress(), HEX);
      Serial.println(")");
      initialized_sensors[4] = true;
    }else{
      Serial.println("Sensor 4 Not Init");
      initialized_sensors[4] = false;
    }


    // lidar 5 addressing procedure
    delay(500);
    digitalWrite(LIDAR5,HIGH);  // turn on the first lidar
    delay(100);

    if(array[5].init()){
      delay(100);
      array[5].setAddress(0x34);   // set the address of the first lidar
      delay(100);
      Serial.print("Sensor 5 Left Front init with I2C adress (0x");
      Serial.print( array[5].getAddress(), HEX);
      Serial.println(")");
      initialized_sensors[5] = true;
    }else{
      Serial.println("Sensor 5 Not Init");
      initialized_sensors[5] = false;
    }


    // lidar 6 addressing procedure
    delay(500);
    digitalWrite(LIDAR_HEIGHT,HIGH);  // turn on the first lidar
    delay(100);

    if(array[6].init()){
      delay(100);
      array[6].setAddress(0x35);   // set the address of the first lidar
      delay(100);
      Serial.print("Sensor 6 Height init with I2C adress (0x");
      Serial.print( array[6].getAddress(), HEX);
      Serial.println(")");
      initialized_sensors[6] = true;
    }else{
      Serial.println("Sensor 6 height Not Init");
      initialized_sensors[6] = false;
    }
    delay(500);

    
    // Set measurement timing budget to a higher value for long-range mode
    for(int i = 0; i<7; i++){
      array[i].setTimeout(50);
      array[i].setMeasurementTimingBudget(33000);

      // Set the sensor to long-range mode
      array[i].setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18); // 18 is the recommended value for long-range mode
      array[i].setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14); // 14 is the recommended value for long-range mode

      array[i].setSignalRateLimit(0.1);

      array[i].startContinuous(); 

    }

    Serial.print("End Inizializations, returns: (");
    for(int i = 0; i<7; i++){
      Serial.print(initialized_sensors[i]); Serial.print(" ");
    }
    Serial.println(" )");

    delay(1000);
}


double samplingHeight(VL53L0X& sensor_height){
  
  uint16_t distance = sensor_height.readRangeContinuousMillimeters();
  delay(40);     // Wait for 40 milliseconds to enable the sensor to take a misurementation 

  if (sensor_height.timeoutOccurred()) {
    Serial.println("TIMEOUT sensor");
    return  -1;           // put the distance equal to the actual avarage so it din't modify the real avarage
  }else{
    // Check for sensor communication errors
    if(distance == 0xFFFF) {
      Serial.print("Error in the reading of the lidar sensor");
      return  -1;          // put the distance equal to the actual avarage so it din't modify the real avarage
    }
    
    Serial.print(" distance: ");  Serial.println(distance); 
  }

  return (double)distance;    
}


double readHeight(VL53L0X& sensor_height){
  Serial.println();
  Serial.print("Detect Height: ");
  uint16_t distance = sensor_height.readRangeContinuousMillimeters();
  delay(100);
     
  if(sensor_height.timeoutOccurred()) {
    Serial.print(" TIMEOUT sensor height ");
    distance = 3000;
  }
  
  // Check for sensor communication errors
  if(distance == 0xFFFF) { 
    Serial.print(" Error in the reading of the lidar height sensor "); 
    distance = 3000;
  }else{
    Serial.println(distance);        
  }
  return distance;
}


double readDistance(VL53L0X& sensor){
  Serial.print("Detect distance: ");
  uint16_t distance = sensor.readRangeContinuousMillimeters();
  delay(100);
     
  if(sensor.timeoutOccurred()) {
    Serial.print(" TIMEOUT sensor ");
    distance = 3000;
  }
  
  // Check for sensor communication errors
  if(distance == 0xFFFF) { 
    Serial.print(" Error in the reading of the lidar sensor 1 "); 
    distance = 3000;
  }else{
    Serial.println(distance);        
  }
  return distance;
}


void vibrateButton(int i, double intensity){
  switch (i) {
    case 0:
      Serial.print("Vibration button 0, with intensity: "); Serial.println(intensity);
      //shotDownAllBotton();
      analogWrite(VIBRATION_BUTTON_0, intensity);
      break;
    case 1:
      Serial.print("Vibration button 1, with intensity: "); Serial.println(intensity);
      //shotDownAllBotton();
      analogWrite(VIBRATION_BUTTON_1, intensity);
      break;
    case 2:
      Serial.print("Vibration button 2, with intensity: "); Serial.println(intensity);
      //shotDownAllBotton();
      analogWrite(VIBRATION_BUTTON_2, intensity);
      break;
    case 3:
      Serial.print("Vibration button 3, with intensity: "); Serial.println(intensity);
      //shotDownAllBotton();
      analogWrite(VIBRATION_BUTTON_3, intensity);
      break;
    case 4:
      Serial.print("Vibration button 4, with intensity: "); Serial.println(intensity);
      //shotDownAllBotton();
      analogWrite(VIBRATION_BUTTON_4, intensity);
      break;
    case 5:
      Serial.print("Vibration button 5, with intensity: "); Serial.println(intensity);
      //shotDownAllBotton();
      analogWrite(VIBRATION_BUTTON_5, intensity);
      break;
    default:
      Serial.print("ERROR in the vibration Error");    
      //shotDownAllBotton();  
  }
}


void soundBeeper(int measured_height, int standard_height ){
  Serial.print("measured height: "); Serial.println(measured_height);
  Serial.print("standard height: "); Serial.println(standard_height); 
  if(measured_height > standard_height + 60){
    Serial.println("Sound on the beeper: HOLE");
    tone(BEEPER, 5, 700);
  }else{
    if(measured_height < (standard_height - 100 ) ){
      if(measured_height > (standard_height - 300 ) ){
        Serial.println("Sound on the beeper: STEP");
        tone(BEEPER, 10, 700);
      }else{
        Serial.println("Sound on the beeper: HIGHER OBSTACLE");
        tone(BEEPER, 500 , 700);
      }
    }else{
        Serial.println("No height ostacle" );
        noTone(BEEPER);
    }
    
  }
  
}

void shotDownAllBotton(){
  digitalWrite(VIBRATION_BUTTON_0, 0); digitalWrite(VIBRATION_BUTTON_1, 0); digitalWrite(VIBRATION_BUTTON_2, 0);
  digitalWrite(VIBRATION_BUTTON_3, 0); digitalWrite(VIBRATION_BUTTON_4, 0); digitalWrite(VIBRATION_BUTTON_5, 0);
   
}
