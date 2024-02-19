#include <Wire.h>
#include <VL53L0X.h>
#include "spider_sense.h"

#define VIBRATION_BUTTON_1_PIN P2_4 // Port 6, Pin 1
#define BEEPER_PIN P6_7 // Port 6, Pin 1
#define BUTTON_PIN P1_1

#define SHT_LOX1 P2_6
#define SHT_LOX2 P2_7


VL53L0X sensor0;
VL53L0X sensor1;


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

// derived information
const unsigned sampling_intervall = 2000;
double height = 1700;



void setup() {
    delay(400);
    Serial.begin(9600);      // Initialize serial communication 
    while ( !Serial );
    delay(5000);
    Serial.println("/////////////////SETUP/////////////////");
    
    Wire.begin();

    pinMode(VIBRATION_BUTTON_1_PIN, OUTPUT);
    pinMode(BEEPER_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
   
    pinMode(SHT_LOX1, OUTPUT); pinMode(SHT_LOX2, OUTPUT); 
    digitalWrite(SHT_LOX1, LOW); digitalWrite(SHT_LOX2, LOW); 

    delay(500);

    //power LOX1 and set adress
    digitalWrite(SHT_LOX1,HIGH);
    delay(100);

    if(sensor0.init()){
      delay(100);
      sensor0.setAddress(0x30);
      delay(100);
      Serial.print("Sensor 0 init with I2C adress (0x");
      Serial.print( sensor0.getAddress(), HEX);
      Serial.println(")");
    }else{
      Serial.println("Sensor Not Init");
    }
  
    delay(500);

    //power LOX2 and set adress
    digitalWrite(SHT_LOX2,HIGH);
    delay(100);

    if(sensor1.init()){
      delay(100);
      sensor1.setAddress(0x32);
      delay(100);
      Serial.print("Sensor 1 init with I2C adress (0x");
      Serial.print( sensor1.getAddress(), HEX);
      Serial.println(")");
    }else{
      Serial.println("Sensor1 Not Init");
    }

    delay(100);
    digitalWrite(SHT_LOX1,HIGH); digitalWrite(SHT_LOX2,HIGH);
    delay(500);

    // Set measurement timing budget to a higher value for long-range mode
    sensor0.setTimeout(50);
    sensor0.setMeasurementTimingBudget(66000); // Adjust this value as needed

    sensor1.setTimeout(50);
    sensor1.setMeasurementTimingBudget(66000); // Adjust this value as needed

    // Set the sensor to long-range mode
    sensor0.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18); // 18 is the recommended value for long-range mode
    sensor0.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14); // 14 is the recommended value for long-range mode

    // Set the sensor to long-range mode
    sensor1.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18); // 18 is the recommended value for long-range mode
    sensor1.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14); // 14 is the recommended value for long-range mode

    sensor0.startContinuous();
    sensor1.startContinuous();

    delay(5000);
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
  
  delay(100); // Allow time for measurement
  
  double arrayAvarage[10];

  for(int i=0; i<5; i++){
    unsigned long startTime = millis();
    bool endTimer = false;
    double avarage = 1700;      //start value of the avarage
    int count = 1;
  
    while(!endTimer){
      uint16_t distance = sensor0.readRangeContinuousMillimeters();

      if (sensor0.timeoutOccurred()) {
        Serial.println("TIMEOUT sensor");
        distance = avarage; // put the distance equal to the actual avarage so it din't modify the real avarage
      }else{
        // Check for sensor communication errors
        if(distance == 0xFFFF) {
          Serial.print("Error in the reading of the lidar sensor");
          distance = avarage; // put the distance equal to the actual avarage so it din't modify the real avarage
        }
        
        delay(40); // Wait for 40 milliseconds to enable the sensor to take a misurementation 
        
        Serial.print("avarage: ");  Serial.print(avarage);
        Serial.print(" count: "); Serial.print(count);
        Serial.print(" distance: "); Serial.println(distance);
  
        // calculate the avrage of the iteration 
        avarage = avarage2(avarage, count, (double)distance);
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
  }
  
  height = deriveHeight(arrayAvarage, 10);
  delay(100);
  
  Serial.print("Derived Height: ");   Serial.println(height);
  delay(1000);
  
  cur_state = STATE_WORK;
    
}

void fn_StateWork(void){
  
    Serial.print("Detect distance 0: ");
    volatile uint16_t distance0 = sensor0.readRangeContinuousMillimeters();
    delay(100);

    if (sensor0.timeoutOccurred()){
      {Serial.print(" TIMEOUT sensor 0 ");}
    }
    
    // Check for sensor communication errors
    if(distance0 == 0xFFFF) { Serial.print(" Error in the reading of the lidar sensor 0 -> "); }
    else{
       Serial.println(distance0);

      int level0 = defineLevel(distance0);
      int impulse0 = pwmButton(level0);
      Serial.print("level 0:"); Serial.println(level0);
      Serial.print("impulse 0:"); Serial.println(impulse0);
      
      menageBeeper(level0);
    }
    
    
    delay(1000);
    
    
    Serial.println();
    Serial.print("Detect distance 1: ");
    uint16_t distance1 = sensor1.readRangeContinuousMillimeters();
    delay(100);
       
    if(sensor1.timeoutOccurred()) {
      Serial.print(" TIMEOUT sensor 1 ");
    }
    
    // Check for sensor communication errors
    if(distance1 == 0xFFFF) { Serial.print(" Error in the reading of the lidar sensor 1 "); }
    else{
      Serial.println(distance1);

      int level1 = defineLevel(distance1);
      int impulse1 = pwmButton(level1);
      Serial.print("level 1:"); Serial.println(level1);
      Serial.print("impulse 1:"); Serial.println(impulse1);
      
      analogWrite(VIBRATION_BUTTON_1_PIN, impulse1);
    }
      
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
