
#include <stdbool.h>
#include "rgb.h"
#include "buttons.h"
#include "pwm.h"
#include "tim.h"
#include "app.h"
#include "task_button.h"

static uint8_t dutyCycle = 0;

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

	    } else {

	    }

	    if (buttons_checkButton (UP) == PUSHED) {


	    	SW1PressEvent();

	    }

	    if (buttons_checkButton (DOWN) == PUSHED) {

	    	serialDebugFlag = !serialDebugFlag;

	    }

	    if (buttons_checkButton (RIGHT) == PUSHED) {


	    } else {

	    }

}
