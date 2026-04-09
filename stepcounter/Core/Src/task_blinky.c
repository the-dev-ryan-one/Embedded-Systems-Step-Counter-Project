
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


