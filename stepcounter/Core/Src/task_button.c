/*
 * task_button.c
 * Button task module - handles button presses and controls LED brightness via PWM
 * Authors: Ryan Teape, Felissa Chian
 * Date: 2026
 */

#include "rgb.h"
#include "pwm.h"
#include "tim.h"
#include "buttons.h"
#include "app.h"
#include "task_button.h"

#define SW2PressWindow 500

static uint8_t dutyCycle = 0;
static uint32_t firstSW2press = 0;
static uint8_t consecutiveSW2Presses = 0;

void SW1PressEvent(void) {

    dutyCycle += 10;
    if (dutyCycle > 100)
    {
        dutyCycle = 0;
    }

    pwm_setDutyCycle(&htim2, TIM_CHANNEL_3, dutyCycle);
}


void button_task_execute(void)
{
	if (inSetGoalState) return;
	rgb_colour_all_on();

	if (buttons_checkButton (LEFT) == PUSHED)
	{
		steps += 7;
	}

	if (buttons_checkButton (UP) == PUSHED)
	{
		SW1PressEvent();
	}

	if (buttons_checkButton (DOWN) == PUSHED)
	{

		serialDebugFlag = !serialDebugFlag;

		consecutiveSW2Presses++;

	    if (consecutiveSW2Presses == 0)
	    {
			consecutiveSW2Presses = 1;
			firstSW2press = HAL_GetTick();
	    }
	    else if (HAL_GetTick() <= (firstSW2press + SW2PressWindow))
	    {
			testStateFlag = !testStateFlag;
			consecutiveSW2Presses = 0;
		}
	    else
	    {
			consecutiveSW2Presses = 1;
			firstSW2press = HAL_GetTick();
		}
	}

}
