#include <Wire.h>
#include "spider_sense.h"

#define VIBRATION_BUTTON_1_PIN P6_0  // vibration button 1

void setup() {
    delay(400);            // wait 0.4 s
    Wire.begin();
    // initialize the pins modes
    pinMode(VIBRATION_BUTTON_1_PIN, OUTPUT);
}

void loop() {
    int impulse = pwmButton(1);
    analogWrite(VIBRATION_BUTTON_1_PIN, impulse);
}
