/*
 * task_display.c
 * Display task module - updates the OLED screen with joystick values
 * and optionally outputs debug info over UART
 * Authors: Ryan Teape, Felissa Chian
 * Date: 2026
 */

#include <stdio.h>
#include "task_joystick.h"

#include "task_display.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "ssd1306_conf.h"
#include "usart.h"
#include "app.h"
#include <string.h>

char buffer[32];

void displayCurrentSteps(void) {

	ssd1306_SetCursor(0, 0);
	snprintf(buffer, sizeof(buffer), "-------------------");
	ssd1306_WriteString(buffer, Font_7x10, White);

	ssd1306_SetCursor(0, 11);
	snprintf(buffer, sizeof(buffer), "  Current Steps  ");
	ssd1306_WriteString(buffer, Font_7x10, White);

	ssd1306_SetCursor(0, 21);
	snprintf(buffer, sizeof(buffer), "-------------------");
	ssd1306_WriteString(buffer, Font_7x10, White);

	ssd1306_SetCursor(52, 40);

	if (stepDisplayUnitsFlag) {
		snprintf(buffer, sizeof(buffer), "%u steps", steps);
		ssd1306_WriteString(buffer, Font_7x10, White);
	} else {
		uint8_t goalPercentage = (uint32_t)steps * 100 / stepGoal;
		snprintf(buffer, sizeof(buffer), "%u %%" , goalPercentage);
		ssd1306_WriteString(buffer, Font_7x10, White);
	}

}


void displayGoalProgress(void) {

	ssd1306_SetCursor(0, 0);
	snprintf(buffer, sizeof(buffer), "-------------------");
	ssd1306_WriteString(buffer, Font_7x10, White);

	ssd1306_SetCursor(0, 11);
	snprintf(buffer, sizeof(buffer), "  Goal Progress  ");
	ssd1306_WriteString(buffer, Font_7x10, White);

	ssd1306_SetCursor(0, 21);
	snprintf(buffer, sizeof(buffer), "-------------------");
	ssd1306_WriteString(buffer, Font_7x10, White);

	ssd1306_SetCursor(52, 40);

	// make progress bar string
	uint8_t goalProgressOutOfTen = ((float)steps / (float)stepGoal ) * 9;
	if (goalProgressOutOfTen >= 9) goalProgressOutOfTen = 9;
	char progressBarString[13] = "[         ]";
	for (uint8_t i=0 ; i<goalProgressOutOfTen ; i++ ) {
		progressBarString[i+1] = '=';
	}
	progressBarString[12] = '\0';

		ssd1306_SetCursor(16, 32);
		snprintf(buffer, sizeof(buffer), "%u / %u Steps", steps , stepGoal);
		ssd1306_WriteString(buffer, Font_6x8, White);

		ssd1306_SetCursor(3, 46);
		snprintf(buffer, sizeof(buffer), progressBarString);
		ssd1306_WriteString(buffer, Font_11x18, White);
}

void displayDistanceTravelled(void) {


	ssd1306_SetCursor(0, 0);
	snprintf(buffer, sizeof(buffer), "-------------------");
	ssd1306_WriteString(buffer, Font_7x10, White);

	ssd1306_SetCursor(0, 11);
	snprintf(buffer, sizeof(buffer), " Distance Traveled ");
	ssd1306_WriteString(buffer, Font_7x10, White);

	ssd1306_SetCursor(0, 21);
	snprintf(buffer, sizeof(buffer), "-------------------");
	ssd1306_WriteString(buffer, Font_7x10, White);

	ssd1306_SetCursor(52, 40);

	uint32_t distInCm = (uint32_t)steps * 80;
	if (distanceDisplayUnitsFlag) {
		uint32_t distWholeComponent = distInCm/100000;
		uint32_t distFractComponent = (distInCm % 100000)/1000;
		snprintf(buffer, sizeof(buffer), "%lu.%02lu km", distWholeComponent , distFractComponent);
		ssd1306_WriteString(buffer, Font_7x10, White);
	} else {
		uint32_t disInYards = distInCm * 1094 / 100000;
		snprintf(buffer, sizeof(buffer), "%lu yds", disInYards);
		ssd1306_WriteString(buffer, Font_7x10, White);
	}
}

void display_task_execute(void) {

	const currJoyStickState* joyStick = getCurrJoyStickState();

	ssd1306_Fill(Black);

	if (inSetGoalState) {
		ssd1306_SetCursor(0, 0);
		snprintf(buffer, sizeof(buffer), "-----Set Goal-----" );
		ssd1306_WriteString(buffer, Font_7x10, White);

		ssd1306_SetCursor(24, 28);
		snprintf(buffer, sizeof(buffer), "%u steps", newGoal);
		ssd1306_WriteString(buffer, Font_7x10, White);

		ssd1306_UpdateScreen();
		return;
	}

	switch(currDisplayState)
	{
	case CurrentSteps:
		displayCurrentSteps();
		break;
	case GoalProgress:
		displayGoalProgress();
		break;
	case DistanceTravelled:
		displayDistanceTravelled();
		break;
	}

	ssd1306_UpdateScreen();

    if (serialDebugFlag) {

    	snprintf(buffer, sizeof(buffer), "Joy X : %s %u", joyStick->xJoyDirection , joyStick->x);

    	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
    	HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 100);

    	snprintf(buffer, sizeof(buffer), "Joy Y : %s %u", joyStick->yJoyDirection , joyStick->y);
        HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
        HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 100);
    }

}
