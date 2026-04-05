
#include "gpio.h"
#include "rgb.h"

#include <stdbool.h>

/*
 * app.h
 * Header file for the main application module
 * Authors: Ryan Teape, Felissa Chian
 * Date: 2026
 */


/* flag to toggle for serial debug */
extern bool serialDebugFlag;
extern bool testStateFlag;


extern uint16_t stepGoal;
extern uint16_t steps;
extern uint16_t distance;

typedef enum {
	CurrentSteps = 0,
	GoalProgress,
	DistanceTravelled
} displayState;

typedef enum {
	km = 0,
	yd

} distanceDisplayUnits;

typedef enum {
	stepsUnit = 0,
	percentToGoal

} stepDisplayUnits;

extern displayState currDisplayState;
extern distanceDisplayUnits currDistanceDisplayUnits;


/* main application */
void app_main(void);





