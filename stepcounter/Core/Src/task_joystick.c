
/*
 * task_joystick.c
 * Joystick task module - reads X and Y axis ADC values using DMA
 * Authors: Ryan Teape, Felissa Chian
 * Date: 2026
 */


#include "adc.h"
#include "app.h"
#include "task_joystick.h"

static uint16_t raw_adc[2];
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


#define uint16MaxVal 65535


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

#define MAX_STEP_INCREMENT 4

uint16_t getJoyStickX (void) {

	return raw_adc[1];
}

uint16_t getJoyStickY (void) {

	return raw_adc[0];
}

uint16_t getRawPotentiometerVal  (void) {

	return raw_adc[2];
}

//int16_t incrementStep(uint16_t percentageYdisplacement) {
//
//	float coeffcient = (float)(percentageYdisplacement / 100.0);
//	int16_t toReturn = (int16_t)MAX_STEP_INCREMENT * coeffcient;
//	return toReturn;
//
//}

void potentiometer_task(void) {

	rawPotVal = getRawPotentiometerVal();


}

uint32_t incrementSteps(uint16_t percentageYdisplacement) {

	return (percentageYdisplacement * percentageYdisplacement * percentageYdisplacement) / 670;

}

void testModeJoyStickTask(void) {

	if (y > ydeadZoneUpBound) {

		if (testStateFlag) {
			uint16_t decrementVal = incrementSteps(percentageYdisplacement);
			if ( steps >= decrementVal ) {
				steps -= decrementVal;
			}
		}
	}

	if (y < ydeadZoneLowerBound) {

		 if (testStateFlag) {
			 uint16_t incrementVal = incrementSteps(percentageYdisplacement);
			 if ( (steps + incrementVal) <= (stepGoal - 10) ) {
			 steps += incrementVal;
			 }
		 }
	}

}


void joystick_task(void)
{
	static char* previousJoyXDirection = "Rest";
	static char* previousJoyYDirection = "Rest";

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)raw_adc, 2);

	x = getJoyStickX();
	y = getJoyStickY();

	if (x > adcRestValX) {
			percentageXdisplacement = (x - adcRestValX)*100 / (adcMaxValX - adcRestValX);
		} else {
			percentageXdisplacement = (adcRestValX - x)*100 / (adcRestValX - adcMinValX);
		}

	if (percentageXdisplacement > 100) {
		percentageXdisplacement = 100;
	}

	if (y > adcRestValY) {
				percentageYdisplacement = (y - adcRestValY)*100 / (adcMaxValY - adcRestValY);
			} else {
				percentageYdisplacement = (adcRestValY - y)*100 / (adcRestValY - adcMinValY);
			}

	if (percentageYdisplacement > 100) {
			percentageYdisplacement = 100;
		}


	 if (y > ydeadZoneUpBound) {
		 yJoyDirection = "Down";

	 }

	 if (y < ydeadZoneLowerBound) {
		 yJoyDirection = "Up";

		 if (previousJoyYDirection == "Rest") {

			 if (currDisplayState == DistanceTravelled) {
				 distanceDisplayUnitsFlag = !distanceDisplayUnitsFlag;
			 }
			 if (!testStateFlag && (currDisplayState == GoalProgress || currDisplayState == CurrentSteps) ) {
				 stepDisplayUnitsFlag = !stepDisplayUnitsFlag;
			 }
		 }

//		 if (testStateFlag) {
//		 steps += incrementStep(percentageYdisplacement);
//		 }

		 } else {
			 yJoyDirection = "Rest";
		 }

	 previousJoyYDirection = yJoyDirection;


	 if (x == 0) return;

		 if (x > xdeadZoneUpBound) {

			xJoyDirection = "Left";
			if (previousJoyXDirection == "Rest") {

			currDisplayState = (currDisplayState + 2) % 3;
			}

			}

		 else if (x < xdeadZoneLowerBound) {

		 xJoyDirection = "Right";
		 if (previousJoyXDirection == "Rest") {
			 currDisplayState = (currDisplayState + 1) % 3;
		 }

		 } else {
			 xJoyDirection = "Rest";
		 }

		 previousJoyXDirection = xJoyDirection;



		//-----------------------
		 static uint16_t joyStickPressCounter;
		 static bool joyStickLongPress = false;

		 if (currDisplayState == GoalProgress && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 1 ) {
			 joyStickPressCounter++;
			 if (joyStickPressCounter >= 56) {
				 joyStickLongPress = true;
				 inSetGoalState = true;
			 }
		 } else {
			 joyStickPressCounter = 0;
		 }



}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){



}
