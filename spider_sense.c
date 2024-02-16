#include "spider_sense.h"
#include <stdio.h>

int defineLevel(double distance) {
    if (distance > D_MAX) {
        return 0;
    } else if (distance < D_MIN) {
        return 3;
    } else {
        double interval = D_MAX - D_MIN;
        double step = interval / 4;
        int level = (int)((distance - D_MIN) / step);
        return level;
    }
}

int pwmButton(int level) {
    int impulse = level * (IMPULSE_TIME / 4);                  // derive the duration of the impulse based on the level

    printf("Generazione impulso PWM con livello %d\n", level);
    printf("Durata dell'impulso: %d microsecondi\n", impulse);
    return impulse;
}

int pwmBeeper(int level) {
    int impulse = 0;

    if (level == 3) impulse = IMPULSE_TIME / DO4_FREQUENCY;
    printf("Attivazione del cicalino con livello %d\n", level);
    printf("Durata dell'impulso: %d microsecondi\n", impulse);

    return impulse;
}