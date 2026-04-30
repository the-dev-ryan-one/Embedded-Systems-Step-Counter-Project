/*
 * task_button.c
 * Button task module - handles button presses and controls LED brightness via PWM
 * Authors: Ryan Teape, Felissa Chian
 * Date: 12/03/2026
 */

#include "rgb.h"
#include "pwm.h"
#include "tim.h"
#include "buttons.h"
#include "app.h"
#include "task_button.h"

#define SW2PressWindow 500 // time window in ms to detect a double press on SW2

static uint8_t dutyCycle = 0;
static uint32_t firstSW2press = 0;
static uint8_t consecutiveSW2Presses = 0;

static void SW1PressEvent(void) 
{
	// cycle LED brightness by incrementing duty cycle 10% at a time, wrapping to 0 after 100%
    dutyCycle += 10;
    if (dutyCycle > 100)
    {
        dutyCycle = 0;
    }

    pwm_setDutyCycle(&htim2, TIM_CHANNEL_3, dutyCycle);
}


void button_task_execute(void)
{
	// Skip button handling while in goal-setting state
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

		// detect double press within SW2PressWindow ms to toggle test state
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
			// window expired, reset and start tracking again
			consecutiveSW2Presses = 1;
			firstSW2press = HAL_GetTick();
		}
	}

}
