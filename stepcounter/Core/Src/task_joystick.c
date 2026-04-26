/*
 * task_joystick.c
 * Joystick task module - reads X and Y axis ADC values using DMA
 * Authors: Ryan Teape, Felissa Chian
 * Date: 2026
 */

#include "adc.h"
#include "app.h"
#include "task_joystick.h"

#define LONG_PRESS_TICKS 56
#define RAW_POT_MIN 130
#define ADC_REST_VAL_X 2203
#define ADC_MAX_VAL_X 3835
#define ADC_MIN_VAL_X 454
#define X_DEAD_ZONE_UP_BOUND 2864
#define X_DEAD_ZONE_LOWER_BOUND 1542
#define ADC_REST_VAL_Y 2233
#define ADC_MAX_VAL_Y 3900
#define ADC_MIN_VAL_Y 343
#define Y_DEAD_ZONE_UP_BOUND 2679
#define Y_DEAD_ZONE_LOWER_BOUND 1786
#define MAX_STEP_INCREMENT 550
#define ADC_INDEX_POT  0
#define ADC_INDEX_JOY_Y 1
#define ADC_INDEX_JOY_X 2

static uint16_t raw_adc[3];
uint16_t rawPotVal;

static currJoyStickState joyStick = {
    .xJoyDirection = JOY_REST,
    .yJoyDirection = JOY_REST,
    .previousJoyXDirection = JOY_REST,
    .previousJoyYDirection = JOY_REST
};

uint16_t getJoyStickX (void)
{
	return raw_adc[ADC_INDEX_JOY_X];
}

uint16_t getJoyStickY (void)
{
	return raw_adc[ADC_INDEX_JOY_Y];
}

uint16_t getRawPotentiometerVal  (void)
{
	return raw_adc[ADC_INDEX_POT];
}

void potentiometer_task(void)
{
	rawPotVal = getRawPotentiometerVal();
	newGoal = (float)(rawPotVal - RAW_POT_MIN) / (4095.0 - RAW_POT_MIN) * (15000 - 500) + 500;

	if (rawPotVal < RAW_POT_MIN)
	{
		newGoal = 500;
	}

	newGoal = (newGoal/50)*50;

}

void testModeJoyStickTask(void)
{

	if (inSetGoalState) return;
	if (!testStateFlag) return;
	int32_t incrementVal = 0;

	if (joyStick.percentageYdisplacement < 4)
	{
	    incrementVal = 0;
	}
	else if (joyStick.percentageYdisplacement < 35)
	{
	    incrementVal = 1;
	}
	else if (joyStick.percentageYdisplacement < 45)
	{
	    incrementVal = 2;
	}
	else if (joyStick.percentageYdisplacement < 55)
	{
	    incrementVal = 5;
	}
	else if (joyStick.percentageYdisplacement < 63)
	{
	    incrementVal = 10;
	}
	else if (joyStick.percentageYdisplacement < 71)
	{
	    incrementVal = 15;
	}
	else if (joyStick.percentageYdisplacement < 95)
	{
	    incrementVal = 25 + (joyStick.percentageYdisplacement - 71) * 225 / 24;
	} else {
	    incrementVal = MAX_STEP_INCREMENT;
	}

	if (joyStick.y > Y_DEAD_ZONE_UP_BOUND)
	{

		if (steps <= incrementVal)
		{
			steps = 0;
		} else {
			steps -=  incrementVal;
		}

	}

	if (joyStick.y < Y_DEAD_ZONE_LOWER_BOUND)
	{
		if ((steps + incrementVal) <= (stepGoal - 10))
		{
			steps += incrementVal;
		}
		if ((steps + incrementVal) > (stepGoal - 10))
		{
			steps = (stepGoal - 10);
		}

	}

}

uint16_t calcPercentageXDisplacement(void)
{

	joyStick.x = getJoyStickX();

	if (joyStick.x > ADC_REST_VAL_X)
	{
		joyStick.percentageXdisplacement = (joyStick.x - ADC_REST_VAL_X)*100 / (ADC_MAX_VAL_X - ADC_REST_VAL_X);
	} else {
		joyStick.percentageXdisplacement = (ADC_REST_VAL_X - joyStick.x)*100 / (ADC_REST_VAL_X - ADC_MIN_VAL_X);
	}

	if (joyStick.percentageXdisplacement > 100)
	{
		joyStick.percentageXdisplacement = 100;
	}

	return joyStick.percentageXdisplacement;

}

const currJoyStickState* getCurrJoyStickState(void)
{
	return &joyStick;
}

