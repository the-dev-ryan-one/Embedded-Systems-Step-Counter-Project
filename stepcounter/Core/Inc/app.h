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

extern bool serialDebugFlag;
extern bool testStateFlag;
extern volatile bool clearInteruptFlag;

extern uint16_t stepGoal;
extern uint16_t newGoal;
extern volatile uint16_t steps;
extern uint16_t distance;

typedef enum {
	CurrentSteps = 0,
	GoalProgress,
	DistanceTravelled
} displayState;

extern bool distanceDisplayUnitsFlag;
extern bool stepDisplayUnitsFlag;
extern bool goalCompleteFlag;
extern bool inSetGoalState;
extern displayState currDisplayState;

void app_main(void);

#endif /* APP_H */





