#include "spider_sense.h"
#include <stdio.h>

#define IMPULSE_TIME 255      // Max duretion of an impulse 
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


double avarage2(double media_precedente, int n, int nuovo_valore){
    return ((media_precedente * n) + nuovo_valore) / (n + 1);
}

double media1(double *arr, int size) {
    double sum = 0;
    for(int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum / size;
}

double deviazione_standard(double *arr, int size, double media) {
    double sum = 0;
    for(int i = 0; i < size; i++) {
        sum += pow(arr[i] - media, 2);
    }
    return sqrt(sum / size);
}

double deriveHeight(double *arr, int size) {
    double m = media1(arr, size);
    double ds = deviazione_standard(arr, size, m);

    double sum = 0;
    int count = 0;
    for(int i = 0; i < size; i++) {
        if(fabs((arr[i] - m) / ds) <= 2) {
            sum += arr[i];
            count++;
        }
        /*else {
            printf("escluso: %.2f\n", arr[i]);
        }*/
    }
    return sum / count;
}
