#ifndef spider_sense_h
#define spider_sense_h
#include <math.h>

typedef enum{
  STATE_WAIT,
  STATE_INIT_HEIGHT,
  STATE_WORK,
  STATE_NUM,
}State_t;

typedef struct{
  State_t state; 
  void (*func)(void); /* defines the function to execute */
} StateMachine_t;


extern int defineLevel(int distance);   // the function take in input the distance detected by lidar and return a corrisponding level
int pwmButton(int level);               // the function return an impulse for the buttons based on the level
int pwmBeeper(int level);               // the function return an impulse for the beeper based on the level

double avarage2(double media_precedente, int n, int nuovo_valore);
double deriveHeight(double *arr, int size);

#endif 
