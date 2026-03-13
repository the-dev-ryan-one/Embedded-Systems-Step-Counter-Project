
#include "rgb.h"
#include "buttons.h"

#include "app.h"

#include "task_button.h"

//void PWM_Init(void) {
//    pwm_setDutyCycle(&htim2, TIM_CHANNEL_3, 25);
//}
//
//
//uint8_t dutyCycle = 25; // initial 25%
//
//void SW1PressEvent(void) {
//
//	dutyCycle += 10;
//
//    if (dutyCycle > 100)  // wrap around
//        dutyCycle = 0;
//
//    pwm_setDutyCycle(&htim2, TIM_CHANNEL_3, dutyCycle);
//}



void button_task_execute(void)
{

	if (buttons_checkButton (LEFT) == PUSHED) {

	        rgb_colour_all_on();
	        rgb_led_on(RGB_LEFT);
	    } else {
	        rgb_led_off(RGB_LEFT);
	    }

	    if (buttons_checkButton (UP) == PUSHED) {


	    	SW1PressEvent();
//	    	rgb_colour_all_on();
//
//	        rgb_led_on(RGB_UP);

	    } else {
	        rgb_led_off(RGB_UP);
	    }

	    if (buttons_checkButton (DOWN) == PUSHED) {

	    	rgb_colour_all_on();
	        rgb_led_on(RGB_DOWN);
	    } else {
	        rgb_led_off(RGB_DOWN);
	    }

	    if (buttons_checkButton (RIGHT) == PUSHED) {

	    	rgb_colour_all_on();
	        rgb_led_on(RGB_RIGHT);
	    } else {
	        rgb_led_off(RGB_RIGHT);
	    }

}
