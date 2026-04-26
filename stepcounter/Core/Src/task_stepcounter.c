/*
 * task_stepcounter.c
 *
 *  Created on: 14/04/2026
 *      Author: Uni
 */

#include "task_stepcounter.h"
#include "imu_lsm6ds.h"
#include "app.h"
#include "tim.h"
#include "pwm.h"

#define ALERT_DISPLAY_PERIOD 300
#define BUZZER_LENGTH 500

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_10)
	{
		steps++;
	}
}

void checkGoalComplete(void)
{
	static uint32_t buzzerStartTime = 0;
	static bool buzzerRunning = false;
	static bool alreadyAlerted = false;
	static bool alreadyAlertedDisplay = false;
	static uint32_t initialAlertTime = 0;

	if ( !(steps >= stepGoal) )
	{
		alreadyAlerted = false;
		alreadyAlertedDisplay = false;
	}

	if (steps >= stepGoal && !alreadyAlertedDisplay)
	{
		goalCompleteFlag = true;
		initialAlertTime = HAL_GetTick();
		alreadyAlertedDisplay = true;
	}

	if (alreadyAlertedDisplay && HAL_GetTick() >= initialAlertTime + ALERT_DISPLAY_PERIOD)
	{
		goalCompleteFlag = false;
	}

	if (steps >= stepGoal && !buzzerRunning && !alreadyAlerted)
	{
		buzzerStartTime = HAL_GetTick();
		HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
		buzzerRunning = true;
		alreadyAlerted = true;
	}

	if (HAL_GetTick() >= (buzzerStartTime + BUZZER_LENGTH) && buzzerRunning)
	{
		HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);
		buzzerRunning = false;
	}

}




