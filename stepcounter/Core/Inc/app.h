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

extern bool distanceDisplayUnitsFlag;
extern bool stepDisplayUnitsFlag;
extern displayState currDisplayState;

/* main application */
void app_main(void);





