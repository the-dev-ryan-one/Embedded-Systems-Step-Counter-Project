/*
 * task_button.c
 * Button task module - handles button presses and controls LED brightness via PWM
 * Authors: Ryan Teape, Felissa Chian
 * Date: 2026
 */

#include <stdbool.h>
#include "rgb.h"
#include "buttons.h"
#include "pwm.h"
#include "tim.h"
#include "app.h"
#include "task_button.h"

static uint8_t dutyCycle = 0;
#define SW2PressWindow 700
static uint8_t consecutiveSW2Presses = 0;
static uint32_t firstSW2press = 0;


void resetSW2PressesAfterDelay(void) {
	consecutiveSW2Presses = 0;
}

void SW1PressEvent(void) {

    dutyCycle += 10;

    if (dutyCycle > 100)
        dutyCycle = 0;

    pwm_setDutyCycle(&htim2, TIM_CHANNEL_3, dutyCycle);
}


void button_task_execute(void)
{
	if (inSetGoalState) return;

	rgb_colour_all_on();

	if (buttons_checkButton (LEFT) == PUSHED) {

		steps += 7;

	    } else {

	    }

	    if (buttons_checkButton (UP) == PUSHED) {


	    	SW1PressEvent();

	    }

	    if (buttons_checkButton (DOWN) == PUSHED) {

	    	serialDebugFlag = !serialDebugFlag;

	    	consecutiveSW2Presses++;
	    	char* pressType;

	    	if (consecutiveSW2Presses == 1) {
	    		firstSW2press = HAL_GetTick();
	    		pressType = "shortPress";
	    	}

	    	if (consecutiveSW2Presses >= 2 && HAL_GetTick() <= (firstSW2press+SW2PressWindow) ) {
	    		testStateFlag = !testStateFlag;
//	    		consecutiveSW2Presses = 0;
//	    		pressType = "longPress";
	    	} else if (HAL_GetTick() > (firstSW2press+SW2PressWindow)) {
	    		consecutiveSW2Presses = 1;
	    		firstSW2press = HAL_GetTick();
	    	}

	    	if (strcmp(pressType, "longPress") == 0) {
	    		testStateFlag = !testStateFlag;
	    		consecutiveSW2Presses = 0;
	    		stepGoal = newGoal;
	    	} else if (strcmp(pressType, "longPress") == 0) {
	    		testStateFlag = !testStateFlag;
	    	}


	    }

	    if (buttons_checkButton (RIGHT) == PUSHED) {


	    } else {

	    }



}
