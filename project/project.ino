#include <Wire.h>
#include <VL53L0X.h>
#include "spider_sense.h"
#include "spider_sense_HD.h"


//define instances for the lidar sensors: 6 lateral and 1 height
VL53L0X sensor0;
VL53L0X sensor1;
VL53L0X sensor2;
VL53L0X sensor3;
VL53L0X sensor4;
VL53L0X sensor5;
VL53L0X sensor_height;

// Array of VL53L0X sensors
VL53L0X array_of_sensors[7] = { sensor0, sensor1, sensor2, sensor3, sensor4, sensor5, sensor_height };
bool initialized_sensors[7];


//STATE MACHINE DECLARETION
State_t cur_state = STATE_WAIT;

void fn_StateWait(void);        // the device waits for the start button to be pressed
void fn_StateInitHeight(void);  // the device starts polling the lidar and computes the standard height of the device
void fn_StateWork(void);        // the device is working  

// State Machine declaration
StateMachine_t StateMachine[] = {
{STATE_WAIT, fn_StateWait},
{STATE_INIT_HEIGHT, fn_StateInitHeight},
{STATE_WORK, fn_StateWork},
} ;


//Other usefull functions
void run(void); // start the whole system
void updateActiveSensors();

//Other global parameters
const unsigned sampling_intervall = 2000; // value of maximum timing for a sampling
double height = 1700;                     // value in mm of average use height

volatile uint16_t distance;
int level;     int impulse;


void setup() {
    delay(400);            // wait 0.4 s
    Serial.begin(9600);    // Initialize serial communication 
    while ( !Serial );     // wait for the serial communication to activate properly
    delay(5000);           // wait 5 seconds to allow processing of the height
    Serial.println("///////////////// SETUP /////////////////");
    
    Wire.begin();
    initSpiderSense(array_of_sensors, initialized_sensors);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), BUTTON1_ISR, FALLING); // Attiva l'interrupt sul pin 2

    Serial.println("///////////////// END SETUP /////////////////");
    Serial.print("Waiting");

}

void loop() {
    
    run();
 
} 


// STATE MACHINE IMPLEMENTATION FUNCTIONS
void fn_StateWait(void){
  Serial.print(".");
  delay(5000);
}


void fn_StateInitHeight(void){

  /**
   * @brief Array to store the average values.
   */
  double arrayAverage[10];

  if(initialized_sensors[6] == 0){
    Serial.println("Sensor 6 for heigt is not initialized, skip the initialization of hight");  
  }else{     
    unsigned long startTime;    bool endTimer;
    double average;             double distance;
    int count;
    
    for(int i=0; i<10; i++){
      startTime = millis();     endTimer = false;
      average = 600;           //start value of the average
      count = 1;
    
      while(!endTimer){
  
          Serial.print(" count: ");     Serial.print(count);
          distance = samplingHeight(array_of_sensors[6]);   
          delay(40);
          if(distance >= 0 ){ average = average2(average, count, distance); }   // calculate the avrage of the iteration 
      
          count++;
          
          if(millis() - startTime >= sampling_intervall){
            endTimer = true;
            Serial.print("Endint sampling number: ");   Serial.print(i);
            Serial.print(" with average:");             Serial.println(average);
            arrayAverage[i] = average;      // save the average of the sampling
          }
    
        delay(100);
      }       
    }
  }
  
  delay(100); 

  Serial.println("Array Average:");
  for(int i=0; i<10; i++){
    Serial.print(i); Serial.print(": "); Serial.println(arrayAverage[i]); 
  }
 
  height = deriveHeight(arrayAverage, 10);
  delay(100);
  
  Serial.print("Derived Height: ");   Serial.println(height);
  delay(5000);
  
  cur_state = STATE_WORK;   
}


void fn_StateWork(void){

    // pooling of the distance from the lateral sensors
    for(int i =0; i<6; i++){
      Serial.print("Sensor "); Serial.print(i); Serial.print(" -> ");
      distance = readDistance(array_of_sensors[i]);
      level = defineLevel(distance);  impulse = pwmButton(level);
      Serial.print("level :"); Serial.println(level);
      Serial.print("impulse :"); Serial.println(impulse);

      vibrateButton(i, impulse);

      delay(100);
    }

    // menage the height sensor in a different way
    distance = readHeight(array_of_sensors[6]);
    soundBeeper(distance, height);
}

void run(void){
  if(cur_state < STATE_NUM){
  (*StateMachine[cur_state].func)();
  }
  else{
    Serial.println("ERRORE");
  }
}

void BUTTON1_ISR() {
  Serial.println("Transitioning to State Height...");
  cur_state = STATE_INIT_HEIGHT;
}

/**
* Source code:
* https://www.italiantechproject.it/tutorial-arduino/sleep-mode-e-interrupt
*/
