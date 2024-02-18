#include <Wire.h>
#include <VL53L0X.h>
#include "spider_sense.h"

#define VIBRATION_BUTTON_PIN P6_6 // Port 6, Pin 6
#define BEEPER_PIN P6_7 // Port 6, Pin 7
#define BUTTON_PIN P1_1

//State Machine Declaration
State_t cur_state = STATE_WAIT;

void fn_StateWait(void);
void fn_StateInitHeight(void);
void fn_StateWork(void);
void menageBeeper(int level);

StateMachine_t StateMachine[] = {
{STATE_WAIT, fn_StateWait},
{STATE_INIT_HEIGHT, fn_StateInitHeight},
{STATE_WORK, fn_StateWork},
} ;

void run(void);


VL53L0X sensor;
const unsigned sampling_intervall = 2000;
double height = 1700;


void setup() {
    Wire.begin();
    sensor.init();
    sensor.setTimeout(500);  // Set timeout in ms (optional)
    //pinMode(VIBRATION_BUTTON_PIN, OUTPUT);
    pinMode(BEEPER_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // Set measurement timing budget to a higher value for long-range mode
    sensor.setMeasurementTimingBudget(66000); // Adjust this value as needed

    // Set the sensor to long-range mode
    sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18); // 18 is the recommended value for long-range mode
    sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14); // 14 is the recommended value for long-range mode


    Serial.begin(9600);      // Initialize serial communication
}

void loop() {
    
    run();
 
} 


void fn_StateWait(void){
  Serial.println("waiting");
  
  // Leggi lo stato del pulsante
  int buttonState = digitalRead(BUTTON_PIN);
  
  // Se il pulsante è premuto, cambia lo stato
  if (buttonState == LOW) {
    Serial.println("Transitioning to State Height...");
    cur_state = STATE_INIT_HEIGHT;
  }
}

void fn_StateInitHeight(void){
  // Start continuous measurement
  sensor.startContinuous();
  delay(100); // Allow time for measurement
  
  double arrayAvarage[10];

  for(int i=0; i<10; i++){
    unsigned long startTime = millis();
    bool endTimer = false;
    double avarage = 1700;      //start value of the avarage
    int count = 1;
  
    while(!endTimer){
      uint16_t distance = sensor.readRangeContinuousMillimeters();

      // Check for sensor communication errors
      if(distance == 0xFFFF) {
        Serial.println("Error in the reading of the lidar sensor");
        distance = avarage; // put the distance equal to the actual avarage so it din't modify the real avarage
      }
      
      delay(40); // Wait for 40 milliseconds to enable the sensor to take a misurementation 
      
      Serial.print("avarage: ");  Serial.print(avarage);
      Serial.print(" count: "); Serial.print(count);
      Serial.print(" distance: "); Serial.println(distance);

      // calculate the avrage of the iteration 
      avarage = avarage2(avarage, count, distance);
      count++;
      
      if(millis() - startTime >= sampling_intervall){
        endTimer = true;
        Serial.print("Endint sampling number: ");   Serial.print(i);
        Serial.print(" with avarage:");             Serial.println(avarage);
        arrayAvarage[i] = avarage;      // save the avarage of the sampling
      }

      delay(100);
    
    }
  }
  
  height = deriveHeight(arrayAvarage, 10);
  delay(100);
  
  Serial.print("Derived Height: ");   Serial.println(height);
  delay(1000);
  
  cur_state = STATE_WORK;
  sensor.stopContinuous();
  
}

void fn_StateWork(void){
  // Start continuous measurement
    
    Serial.print("Detect distance: ");
    uint16_t distance = sensor.readRangeSingleMillimeters();

    // Check for sensor communication errors
    if(distance == 0xFFFF) { Serial.println("Error in the reading of the lidar sensor"); }

    Serial.println(distance);

    int level = defineLevel(distance);
    int impulse = pwmButton(level);
    Serial.print("level:"); Serial.print(level);
    Serial.print(" impulse:"); Serial.println(impulse);
    
    menageBeeper(level);
      
    delay(500); // Wait for an half of second to take another misurementation
}

void run(void){
  if(cur_state < STATE_NUM){
  (*StateMachine[cur_state].func)();
  }
  else{
    Serial.println("ERRORE");
  }
}

void menageBeeper(int level){
  if(level == 0){
    tone(BEEPER_PIN, 255, 100);
  }else{
    noTone(BEEPER_PIN);
  }

}
