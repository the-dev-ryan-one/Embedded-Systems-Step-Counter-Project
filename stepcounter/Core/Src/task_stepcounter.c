/*
 * task_stepcounter.c
 *
 *  Created on: 14/04/2026
 *      Author: Uni
 */

#include "task_stepcounter.h"
#include "accelerometer.h"
#include "imu_lsm6ds.h"
#include "app.h"
#include "usart.h"


#include <stdbool.h>

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_10)
	{
		steps++;
	}
}




