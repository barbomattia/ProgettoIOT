#ifndef spider_sense_HD_h
#define spider_sense_HD_h

#include <VL53L0X.h>
#include <stdio.h>

#define VIBRATION_BUTTON_0 P2_4  // vibration button 0
#define VIBRATION_BUTTON_1 P6_7  // vibration button 1
#define VIBRATION_BUTTON_2 P2_4  // vibration button 2
#define VIBRATION_BUTTON_3 P6_7  // vibration button 3
#define VIBRATION_BUTTON_4 P2_4  // vibration button 4
#define VIBRATION_BUTTON_5 P6_7  // vibration button 5
#define BEEPER P6_7              // beeper

#define BUTTON_PIN P1_1              // start button

#define LIDAR0 P2_6                // lidar sensor 0
#define LIDAR1 P2_7                // lidar sensor 1
#define LIDAR2 P2_7                // lidar sensor 2
#define LIDAR3 P2_7                // lidar sensor 3
#define LIDAR4 P2_7                // lidar sensor 4
#define LIDAR5 P2_7                // lidar sensor 5
#define LIDAR_HEIGHT P2_7          // lidar sensor 6 - Height


void initSpiderSense(VL53L0X (&array)[7], bool (&initialized_sensors)[7]);
double samplingHeight(VL53L0X& sensor_height);
double readHeight(VL53L0X& sensor_height);
double readDistance(VL53L0X& sensor);

void vibrateButton(int i, double intensity);
void soundBeeper(int level);


#endif 
