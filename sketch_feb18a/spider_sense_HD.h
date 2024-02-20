#ifndef spider_sense_HD_h
#define spider_sense_HD_h

#include <VL53L0X.h>
#include <stdio.h>

#define VIBRATION_BUTTON_0 P2_5  // vibration button 0 - a
#define VIBRATION_BUTTON_1 P5_7  // vibration button 1 - b
#define VIBRATION_BUTTON_2 P2_4  // vibration button 2 - c
#define VIBRATION_BUTTON_3 P5_6  // vibration button 3 - d
#define VIBRATION_BUTTON_4 P3_0  // vibration button 4 - e
#define VIBRATION_BUTTON_5 P2_6  // vibration button 5 - f
#define BEEPER P2_7              // beeper - g

#define BUTTON_PIN P1_1              // start button

#define LIDAR0 P6_1                // lidar sensor 0 - front
#define LIDAR1 P3_2                // lidar sensor 1 - right front
#define LIDAR2 P4_0                // lidar sensor 2 - right back
#define LIDAR3 P4_2                // lidar sensor 3 - back
#define LIDAR4 P4_1                // lidar sensor 4 - left back
#define LIDAR5 P3_3                // lidar sensor 5 - left front
#define LIDAR_HEIGHT P6_0          // lidar sensor 6 - Height


void initSpiderSense(VL53L0X (&array)[7], bool (&initialized_sensors)[7]);
double samplingHeight(VL53L0X& sensor_height);
double readHeight(VL53L0X& sensor_height);
double readDistance(VL53L0X& sensor);

void vibrateButton(int i, double intensity);
void soundBeeper(int level);
double readHeight1(VL53L0X& sensor_height);


#endif 
