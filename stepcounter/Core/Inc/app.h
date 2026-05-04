/*
 * app.h
 * Header file for the main application module
 * Authors: Ryan Teape, Felissa Chian
 * Date: 26/02/2026
 */

#ifndef APP_H
#define APP_H

#include "gpio.h"
#include "rgb.h"
#include <stdbool.h>
#include <stdint.h>

extern bool serialDebugFlag; 
extern bool testStateFlag; 

extern volatile bool clearInteruptFlag; // set true to acknowledge step ISR

extern bool goalCompleteFlag; 
extern bool inSetGoalState; 

extern volatile uint16_t steps; // total step count, incremented by ISR
extern uint16_t stepGoal; 
extern uint16_t newGoal; // pending goal value being set by potentiometer
extern uint16_t distance; // estimated distance in metres

typedef enum {
	CurrentSteps = 0, 
	GoalProgress, 
	DistanceTravelled 
} displayState;

extern displayState currDisplayState;
extern bool distanceDisplayUnitsFlag; // true = km, false = yards
extern bool stepDisplayUnitsFlag; // true = raw steps, false = percentage of goal

// initialises peripherals and runs the tick-based task scheduler
void app_main(void);

#endif /* APP_H */





