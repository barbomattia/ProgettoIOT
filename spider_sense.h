#ifndef __SPIDER_SENSE_H__
#define __SPIDER_SENSE_H__

#define D_MAX 20                // maximum value of the detected distance
#define D_MIN 0                 // minimum value of the detected distance
#define IMPULSE_TIME 10000      // Max duretion time of an impulse in microseconds (10ms)
#define DO4_FREQUENCY 261.63    // Frequency of note Do4

int defineLevel(double distance);   // the function take in input the distance detected by lidar and return a corrisponding level
int pwmButton(int level);           // the function return an impulse for the buttons based on the level
int pwmBeeper(int level);           // the function return an impulse for the beeper based on the level

#endif 
