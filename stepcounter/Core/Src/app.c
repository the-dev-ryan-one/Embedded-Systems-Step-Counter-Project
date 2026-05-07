/*
 * app.c
 * Main application module - initialises peripherals and runs the task scheduler
 * Authors: Ryan Teape, Felissa Chian
 * Date: 26/02/2026
 */

#include "app.h"
#include "buttons.h"
#include "task_LED.h"
#include "task_joystick.h"
#include "task_button.h"
#include "task_display.h"
#include "ssd1306.h"
#include "pwm.h"
#include "task_stepcounter.h"
#include "potentiometer.h"
#include "accelerometer.h"

#define TICK_FREQUENCY_HZ 1000
#define HZ_TO_TICKS(FREQUENCY_HZ) (TICK_FREQUENCY_HZ/FREQUENCY_HZ)

#define BLINKY_FREQUENCY_HZ 2
#define BUTTON_FREQUENCY_HZ 56
#define JOYSTICK_FREQUENCY_HZ 56
#define DISPLAY_FREQUENCY_HZ 4
#define POTENTIOMETER_FREQUENCY_HZ 8
#define TESTMODE_FREQUENCY_HZ 6
#define CHECKGOALCOMPLETE_FREQUENCY_HZ 4
#define PROGRESSLEDS_FREQUENCY_HZ 4

#define BLINKY_PERIOD_TICKS        HZ_TO_TICKS(BLINKY_FREQUENCY_HZ)
#define BUTTON_PERIOD_TICKS        HZ_TO_TICKS(BUTTON_FREQUENCY_HZ)
#define JOYSTICK_PERIOD_TICKS      HZ_TO_TICKS(JOYSTICK_FREQUENCY_HZ)
#define DISPLAY_PERIOD_TICKS       HZ_TO_TICKS(DISPLAY_FREQUENCY_HZ)
#define POTENTIOMETER_PERIOD_TICKS HZ_TO_TICKS(POTENTIOMETER_FREQUENCY_HZ)
#define TESTMODE_PERIOD_TICKS      HZ_TO_TICKS(TESTMODE_FREQUENCY_HZ)
#define CHECKGOALCOMPLETE_PERIOD_TICKS HZ_TO_TICKS(CHECKGOALCOMPLETE_FREQUENCY_HZ)
#define PROGRESSLEDS_PERIOD_TICKS  HZ_TO_TICKS(PROGRESSLEDS_FREQUENCY_HZ)

static uint32_t BlinkyNextRun = 0;
static uint32_t ButtonNextRun = 0;
static uint32_t JoystickNextRun = 0;
static uint32_t DisplayNextRun = 0;
static uint32_t PotentiometerNextRun = 0;
static uint32_t TestModeNextRun = 0;

static uint32_t CheckGoalCompleteNextRun = 0;
static uint32_t ProgressLEDsNextRun = 0;

bool inSetGoalState = false;
bool serialDebugFlag = false;
bool testStateFlag = false;
bool goalCompleteFlag = false;
volatile bool clearInteruptFlag = false;

// default values on startup
uint16_t stepGoal = 1000;
volatile uint16_t steps = 0;
uint16_t newGoal = 500;

displayState currDisplayState = CurrentSteps;

bool distanceDisplayUnitsFlag = false;
bool stepDisplayUnitsFlag = false;

void app_main(void) {

	// stagger initial task run times to avoid all tasks firing at once
	BlinkyNextRun = HAL_GetTick() + BLINKY_PERIOD_TICKS;
	ButtonNextRun = HAL_GetTick() + BUTTON_PERIOD_TICKS;
	JoystickNextRun = HAL_GetTick() + JOYSTICK_PERIOD_TICKS;
	DisplayNextRun = HAL_GetTick() + DISPLAY_PERIOD_TICKS;
	PotentiometerNextRun = HAL_GetTick() + POTENTIOMETER_PERIOD_TICKS;
	TestModeNextRun = HAL_GetTick() + TESTMODE_PERIOD_TICKS;
	CheckGoalCompleteNextRun = HAL_GetTick() + CHECKGOALCOMPLETE_PERIOD_TICKS;
	ProgressLEDsNextRun = HAL_GetTick() + PROGRESSLEDS_PERIOD_TICKS;

	buttons_init();
	ssd1306_Init();
	initialisePWM();
	initIMU();

	// run each task at its scheduled frequency using tick-based scheduling
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

    	  if (ticks > TestModeNextRun)
    	  {
    		  testModeJoyStickTask();
    	      TestModeNextRun += TESTMODE_PERIOD_TICKS;
    	  }

    	  if (ticks > CheckGoalCompleteNextRun)
    	  {
    		  checkGoalComplete();
    	      CheckGoalCompleteNextRun += CHECKGOALCOMPLETE_PERIOD_TICKS;

    	  }

    	  if (ticks > ProgressLEDsNextRun)
    	  {
    	      updateProgressLEDs();
    	      ProgressLEDsNextRun += PROGRESSLEDS_PERIOD_TICKS;
    	  }

    	}

}
