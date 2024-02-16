#include <stdio.h>
#include "spider_sense.h"

int main() {
    double input_value = 7.5;

    int level = defineLevel(input_value);
    printf("Livello: %d\n", level);

    pwmButton(level);

    return 0;
}