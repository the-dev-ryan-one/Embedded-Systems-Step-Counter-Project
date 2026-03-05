#include "app.h"
#include "gpio.h"
#include "buttons.h"
#include "adc.h"

#define TICK_FREQUENCY_HZ 1000
#define HZ_TO_TICKS(FREQUENCY_HZ) (TICK_FREQUENCY_HZ/FREQUENCY_HZ)

#define BLINKY_FREQUENCY_HZ 2
#define BUTTON_FREQUENCY_HZ 56

#define BLINKY_PERIOD_TICKS (TICK_FREQUENCY_HZ/BLINKY_FREQUENCY_HZ)
#define BUTTON_PERIOD_TICKS (TICK_FREQUENCY_HZ/BUTTON_FREQUENCY_HZ)

static uint32_t BlinkyNextRun = 0;
static uint32_t ButtonNextRun = 0;

static uint16_t raw_adc[2];

void blinky_task_execute(void);
void button_task_execute(void);


void app_main(void) {

	BlinkyNextRun = HAL_GetTick() + BLINKY_PERIOD_TICKS;
	ButtonNextRun = HAL_GetTick() + BUTTON_PERIOD_TICKS;

	buttons_init();

    while(1)
    	{
    	  uint32_t ticks = HAL_GetTick();

    	  if(ticks > BlinkyNextRun)
    	  {
    		blinky_task_execute();
    		BlinkyNextRun += BLINKY_PERIOD_TICKS;
    	  }

    	  if (ticks > ButtonNextRun)
    	  {
    		buttons_update();
    		button_task_execute();
    		HAL_ADC_Start_DMA(&hadc1, (uint32_t*)raw_adc, 2);
    		ButtonNextRun += BUTTON_PERIOD_TICKS;
    	  }
    	}



}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){

}


void blinky_task_execute(void)
{
	// Task one logic
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

}

void button_task_execute(void)
{
	// Task two logic

	if (buttons_checkButton (LEFT) == PUSHED) {

	        rgb_colour_all_on();
	        rgb_led_on(RGB_LEFT);
	    } else {
	        rgb_led_off(RGB_LEFT);
	    }

	    if (buttons_checkButton (UP) == PUSHED) {

	    	rgb_colour_all_on();

	        rgb_led_on(RGB_UP);
	    } else {
	        rgb_led_off(RGB_UP);
	    }

	    if (buttons_checkButton (DOWN) == PUSHED) {

	    	rgb_colour_all_on();
	        rgb_led_on(RGB_DOWN);
	    } else {
	        rgb_led_off(RGB_DOWN);
	    }

	    if (buttons_checkButton (RIGHT) == PUSHED) {

	    	rgb_colour_all_on();
	        rgb_led_on(RGB_RIGHT);
	    } else {
	        rgb_led_off(RGB_RIGHT);
	    }

}
