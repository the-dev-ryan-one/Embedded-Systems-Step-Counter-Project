
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
uint16_t percentageXdisplacement = 0;
uint16_t percentageYdisplacement = 0;

uint16_t maxXValue = 3850;
uint16_t minXValue = 610;
uint16_t maxYValue = 3900;
uint16_t minYValue = 706;


uint16_t currYVal;
uint16_t x;
uint16_t y;
uint16_t rawPotVal;

char* xJoyDirection = "Rest";
char* yJoyDirection = "Rest";

static char* previousJoyXDirection = "Rest";
static char* previousJoyYDirection = "Rest";


#define uint16MaxVal 65535

//typedef struct {
//
//
//
//} JoyStickState;


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

	return (percentageYdisplacement * percentageYdisplacement * percentageYdisplacement) / 670;

}

//void testModeJoyStickTask(void) {
//
//	if (y > ydeadZoneUpBound) {
//
//		if (testStateFlag) {
//			uint16_t decrementVal = incrementSteps(percentageYdisplacement);
//			if ( steps >= decrementVal ) {
//				steps -= decrementVal;
//			}
//		}
//	}
//
//	if (y < ydeadZoneLowerBound) {
//
//		 if (testStateFlag) {
//			 uint16_t incrementVal = incrementSteps(percentageYdisplacement);
//			 if ( (steps + incrementVal) <= (stepGoal - 10) ) {
//			 steps += incrementVal;
//			 }
//		 }
//	}
//
//}


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

	float normalisedDisplacement = (float)(percentageYdisplacement)/100.0f;
	int32_t incrementVal = (int32_t)(normalisedDisplacement * normalisedDisplacement * normalisedDisplacement * MAX_STEP_INCREMENT);
	if (incrementVal < 1.0) {
		incrementVal = 1.0;
	}

	if (y > ydeadZoneUpBound) {

		if (steps <= incrementVal) {
			steps = 0;
		} else {
			steps -=  incrementVal;
		}

	}

	if (y < ydeadZoneLowerBound) {
		if ((steps + incrementVal) <= (stepGoal - 10)) {
			steps += incrementVal;
		}
		if ((steps + incrementVal) > (stepGoal - 10)) {
			steps = (stepGoal - 10);
		}

	}

}

uint16_t calcPercentageXDisplacement(void) {

	x = getJoyStickX();

	if (x > adcRestValX) {
		percentageXdisplacement = (x - adcRestValX)*100 / (adcMaxValX - adcRestValX);
	} else {
		percentageXdisplacement = (adcRestValX - x)*100 / (adcRestValX - adcMinValX);
	}

	if (percentageXdisplacement > 100) {
		percentageXdisplacement = 100;
	}

	return percentageXdisplacement;

}

uint16_t calcPercentageYDisplacement(void) {

	y = getJoyStickY();

	if (y > adcRestValY) {
		percentageYdisplacement = (y - adcRestValY)*100 / (adcMaxValY - adcRestValY);
		} else {
		percentageYdisplacement = (adcRestValY - y)*100 / (adcRestValY - adcMinValY);
		}

		if (percentageYdisplacement > 100) {
				percentageYdisplacement = 100;
		}

	return percentageYdisplacement;

}

void setJoyYDirection() {
	if (y < ydeadZoneLowerBound) {
			yJoyDirection = "Up";
	}
	else if (y > ydeadZoneUpBound) {
			 yJoyDirection = "Down";
	} else {
		yJoyDirection = "Rest";
	}
}

void setJoyXDirection() {

	 if (x > xdeadZoneUpBound) {
	        xJoyDirection = "Left";
	    } else if (x < xdeadZoneLowerBound) {
	        xJoyDirection = "Right";
	    } else {
	        xJoyDirection = "Rest";
	    }
}

void toggleUnits(void) {

	if (strcmp(yJoyDirection, "Up") == 0) {

		 if (strcmp(previousJoyYDirection, "Rest") == 0) {

			 if (currDisplayState == DistanceTravelled) {
				 distanceDisplayUnitsFlag = !distanceDisplayUnitsFlag;
			 }
			 if (!testStateFlag && (currDisplayState == GoalProgress || currDisplayState == CurrentSteps) ) {
				 stepDisplayUnitsFlag = !stepDisplayUnitsFlag;
			 }
		 }

		 } else {
			 yJoyDirection = "Rest";
		 }

	previousJoyYDirection = yJoyDirection;
}

void cycleDisplayStates(void) {

	if (x == 0) return;

		 if (x > xdeadZoneUpBound) {

			xJoyDirection = "Left";
			if (strcmp(previousJoyXDirection, "Rest") == 0) {

			currDisplayState = (currDisplayState + 2) % 3;
			}

			}

		 else if (x < xdeadZoneLowerBound) {

		 xJoyDirection = "Right";
		 if (strcmp(previousJoyXDirection, "Rest") == 0) {
			 currDisplayState = (currDisplayState + 1) % 3;
		 }

		 } else {
			 xJoyDirection = "Rest";
		 }

		 previousJoyXDirection = xJoyDirection;

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

//	percentageXdisplacement = calcPercentageXDisplacement();
//	percentageYdisplacement = calcPercentageYDisplacement();
	percentageXdisplacement = calcPercentageXDisplacement();
	percentageYdisplacement = calcPercentageYDisplacement();

	setJoyXDirection();
	setJoyYDirection();

	toggleUnits();

	cycleDisplayStates();
	handleJoyLongPress();

}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){


}
