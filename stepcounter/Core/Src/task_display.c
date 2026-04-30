/*
 * task_display.c
 * Renders different OLED screens based on application state,
 * including step count, goal progress, and distance travelled
 * Authors: Ryan Teape, Felissa Chian
 * Date: 13/03/2026
 */

#include <stdio.h>
#include <string.h>
#include "task_display.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "ssd1306_conf.h"
#include "app.h"

#define STEP_LENGTH_IN_CM 80
#define CM_PER_KM 100000
#define YARDS_PER_KM 1094

static char buffer[32];

static void drawScreenHeader(const char* text)
{
	ssd1306_SetCursor(0, 0);
	ssd1306_WriteString("-------------------", Font_7x10, White);

	ssd1306_SetCursor(0, 11);
	snprintf(buffer, sizeof(buffer), "%s", text);
	ssd1306_WriteString(buffer, Font_7x10, White);

	ssd1306_SetCursor(0, 21);
	ssd1306_WriteString("-------------------", Font_7x10, White);

}

static void drawGoalCompleteScreen(void)
{
	ssd1306_SetCursor(10, 16);
	ssd1306_WriteString("******************", Font_6x8, White);
	ssd1306_SetCursor(10, 26);
	ssd1306_WriteString("* Goal Complete! *", Font_6x8, White);
	ssd1306_SetCursor(10, 36);
	ssd1306_WriteString("******************", Font_6x8, White);

}

static void displayCurrentSteps(void)
{

	drawScreenHeader("  Current Steps  ");
	ssd1306_SetCursor(52, 40);

	// display steps as raw count or percentage of goal based on unit flag
	if (stepDisplayUnitsFlag)
	{
		snprintf(buffer, sizeof(buffer), "%u steps", steps);
		ssd1306_WriteString(buffer, Font_7x10, White);
	}
	else
	{
		uint8_t goalPercentage = (uint32_t)steps * 100 / stepGoal;
		snprintf(buffer, sizeof(buffer), "%u %%" , goalPercentage);
		ssd1306_WriteString(buffer, Font_7x10, White);
	}

}

static void drawSetGoalState(void)
{
	ssd1306_SetCursor(0, 0);
	ssd1306_WriteString("-----Set Goal-----", Font_7x10, White);

	ssd1306_SetCursor(24, 28);
	snprintf(buffer, sizeof(buffer), "%u steps", newGoal);
	ssd1306_WriteString(buffer, Font_7x10, White);

}


static void displayGoalProgress(void)
{

	drawScreenHeader("  Goal Progress  ");
	ssd1306_SetCursor(52, 40);

	// scale progress to 0-9 for the progress bar
	uint8_t goalProgressOutOfTen = ((float)steps / (float)stepGoal ) * 9;
	if (goalProgressOutOfTen >= 9)
	{
		goalProgressOutOfTen = 9;
	}

	// build progress bar string by filling '=' characters from left
	char progressBarString[13] = "[         ]";
	for (uint8_t i=0 ; i<goalProgressOutOfTen ; i++ )
	{
		progressBarString[i+1] = '=';
	}
	progressBarString[12] = '\0';

	ssd1306_SetCursor(16, 32);
	snprintf(buffer, sizeof(buffer), "%u / %u Steps", steps , stepGoal);
	ssd1306_WriteString(buffer, Font_6x8, White);

	ssd1306_SetCursor(3, 46);
	snprintf(buffer, sizeof(buffer),  "%s", progressBarString);
	ssd1306_WriteString(buffer, Font_11x18, White);

}

static void displayDistanceTravelled(void)
{

	drawScreenHeader(" Distance Traveled ");

	ssd1306_SetCursor(52, 40);
	uint32_t distInCm = (uint32_t)steps * STEP_LENGTH_IN_CM;

	// display distance in km or yards based on unit flag
	if (distanceDisplayUnitsFlag)
	{
		uint32_t distWholeComponent = distInCm/CM_PER_KM;
		uint32_t distFractComponent = (distInCm % CM_PER_KM)/1000;
		snprintf(buffer, sizeof(buffer), "%lu.%02lu km", distWholeComponent , distFractComponent);
		ssd1306_WriteString(buffer, Font_7x10, White);
	}
	else
	{
		uint32_t disInYards = distInCm * YARDS_PER_KM / CM_PER_KM;
		snprintf(buffer, sizeof(buffer), "%lu yds", disInYards);
		ssd1306_WriteString(buffer, Font_7x10, White);
	}

}

void display_task_execute(void) {

	ssd1306_Fill(Black);

	// priority screens: goal complete and set goal override normal display
	if (goalCompleteFlag)
	{
		drawGoalCompleteScreen();
		ssd1306_UpdateScreen();
		return;
	}

	if (inSetGoalState)
	{
		drawSetGoalState();
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

}
