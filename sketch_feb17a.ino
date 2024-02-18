#include <Wire.h>
#include <VL53L0X.h>
#include "spider_sense.h"

#define VIBRATION_BUTTON_PIN P6_6 // Port 6, Pin 1
VL53L0X sensor;

void setup() {
    Wire.begin();
    sensor.init();
    sensor.setTimeout(500);  // Set timeout in ms (optional)
    pinMode(VIBRATION_BUTTON_PIN, OUTPUT);

    // Set measurement timing budget to a higher value for long-range mode
    sensor.setMeasurementTimingBudget(200000); // Adjust this value as needed

    // Set the sensor to long-range mode
    sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18); // 18 is the recommended value for long-range mode
    sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14); // 14 is the recommended value for long-range mode

    Serial.begin(9600);      // Initialize serial communication
}

void loop() {
    // Start continuous measurement
    sensor.startContinuous();
    delay(100); // Allow time for measurement

    // Read distance in millimeters
    uint16_t distance = sensor.readRangeContinuousMillimeters();

    // Print calibrated distance value
    Serial.print("Calibrated Distance (mm): ");
    Serial.println(distance);

    int level = defineLevel(distance);
    int impulse = pwmButton(level);
    Serial.println(impulse);
    
    analogWrite(VIBRATION_BUTTON_PIN, impulse);
      
    delay(999); // Wait for 33 milliseconds before the next reading
}
