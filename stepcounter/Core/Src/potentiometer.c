/*
 * potentiometer.c
 *
 *  Created on: 27/04/2026
 *      Author: Uni
 */
#include <stdbool.h>
#include "adc.h"
#include "app.h"
#include "task_joystick.h"

#define ADC_INDEX_POT  0
#define MAX_GOAL 15000
#define MIN_GOAL 500
#define MAX_ADC 4095.0
#define GOAL_INCREMENT_SIZE 50
#define RAW_POT_MIN 130

uint16_t rawPotVal;

static uint16_t getRawPotentiometerVal  (void)
{
	return raw_adc[ADC_INDEX_POT];
}

void potentiometer_task(void)
{
	rawPotVal = getRawPotentiometerVal();
	newGoal = (float)(rawPotVal - RAW_POT_MIN) / (MAX_ADC - RAW_POT_MIN) * (MAX_GOAL - MIN_GOAL) + MIN_GOAL;

	if (rawPotVal < RAW_POT_MIN)
	{
		newGoal = MIN_GOAL;
	}

	newGoal = (newGoal/GOAL_INCREMENT_SIZE)*GOAL_INCREMENT_SIZE;

}


