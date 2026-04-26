/*
 * pwm.c
 *
 *  Created on: Dec 18, 2024
 *      Author: fsy13
 */

#include "pwm.h"

void initialisePWM(void)
{
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	pwm_setDutyCycle(&htim2, TIM_CHANNEL_3, 0);
}

void pwm_setDutyCycle(TIM_HandleTypeDef* tim, uint32_t tim_channel, uint8_t duty)
{

	if (HAL_TIM_GetChannelState(tim, tim_channel) == HAL_TIM_CHANNEL_STATE_READY)
	{
		HAL_TIM_PWM_Start(tim, tim_channel);
	}

	uint32_t reloadValue = __HAL_TIM_GET_AUTORELOAD(tim);
	uint32_t desiredCompare = duty * (reloadValue / 100);

	__HAL_TIM_SET_COMPARE(tim, tim_channel, desiredCompare);
}

uint8_t pwm_getDutyCycle(TIM_HandleTypeDef* tim, uint32_t tim_channel)
{
	uint32_t reloadValue = __HAL_TIM_GET_AUTORELOAD(tim);
	uint32_t compareValue = __HAL_TIM_GET_COMPARE(tim, tim_channel);

	if (reloadValue == 0)
	{
		return 0;
	}

	return (compareValue * 100) / reloadValue;
}
