
#include "app.h"
#include "gpio.h"
#include "buttons.h"
#include "adc.h"

#include "task_joystick.h"
#include "task_blinky.h"
#include "task_button.h"

#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "ssd1306_conf.h"


#define TICK_FREQUENCY_HZ 1000
#define HZ_TO_TICKS(FREQUENCY_HZ) (TICK_FREQUENCY_HZ/FREQUENCY_HZ)

#define BLINKY_FREQUENCY_HZ 2
#define BUTTON_FREQUENCY_HZ 56
#define JOYSTICK_FREQUENCY_HZ 56
#define DISPLAY_FREQUENCY_HZ 4

#define BLINKY_PERIOD_TICKS (TICK_FREQUENCY_HZ/BLINKY_FREQUENCY_HZ)
#define BUTTON_PERIOD_TICKS (TICK_FREQUENCY_HZ/BUTTON_FREQUENCY_HZ)
#define JOYSTICK_PERIOD_TICKS (TICK_FREQUENCY_HZ/JOYSTICK_FREQUENCY_HZ)
#define DISPLAY_PERIOD_TICKS (TICK_FREQUENCY_HZ/DISPLAY_FREQUENCY_HZ)

static uint32_t BlinkyNextRun = 0;
static uint32_t ButtonNextRun = 0;
static uint32_t JoystickNextRun = 0;
static uint32_t DisplayNextRun = 0;



void display_task_execute(void) {

 ssd1306_UpdateScreen();

}






void app_main(void) {

	BlinkyNextRun = HAL_GetTick() + BLINKY_PERIOD_TICKS;
	ButtonNextRun = HAL_GetTick() + BUTTON_PERIOD_TICKS;
	JoystickNextRun = HAL_GetTick() + JOYSTICK_PERIOD_TICKS;
	DisplayNextRun = HAL_GetTick() + DISPLAY_PERIOD_TICKS;

	buttons_init();

	ssd1306_Init();
	ssd1306_SetCursor(0, 0);
	ssd1306_WriteString("Hello world!", Font_7x10, White);


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


    	}



}



