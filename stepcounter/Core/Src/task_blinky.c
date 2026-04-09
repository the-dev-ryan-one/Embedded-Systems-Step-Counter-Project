
#include "gpio.h"
#include "task_blinky.h"
#include "rgb.h"
#include "app.h"
#include "pwm.h"

/*
 * task_blinky.c
 * Blinky task module - toggles the onboard LED
 * Authors: Ryan Teape, Felissa Chian
 * Date: 2026
 */

void blinky_task_execute(void)
{
	// Task one logic
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

}

void updateProgressLEDs(void) {

	uint8_t goalCompletePrecentage = (steps * 100) / (stepGoal);

	uint8_t DS3Brightness;
	if (goalCompletePrecentage >= 25) {
		DS3Brightness = 100;
		pwm_setDutyCycle(&htim2, TIM_CHANNEL_3, DS3Brightness);
	} else {
		DS3Brightness = (goalCompletePrecentage*100) / 25;
		pwm_setDutyCycle(&htim2, TIM_CHANNEL_3, DS3Brightness);
	}

	rgb_colour_on(RGB_RED);
	rgb_colour_on(RGB_GREEN);
	rgb_colour_on(RGB_BLUE);

	if (goalCompletePrecentage >= 50) {
		rgb_led_on(RGB_RIGHT);
	} else {
		rgb_led_off(RGB_RIGHT);
	}

	if (goalCompletePrecentage >= 75) {
		rgb_led_on(RGB_DOWN);
	} else {
		rgb_led_off(RGB_DOWN);
	}

	if (goalCompletePrecentage >= 100) {
		rgb_led_on(RGB_LEFT);
	} else {
		rgb_led_off(RGB_LEFT);
	}

}


