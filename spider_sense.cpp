#include "spider_sense.h"
#include <stdio.h>

#define D_MAX 2000                // maximum value of the detected distance
#define D_MIN 0                 // minimum value of the detected distance
#define IMPULSE_TIME 255      // Max duretion time of an impulse in microseconds (10ms)
#define DO4_FREQUENCY 261.63    // Frequency of note Do4

int defineLevel(int distance) {
    if (distance > 2000) {
        return 4;
    } else {
        int level = (int)((distance) / 500);
        return level;
    }
}

int pwmButton(int level) {
    int impulse = (int)((4-level) * (IMPULSE_TIME / 4));                  // derive the duration of the impulse based on the level
    return impulse;
}

int pwmBeeper(int level) {
    int impulse = 0;

    if (level == 3) impulse = IMPULSE_TIME / DO4_FREQUENCY;
    printf("Attivazione del cicalino con livello %d\n", level);
    printf("Durata dell'impulso: %d microsecondi\n", impulse);

    return impulse;
}
