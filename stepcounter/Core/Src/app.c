/*
 * app.c
 * Main application module - initialises peripherals and runs the task scheduler
 * Authors: Ryan Teape, Felissa Chian
 * Date: 2026
 */

#include "app.h"
#include "gpio.h"
#include "buttons.h"
#include "adc.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "task_joystick.h"
#include "task_blinky.h"
#include "task_button.h"
#include "task_display.h"

#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "ssd1306_conf.h"
#include "usart.h"
#include "tim.h"
#include "pwm.h"


#define TICK_FREQUENCY_HZ 1000
#define HZ_TO_TICKS(FREQUENCY_HZ) (TICK_FREQUENCY_HZ/FREQUENCY_HZ)

#define BLINKY_FREQUENCY_HZ 2
#define BUTTON_FREQUENCY_HZ 56
#define JOYSTICK_FREQUENCY_HZ 56
#define DISPLAY_FREQUENCY_HZ 4
#define SW2RESET_FREQUENCY_HZ 1
#define POTENTIOMETER_FREQUENCY_HZ 30

#define BLINKY_PERIOD_TICKS       HZ_TO_TICKS(BLINKY_FREQUENCY_HZ)
#define BUTTON_PERIOD_TICKS       HZ_TO_TICKS(BUTTON_FREQUENCY_HZ)
#define JOYSTICK_PERIOD_TICKS     HZ_TO_TICKS(JOYSTICK_FREQUENCY_HZ)
#define DISPLAY_PERIOD_TICKS      HZ_TO_TICKS(DISPLAY_FREQUENCY_HZ)
#define SW2RESET_PERIOD_TICKS     HZ_TO_TICKS(SW2RESET_FREQUENCY_HZ)
#define POTENTIOMETER_PERIOD_TICKS HZ_TO_TICKS(POTENTIOMETER_FREQUENCY_HZ)

//#define BLINKY_PERIOD_TICKS (TICK_FREQUENCY_HZ/BLINKY_FREQUENCY_HZ)
//#define BUTTON_PERIOD_TICKS (TICK_FREQUENCY_HZ/BUTTON_FREQUENCY_HZ)
//#define JOYSTICK_PERIOD_TICKS (TICK_FREQUENCY_HZ/JOYSTICK_FREQUENCY_HZ)
//#define DISPLAY_PERIOD_TICKS (TICK_FREQUENCY_HZ/DISPLAY_FREQUENCY_HZ)
//#define SW2RESET_PERIOD_TICKS (TICK_FREQUENCY_HZ/SW2RESET_FREQUENCY_HZ)
//#define POTENTIOMETER_PERIOD_TICKS (TICK_FREQUENCY_HZ/POTENTIOMETER_FREQUENCY_HZ)

static uint32_t BlinkyNextRun = 0;
static uint32_t ButtonNextRun = 0;
static uint32_t JoystickNextRun = 0;
static uint32_t DisplayNextRun = 0;
static uint32_t PotentiometerNextRun = 0;
//static uint32_t SW2PressesResetNextRun = 0;

bool serialDebugFlag = false;
bool testStateFlag = false;

// reset Values
uint16_t stepGoal = 1000;
uint16_t steps = 0;

displayState currDisplayState = CurrentSteps;

bool distanceDisplayUnitsFlag = false;
bool stepDisplayUnitsFlag = false;

void app_main(void) {

	BlinkyNextRun = HAL_GetTick() + BLINKY_PERIOD_TICKS;
	ButtonNextRun = HAL_GetTick() + BUTTON_PERIOD_TICKS;
	JoystickNextRun = HAL_GetTick() + JOYSTICK_PERIOD_TICKS;
	DisplayNextRun = HAL_GetTick() + DISPLAY_PERIOD_TICKS;
	PotentiometerNextRun = HAL_GetTick() + POTENTIOMETER_PERIOD_TICKS;

	buttons_init();
	ssd1306_Init();
	initialisePWM();

    while(1)
    	{
    	  uint32_t ticks = HAL_GetTick();

    	  if(ticks > DisplayNextRun)
		  {
    		 display_task_execute();
    		 DisplayNextRun += DISPLAY_PERIOD_TICKS;
		  }

    	  if(ticks > BlinkyNextRun)
    	  {
    		blinky_task_execute();
    		BlinkyNextRun += BLINKY_PERIOD_TICKS;
    	  }

    	  if (ticks > ButtonNextRun)
    	  {
    		buttons_update();
    		button_task_execute();
    		ButtonNextRun += BUTTON_PERIOD_TICKS;
    	  }

    	  if (ticks > JoystickNextRun)
			  {
				joystick_task();
				JoystickNextRun += JOYSTICK_PERIOD_TICKS;
			  }
    	  if (ticks > PotentiometerNextRun)
			  {
    		  potentiometer_task();
    		  PotentiometerNextRun += POTENTIOMETER_PERIOD_TICKS;
			  }

    	}

}



