
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

static uint16_t raw_adc[3];

uint16_t maxXValue = 3850;
uint16_t minXValue = 610;
uint16_t maxYValue = 3900;
uint16_t minYValue = 706;

uint16_t rawPotVal;

#define uint16MaxVal 65535

typedef enum {

	LEFT = 0,
	RIGHT,
	UP,
	DOWN,
	REST

} joyStickDirections;

static currJoyStickState joyStick = {
    .xJoyDirection = REST,
    .yJoyDirection = REST,
    .previousJoyXDirection = REST,
    .previousJoyYDirection = REST
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

#define MAX_STEP_INCREMENT 100

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


void potentiometer_task(void) {

	rawPotVal = getRawPotentiometerVal();

	newGoal = (float)(rawPotVal-130) / (4095.0 - 130.0) * (15000 - 500) + 500;
	if (rawPotVal < 130) {
		newGoal = 500;
	};
	newGoal = (newGoal/50)*50;

}

uint32_t incrementSteps(uint16_t percentageYdisplacement) {

	return (percentageYdisplacement * joyStick.percentageYdisplacement * joyStick.percentageYdisplacement) / 670;

}


void testModeJoyStickTask(void) {

	if (!testStateFlag) return;


//	if (percentageYdisplacement < 10) {
//	        incrementVal = 0;
//	    }
//	    else if (percentageYdisplacement < 25) {
//	        incrementVal = 1;
//	    }
//	    else if (percentageYdisplacement < 40) {
//	        incrementVal = 1;
//	    }
//	    else if (percentageYdisplacement < 55) {
//	        incrementVal = 2;
//	    }
//	    else if (percentageYdisplacement < 70) {
//	        incrementVal = 3;
//	    }
//	    else if (percentageYdisplacement < 82) {
//	        incrementVal = 5;
//	    }
//	    else if (percentageYdisplacement < 92) {
//	        incrementVal = 8;
//	    }
//	    else {
//	        incrementVal = 15;
//	    }

	float normalisedDisplacement = (float)(joyStick.percentageXdisplacement)/100.0f;
	int32_t incrementVal = (int32_t)(normalisedDisplacement * normalisedDisplacement * normalisedDisplacement * MAX_STEP_INCREMENT);
	if (incrementVal < 1.0) {
		incrementVal = 1.0;
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
		joyStick.percentageXdisplacement = (joyStick.y - adcRestValY)*100 / (adcMaxValY - adcRestValY);
		} else {
		joyStick.percentageXdisplacement = (adcRestValY - joyStick.y)*100 / (adcRestValY - adcMinValY);
		}

		if (joyStick.percentageXdisplacement > 100) {
				joyStick.percentageXdisplacement = 100;
		}

	return joyStick.percentageXdisplacement;

}

void setJoyYDirection() {
	if (joyStick.y < ydeadZoneLowerBound) {
			joyStick.yJoyDirection = UP;
	}
	else if (joyStick.y > ydeadZoneUpBound) {
			 joyStick.yJoyDirection = DOWN;
	} else {
		joyStick.yJoyDirection = REST;
	}
}

void setJoyXDirection() {

	 if (joyStick.x > xdeadZoneUpBound) {
	        joyStick.xJoyDirection = LEFT;
	    } else if (joyStick.x < xdeadZoneLowerBound) {
	        joyStick.xJoyDirection = RIGHT;
	    } else {
	        joyStick.xJoyDirection = REST;
	    }
}

void toggleUnits(void) {

	if (joyStick.yJoyDirection == UP) {

		 if (joyStick.previousJoyYDirection == REST) {

			 if (currDisplayState == DistanceTravelled) {
				 distanceDisplayUnitsFlag = !distanceDisplayUnitsFlag;
			 }
			 if (!testStateFlag && (currDisplayState == GoalProgress || currDisplayState == CurrentSteps) ) {
				 stepDisplayUnitsFlag = !stepDisplayUnitsFlag;
			 }
		 }

		 } else {
			 joyStick.yJoyDirection = REST;
		 }

	joyStick.previousJoyYDirection = joyStick.yJoyDirection;
}

void cycleDisplayStates(void) {

	if (joyStick.x == 0) return;

		 if (joyStick.x > xdeadZoneUpBound) {

			joyStick.xJoyDirection = LEFT;
			if (joyStick.previousJoyXDirection == REST) {

			currDisplayState = (currDisplayState + 2) % 3;
			}

			}

		 else if (joyStick.x < xdeadZoneLowerBound) {

		 joyStick.xJoyDirection = RIGHT;
		 if (joyStick.previousJoyXDirection == REST) {
			 currDisplayState = (currDisplayState + 1) % 3;
		 }

		 } else {
			 joyStick.xJoyDirection = REST;
		 }

		 joyStick.previousJoyXDirection = joyStick.xJoyDirection;

}

void handleJoyLongPress(void) {

	 static uint16_t joyStickPressCounter;
	 static bool joyStickLongPress = false;

	 if (currDisplayState == GoalProgress && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 1 ) {
		 joyStickPressCounter++;
		 if (joyStickPressCounter >= 56 && !joyStickLongPress) {
			 joyStickLongPress = true;

			 if (inSetGoalState) {
				 stepGoal = newGoal;
				 inSetGoalState = !inSetGoalState;
			 } else {
				 inSetGoalState = !inSetGoalState;
			 }
		 }
	 } else {
		 joyStickPressCounter = 0;
		 joyStickLongPress = false;
	 }
}


void joystick_task(void)
{

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)raw_adc, 3);

	joyStick.percentageXdisplacement = calcPercentageXDisplacement();
	joyStick.percentageXdisplacement = calcPercentageYDisplacement();

	setJoyXDirection();
	setJoyYDirection();

	toggleUnits();

	cycleDisplayStates();
	handleJoyLongPress();

}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){


}