uint16_t calcPercentageYDisplacement(void)
{

	joyStick.y = getJoyStickY();

	if (joyStick.y > ADC_REST_VAL_Y)
	{
		joyStick.percentageYdisplacement = (joyStick.y - ADC_REST_VAL_Y)*100 / (ADC_MAX_VAL_Y - ADC_REST_VAL_Y);
		} else {
		joyStick.percentageYdisplacement = (ADC_REST_VAL_Y - joyStick.y)*100 / (ADC_REST_VAL_Y - ADC_MIN_VAL_Y);
		}

		if (joyStick.percentageYdisplacement > 100)
		{
				joyStick.percentageYdisplacement = 100;
		}

	return joyStick.percentageYdisplacement;

}

void setJoyYDirection()
{
	if (joyStick.y < Y_DEAD_ZONE_LOWER_BOUND)
	{
			joyStick.yJoyDirection = JOY_UP;
	}
	else if (joyStick.y > Y_DEAD_ZONE_UP_BOUND) {
			 joyStick.yJoyDirection = JOY_DOWN;
	} else {
		joyStick.yJoyDirection = JOY_REST;
	}
}

void setJoyXDirection()
{

	 if (joyStick.x > X_DEAD_ZONE_UP_BOUND)
	 {
	        joyStick.xJoyDirection = JOY_LEFT;
	    } else if (joyStick.x < X_DEAD_ZONE_LOWER_BOUND) {
	        joyStick.xJoyDirection = JOY_RIGHT;
	    } else {
	        joyStick.xJoyDirection = JOY_REST;
	    }
}

void toggleUnits(void)
{

	if (inSetGoalState) return;
	if (testStateFlag) return;

	if (joyStick.yJoyDirection == JOY_UP)
	{

		 if (joyStick.previousJoyYDirection == JOY_REST)
		 {

			 if (currDisplayState == DistanceTravelled)
			 {
				 distanceDisplayUnitsFlag = !distanceDisplayUnitsFlag;
			 }
			 if (!testStateFlag && (currDisplayState == GoalProgress || currDisplayState == CurrentSteps) )
			 {
				 stepDisplayUnitsFlag = !stepDisplayUnitsFlag;
			 }
		 }

		 } else {
			 joyStick.yJoyDirection = JOY_REST;
		 }

	joyStick.previousJoyYDirection = joyStick.yJoyDirection;
}

void cycleDisplayStates(void)
{

	if (inSetGoalState) return;

	if (joyStick.x == 0) return;

		 if (joyStick.x > X_DEAD_ZONE_UP_BOUND)
		 {

			joyStick.xJoyDirection = JOY_LEFT;
			if (joyStick.previousJoyXDirection == JOY_REST)
			{

			currDisplayState = (currDisplayState + 2) % 3;
			}

			}

		 else if (joyStick.x < X_DEAD_ZONE_LOWER_BOUND) {

		 joyStick.xJoyDirection = JOY_RIGHT;
		 if (joyStick.previousJoyXDirection == JOY_REST) {
			 currDisplayState = (currDisplayState + 1) % 3;
		 }

		 } else {
			 joyStick.xJoyDirection = JOY_REST;
		 }

		 joyStick.previousJoyXDirection = joyStick.xJoyDirection;

}

void handleJoyLongPress(void) {

	if (currDisplayState != GoalProgress) return;

	 static uint16_t joyStickPressCounter = 0;
	 static bool longPressHandled = false;

	 if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 1 ) {

		 joyStickPressCounter++;

		 if (joyStickPressCounter >= LONG_PRESS_TICKS && !inSetGoalState &&!longPressHandled) {
			 inSetGoalState = true;
			 joyStickPressCounter = 0;
			 longPressHandled = !longPressHandled;
		} else if (joyStickPressCounter >= LONG_PRESS_TICKS && inSetGoalState && !longPressHandled) {
			stepGoal = newGoal;
			inSetGoalState = false;
			joyStickPressCounter = 0;
			longPressHandled = !longPressHandled;
		}

	 } else {

		 if (joyStickPressCounter > 0 && inSetGoalState && !longPressHandled){
			 inSetGoalState = false;
			 joyStickPressCounter = 0;
		 }

		 joyStickPressCounter = 0;
		 longPressHandled = false;

	 	 }
}
void joystick_task(void)
{

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)raw_adc, 3);

	joyStick.percentageXdisplacement = calcPercentageXDisplacement();
	joyStick.percentageYdisplacement = calcPercentageYDisplacement();

	setJoyXDirection();
	setJoyYDirection();
	toggleUnits();
	cycleDisplayStates();
	handleJoyLongPress();

}
