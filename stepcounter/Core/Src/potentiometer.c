/*
 * potentiometer.c
 * Maps potentiometer ADC readings to a step goal within the max and min range rounded to the nearest goal increment size
 * Authors: Ryan Teape, Felissa Chian
 * Date: 27/04/2026
 */

#include "adc.h"
#include "app.h"
#include "task_joystick.h"

#define ADC_INDEX_POT  0
#define MAX_GOAL 15000
#define MIN_GOAL 500
#define MAX_ADC 4095.0
#define GOAL_INCREMENT_SIZE 50
#define RAW_POT_MIN 130

static uint16_t rawPotVal;

// Returns the raw ADC reading from the potentiometer channel
static uint16_t getRawPotentiometerVal  (void)
{
	return raw_adc[ADC_INDEX_POT];
}

void potentiometer_task(void)
{
	rawPotVal = getRawPotentiometerVal();

	// Clamp low ADC values to minimum goal to avoid jitter near zero
	if (rawPotVal < RAW_POT_MIN)
	{
		newGoal = MIN_GOAL;
	}
	else
	{
		// Linearly scale the ADC reading to the goal range
		newGoal = (float)(rawPotVal - RAW_POT_MIN) / (MAX_ADC - RAW_POT_MIN) * (MAX_GOAL - MIN_GOAL) + MIN_GOAL;
	}

	// Round down to the nearest increment for cleaner goal values
	newGoal = (newGoal/GOAL_INCREMENT_SIZE)*GOAL_INCREMENT_SIZE;

}


