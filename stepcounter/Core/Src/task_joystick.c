
/*
 * task_joystick.c
 * Joystick task module - reads X and Y axis ADC values using DMA
 * Authors: Ryan Teape, Felissa Chian
 * Date: 2026
 */

#include <string.h>

#include "adc.h"
#include "app.h"
#include "task_joystick.h"

#include "tim.h"
#include "pwm.h"

static uint16_t raw_adc[3];

uint16_t maxXValue = 3850;
uint16_t minXValue = 610;
uint16_t maxYValue = 3900;
uint16_t minYValue = 706;

uint16_t rawPotVal;

#define uint16MaxVal 65535


static currJoyStickState joyStick = {
    .xJoyDirection = JOY_REST,
    .yJoyDirection = JOY_REST,
    .previousJoyXDirection = JOY_REST,
    .previousJoyYDirection = JOY_REST
};


#define adcRestValX 2203
#define adcMaxValX 3835
#define adcMinValX 454
#define xdeadZoneUpBound 2864
#define xdeadZoneLowerBound 1542

#define adcRestValY 2233
#define adcMaxValY 3900
#define adcMinValY 343
#define ydeadZoneUpBound 2679
#define ydeadZoneLowerBound 1786

#define MAX_STEP_INCREMENT 800
#define MAX_LINEAR_STEP_INCREMENT 80

uint16_t getJoyStickX (void) {

//	return raw_adc[1];
	return raw_adc[2];
}

uint16_t getJoyStickY (void) {

//	return raw_adc[0];
	return raw_adc[1];
}

uint16_t getRawPotentiometerVal  (void) {

//	return raw_adc[2];
	return raw_adc[0];
}

void checkGoalComplete(void) {

	static uint32_t buzzerStartTime = 0;
	static bool buzzerRunning = false;
	static bool alreadyAlerted = false;
	uint32_t buzzerLength = 300;

	if ( !(steps >= stepGoal) ) {
		alreadyAlerted = false;
	}

	if (steps >= stepGoal && !buzzerRunning && !alreadyAlerted) {

		buzzerStartTime = HAL_GetTick();
		HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
		buzzerRunning = true;
		alreadyAlerted = true;
	}

	if (HAL_GetTick() >= (buzzerStartTime + buzzerLength) && buzzerRunning) {

		HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);
		buzzerRunning = false;
	}

}

void potentiometer_task(void) {

	rawPotVal = getRawPotentiometerVal();

	newGoal = (float)(rawPotVal-130) / (4095.0 - 130.0) * (15000 - 500) + 500;
	if (rawPotVal < 130) {
		newGoal = 500;
	};
	newGoal = (newGoal/50)*50;

}

//uint32_t incrementSteps(uint16_t percentageYdisplacement) {
//
//	return (joyStick.percentageYdisplacement * joyStick.percentageYdisplacement * joyStick.percentageYdisplacement) / 670;
//
//}


void testModeJoyStickTask(void) {

	if (!testStateFlag) return;

	int32_t incrementVal = 0;

	if (joyStick.percentageYdisplacement < 6) {
	    incrementVal = 0;
	}
	else if (joyStick.percentageYdisplacement < 40) {
	    incrementVal = 1;
	}
	else if (joyStick.percentageYdisplacement < 65) {
	    incrementVal = 5;
	}
	else if (joyStick.percentageYdisplacement < 75) {
	    incrementVal = 10;
	}
	else if (joyStick.percentageYdisplacement < 85) {
	    incrementVal = 25;
	}
	else if (joyStick.percentageYdisplacement < 95) {
	    incrementVal = 50;
	}
	else {
	    incrementVal = 800;
	}

	if (joyStick.y > ydeadZoneUpBound) {

		if (steps <= incrementVal) {
			steps = 0;
		} else {
			steps -=  incrementVal;
		}

	}

	if (joyStick.y < ydeadZoneLowerBound) {
		if ((steps + incrementVal) <= (stepGoal - 10)) {
			steps += incrementVal;
		}
		if ((steps + incrementVal) > (stepGoal - 10)) {
			steps = (stepGoal - 10);
		}

	}

}

uint16_t calcPercentageXDisplacement(void) {

	joyStick.x = getJoyStickX();

	if (joyStick.x > adcRestValX) {
		joyStick.percentageXdisplacement = (joyStick.x - adcRestValX)*100 / (adcMaxValX - adcRestValX);
	} else {
		joyStick.percentageXdisplacement = (adcRestValX - joyStick.x)*100 / (adcRestValX - adcMinValX);
	}

	if (joyStick.percentageXdisplacement > 100) {
		joyStick.percentageXdisplacement = 100;
	}

	return joyStick.percentageXdisplacement;

}

// use const so that display cant modify joystick state
const currJoyStickState* getCurrJoyStickState(void) {
	return &joyStick;
}

uint16_t calcPercentageYDisplacement(void) {

	joyStick.y = getJoyStickY();

	if (joyStick.y > adcRestValY) {
		joyStick.percentageYdisplacement = (joyStick.y - adcRestValY)*100 / (adcMaxValY - adcRestValY);
		} else {
		joyStick.percentageYdisplacement = (adcRestValY - joyStick.y)*100 / (adcRestValY - adcMinValY);
		}

		if (joyStick.percentageYdisplacement > 100) {
				joyStick.percentageYdisplacement = 100;
		}

	return joyStick.percentageYdisplacement;

}

void setJoyYDirection() {
	if (joyStick.y < ydeadZoneLowerBound) {
			joyStick.yJoyDirection = JOY_UP;
	}
	else if (joyStick.y > ydeadZoneUpBound) {
			 joyStick.yJoyDirection = JOY_DOWN;
	} else {
		joyStick.yJoyDirection = JOY_REST;
	}
}

void setJoyXDirection() {

	 if (joyStick.x > xdeadZoneUpBound) {
	        joyStick.xJoyDirection = JOY_LEFT;
	    } else if (joyStick.x < xdeadZoneLowerBound) {
	        joyStick.xJoyDirection = JOY_RIGHT;
	    } else {
	        joyStick.xJoyDirection = JOY_REST;
	    }
}

void toggleUnits(void) {

	if (inSetGoalState) return;
	if (testStateFlag) return;

	if (joyStick.yJoyDirection == JOY_UP) {

		 if (joyStick.previousJoyYDirection == JOY_REST) {

			 if (currDisplayState == DistanceTravelled) {
				 distanceDisplayUnitsFlag = !distanceDisplayUnitsFlag;
			 }
			 if (!testStateFlag && (currDisplayState == GoalProgress || currDisplayState == CurrentSteps) ) {
				 stepDisplayUnitsFlag = !stepDisplayUnitsFlag;
			 }
		 }

		 } else {
			 joyStick.yJoyDirection = JOY_REST;
		 }

	joyStick.previousJoyYDirection = joyStick.yJoyDirection;
}

void cycleDisplayStates(void) {

	if (inSetGoalState) return;

	if (joyStick.x == 0) return;

		 if (joyStick.x > xdeadZoneUpBound) {

			joyStick.xJoyDirection = JOY_LEFT;
			if (joyStick.previousJoyXDirection == JOY_REST) {

			currDisplayState = (currDisplayState + 2) % 3;
			}

			}

		 else if (joyStick.x < xdeadZoneLowerBound) {

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

		 if (joyStickPressCounter >= 56 && !inSetGoalState &&!longPressHandled) {
			 inSetGoalState = true;
			 joyStickPressCounter = 0;
			 longPressHandled = !longPressHandled;
		} else if (joyStickPressCounter >= 56 && inSetGoalState && !longPressHandled) {
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

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){


}
