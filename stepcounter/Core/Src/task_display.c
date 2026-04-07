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

	ssd1306_SetCursor(0, 6);
	snprintf(buffer, sizeof(buffer), "Current Steps");
	ssd1306_WriteString(buffer, Font_7x10, White);

	ssd1306_SetCursor(0, 22);

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

	ssd1306_SetCursor(0, 6);
	snprintf(buffer, sizeof(buffer), "Goal Progress");
	ssd1306_WriteString(buffer, Font_7x10, White);

	ssd1306_SetCursor(0, 22);

	if (stepDisplayUnitsFlag) {
		snprintf(buffer, sizeof(buffer), "%u steps", steps);
		ssd1306_WriteString(buffer, Font_7x10, White);
	} else {
		uint8_t goalPercentage = (uint32_t)steps * 100 / stepGoal;
		snprintf(buffer, sizeof(buffer), "%u %%" , goalPercentage);
		ssd1306_WriteString(buffer, Font_7x10, White);
	}

}

void displayDistanceTravelled(void) {

	ssd1306_SetCursor(0, 6);
	snprintf(buffer, sizeof(buffer), "Distance Traveled");
	ssd1306_WriteString(buffer, Font_7x10, White);

	ssd1306_SetCursor(24, 22);
	uint32_t distInCm = (uint32_t)steps * 80;
	if (distanceDisplayUnitsFlag) {
		uint32_t distWholeComponent = distInCm/100000;
		uint32_t distFractComponent = (distInCm % 100000)/1000;
		snprintf(buffer, sizeof(buffer), "%lu.%03lu km", distWholeComponent , distFractComponent);
		ssd1306_WriteString(buffer, Font_7x10, White);
	} else {
		uint32_t disInYards = distInCm * 1094 / 100000;
		snprintf(buffer, sizeof(buffer), "%lu yds", disInYards);
		ssd1306_WriteString(buffer, Font_7x10, White);
	}
}

void display_task_execute(void) {

	ssd1306_Fill(Black);

	if (inSetGoalState) {
		ssd1306_SetCursor(0, 0);
		snprintf(buffer, sizeof(buffer), "-----Set Goal-----" );
		ssd1306_WriteString(buffer, Font_7x10, White);

		ssd1306_SetCursor(0, 16);
		snprintf(buffer, sizeof(buffer), "%u", newGoal);
		ssd1306_WriteString(buffer, Font_7x10, White);

		ssd1306_UpdateScreen();
		return;
	}

//	if(testStateFlag) {
//			ssd1306_SetCursor(0, 0);
//			snprintf(buffer, sizeof(buffer), "-----Test Mode-----" );
//			ssd1306_WriteString(buffer, Font_7x10, White);
//
//			ssd1306_SetCursor(0, 26);
//			snprintf(buffer, sizeof(buffer), "   [ steps: %u ]  " , steps );
//			ssd1306_WriteString(buffer, Font_7x10, White);
//
//			ssd1306_UpdateScreen();
//			return;
//
//	}

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

    	snprintf(buffer, sizeof(buffer), "Joy X : %s %u", xJoyDirection , x);

    	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
    	HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 100);

    	snprintf(buffer, sizeof(buffer), "Joy Y : %s %u", yJoyDirection , y);
        HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
        HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 100);
    }

}
