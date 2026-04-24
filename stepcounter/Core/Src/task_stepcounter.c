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

//void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin) {
//
//	char buffer[32];
//	snprintf(buffer, sizeof(buffer), "EXTI fired: pin %d\r\n", GPIO_Pin);
//	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
//
//    if (GPIO_Pin == GPIO_PIN_10) {
//        steps = getStepCount();
//    }
//}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "EXTI fired: pin %d\r\n", GPIO_Pin);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);

	if (GPIO_Pin && GPIO_PIN_10)
	{
		steps++;
	}
}




