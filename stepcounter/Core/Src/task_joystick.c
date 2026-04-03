
/*
 * task_joystick.c
 * Joystick task module - reads X and Y axis ADC values using DMA
 * Authors: Ryan Teape, Felissa Chian
 * Date: 2026
 */


#include "adc.h"
#include "task_joystick.h"

static uint16_t raw_adc[2];
uint16_t percentageXdisplacement = 0;
uint16_t percentageYdisplacement = 0;
uint16_t maxXValue = 3850;
uint16_t minXValue = 470;
uint16_t maxYValue = 3900;
uint16_t minYValue = 335;
uint16_t y;
uint16_t x;

char* xJoyDirection = "Rest";
char* yJoyDirection = "Rest";



uint16_t getJoyStickX (void) {

	return raw_adc[1];
}

uint16_t getJoyStickY (void) {

	return raw_adc[0];
}


void joystick_task(void)
{
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)raw_adc, 2);

	//-----------
	y = getJoyStickY();
	x = getJoyStickX();

	percentageXdisplacement = ((x - minXValue) * 100) / (maxXValue - minXValue);
	if (percentageXdisplacement > 100) {
		percentageXdisplacement = 100;
	}


	    if (percentageXdisplacement > 60) {
	    	xJoyDirection = "Left";
	    } else if (percentageXdisplacement < 40) {
	    	xJoyDirection = "Right";
	    } else {
	    	xJoyDirection = "Rest";
	    }

	    y = getJoyStickY();

	     percentageYdisplacement = ((y - minYValue) * 100) / (maxYValue - minYValue);
	     if (percentageYdisplacement > 100) {
	         percentageYdisplacement = 100;
	     }

	     if (percentageYdisplacement > 60) {
	         yJoyDirection = "Down";
	     } else if (percentageYdisplacement < 40) {
	         yJoyDirection = "Up";
	     } else {
	         yJoyDirection = "Rest";
	     }

}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){



}
