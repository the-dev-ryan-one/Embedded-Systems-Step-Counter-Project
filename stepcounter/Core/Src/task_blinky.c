/*
 * task_blinky.c
 * Blinky task module - toggles the onboard LED
 * Authors: Ryan Teape, Felissa Chian
 * Date: 2026
 */

#include "gpio.h"
#include "task_blinky.h"
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


