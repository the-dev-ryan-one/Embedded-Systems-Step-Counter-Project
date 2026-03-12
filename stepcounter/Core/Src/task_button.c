
#include "rgb.h"
#include "buttons.h"

#include "task_button.h"



void button_task_execute(void)
{
	// Task two logic

	if (buttons_checkButton (LEFT) == PUSHED) {

	        rgb_colour_all_on();
	        rgb_led_on(RGB_LEFT);
	    } else {
	        rgb_led_off(RGB_LEFT);
	    }

	    if (buttons_checkButton (UP) == PUSHED) {

	    	rgb_colour_all_on();

	        rgb_led_on(RGB_UP);
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
