/*
 * task_stepcounter.c
 * Counts steps via GPIO interrupt and triggers buzzer and display alert on goal completion
 * Authors: Ryan Teape, Felissa Chian
 * Date: 14/04/2026
 */
 
#include "task_stepcounter.h"
#include "app.h"
#include "tim.h"
#include "pwm.h"

#define ALERT_DISPLAY_PERIOD 300
#define BUZZER_LENGTH 500

// increment step count on rising edge interrupt from IMU INT1 pin
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
	uint32_t currTime = HAL_GetTick();

	// reset alert flags if steps drop below goal
	if (steps < stepGoal)
	{
		alreadyAlerted = false;
		alreadyAlertedDisplay = false;
	}

	// shows goal complete screen for ALERT_DISPLAY_PERIOD ms
	if (steps >= stepGoal && !alreadyAlertedDisplay)
	{
		goalCompleteFlag = true;
		initialAlertTime = currTime;
		alreadyAlertedDisplay = true;
	}

	if (alreadyAlertedDisplay && currTime >= initialAlertTime + ALERT_DISPLAY_PERIOD)
	{
		goalCompleteFlag = false;
	}

	// sound buzzer once when goal is first reached
	if (steps >= stepGoal && !buzzerRunning && !alreadyAlerted)
	{
		buzzerStartTime = currTime;
		HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
		buzzerRunning = true;
		alreadyAlerted = true;
	}

	// stop buzzer after BUZZER_LENGTH ms
	if (currTime >= (buzzerStartTime + BUZZER_LENGTH) && buzzerRunning)
	{
		HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);
		buzzerRunning = false;
	}
}
