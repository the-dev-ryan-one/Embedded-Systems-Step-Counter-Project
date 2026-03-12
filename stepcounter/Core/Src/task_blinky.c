
#include "gpio.h"
#include "task_blinky.h"



void blinky_task_execute(void)
{
	// Task one logic
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

}
