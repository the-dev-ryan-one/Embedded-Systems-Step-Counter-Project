/*
 * task_LED.c
 * LED task module - toggles the onboard LED and updates progress LEDs
 * based on step goal completion percentage.
 * Authors: Ryan Teape, Felissa Chian
 * Date: 12/03/2026
 */

#include "task_LED.h"
#include "gpio.h"
#include "rgb.h"
#include "app.h"
#include "pwm.h"

void blinky_task_execute(void)
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}

void updateProgressLEDs(void)
{
	if (stepGoal == 0)
	{
		return;
	}
	uint8_t goalCompletePercentage = (steps * 100) / (stepGoal);
	uint8_t DS3Brightness;

	// ramp DS3 LED brightness up to 100% as goal approaches 25%
	if (goalCompletePercentage >= 25)
	{
		DS3Brightness = 100;
		pwm_setDutyCycle(&htim2, TIM_CHANNEL_3, DS3Brightness);
	}
	else
	{
		DS3Brightness = (goalCompletePercentage*100) / 25;
		pwm_setDutyCycle(&htim2, TIM_CHANNEL_3, DS3Brightness);
	}

	rgb_colour_on(RGB_RED);
	rgb_colour_on(RGB_GREEN);
	rgb_colour_on(RGB_BLUE);

	// turn on additional RGB LEDs at 50%, 75%, and 100% goal completion
	if (goalCompletePercentage >= 50)
	{
		rgb_led_on(RGB_RIGHT);
	}
	else
	{
		rgb_led_off(RGB_RIGHT);
	}

	if (goalCompletePercentage >= 75)
	{
		rgb_led_on(RGB_DOWN);
	}
	else
	{
		rgb_led_off(RGB_DOWN);
	}

	if (goalCompletePercentage >= 100)
	{
		rgb_led_on(RGB_LEFT);
	}
	else
	{
		rgb_led_off(RGB_LEFT);
	}

}


