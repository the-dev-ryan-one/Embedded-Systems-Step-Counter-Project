
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

#include "usart.h"

#include "tim.h"
#include "pwm.h"


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

#include <stdio.h>
#include <string.h>

bool serialDebugFlag = false;


void display_task_execute(void) {

    char buffer[32];

    uint16_t x = getJoyStickX();

    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    snprintf(buffer, sizeof(buffer), "Joy X value: %u", x);
    ssd1306_WriteString(buffer, Font_7x10, White);

    if (serialDebugFlag) {

    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
    HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 100);

    }

    uint16_t y = getJoyStickY();

    ssd1306_SetCursor(0, 16);
    snprintf(buffer, sizeof(buffer), "Joy Y value: %u", y);

    ssd1306_WriteString(buffer, Font_7x10, White);
    ssd1306_UpdateScreen();

    if (serialDebugFlag) {

    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
    HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 100);

    }

}


void PWM_Init(void) {
    pwm_setDutyCycle(&htim2, TIM_CHANNEL_3, 25);
}

uint8_t dutyCycle = 25; // initial 25%

void SW1PressEvent(void) {

	dutyCycle += 10;

    if (dutyCycle > 100)  // wrap around
        dutyCycle = 0;

    pwm_setDutyCycle(&htim2, TIM_CHANNEL_3, dutyCycle);
}




void app_main(void) {

	BlinkyNextRun = HAL_GetTick() + BLINKY_PERIOD_TICKS;
	ButtonNextRun = HAL_GetTick() + BUTTON_PERIOD_TICKS;
	JoystickNextRun = HAL_GetTick() + JOYSTICK_PERIOD_TICKS;
	DisplayNextRun = HAL_GetTick() + DISPLAY_PERIOD_TICKS;

	buttons_init();

	ssd1306_Init();

	PWM_Init();


	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);




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



