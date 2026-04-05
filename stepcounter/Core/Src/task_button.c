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
uint8_t consecutiveSW2Presses = 0;


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

	    	uint32_t currTime = HAL_GetTick();

	    	consecutiveSW2Presses++;

	    	if (consecutiveSW2Presses >= 2) {
	    		testStateFlag = !testStateFlag;
	    	}




	    }

	    if (buttons_checkButton (RIGHT) == PUSHED) {


	    } else {

	    }



}
