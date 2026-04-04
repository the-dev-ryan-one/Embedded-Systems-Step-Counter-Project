
/*
 * task_joystick.c
 * Joystick task module - reads X and Y axis ADC values using DMA
 * Authors: Ryan Teape, Felissa Chian
 * Date: 2026
 */


#include "adc.h"
#include "app.h"
#include "task_joystick.h"

static uint16_t raw_adc[2];
uint16_t percentageXdisplacement = 0;
uint16_t percentageYdisplacement = 0;

uint16_t maxXValue = 3850;
uint16_t minXValue = 610;
uint16_t maxYValue = 3900;
uint16_t minYValue = 706;


uint16_t currYVal;
uint16_t x;
uint16_t y;

char* xJoyDirection = "Rest";
char* yJoyDirection = "Rest";


#define maxStepIncrement 100
#define uint16MaxVal 65535


#define adcRestValX 2203
#define adcMaxValX 3835
#define adcMinValX 454

#define adcRestValY 2233
#define adcMaxValY 3900
#define adcMinValY 343

uint16_t getJoyStickX (void) {

	return raw_adc[1];
}

uint16_t getJoyStickY (void) {

	return raw_adc[0];
}

int16_t calcStepIncrement(uint16_t percentageYdisplacement) {
//		float stepIncrementCoEfficient = 0.5 - (percentageYdisplacement/ 100.0f);
//
//		int16_t toReturn = (int16_t)(maxStepIncrement*stepIncrementCoEfficient);
//
//		return toReturn;

		if (percentageYdisplacement >= 55) {
	        // 55→100 maps to 1→1000
	        return (int16_t)((percentageYdisplacement - 55.0) / (100.0 - 55.0) * (1000.0 - 1.0) + 1.0);
	    } else if (percentageYdisplacement <= 45) {
	        // 45→0 maps to -1→-1000
	        return (int16_t)((percentageYdisplacement - 45.0) / (0.0 - 45.0) * (-1000.0 - (-1.0)) + (-1.0));
	    }
	    return 0; // dead zone 46–54

}


void joystick_task(void)
{
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)raw_adc, 2);

	//-----------
	x = getJoyStickX();
	y = getJoyStickY();



	if (x > adcRestValX) {
			percentageXdisplacement = (x - adcRestValX)*100 / (adcMaxValX - adcRestValX);
		} else {
			percentageXdisplacement = (adcRestValX - x)*100 / (adcRestValX - adcMinValX);
		}

	if (percentageXdisplacement > 100) {
		percentageXdisplacement = 100;
	}


	if (y > adcRestValY) {
				percentageYdisplacement = (y - adcRestValY)*100 / (adcMaxValY - adcRestValY);
			} else {
				percentageYdisplacement = (adcRestValY - y)*100 / (adcRestValY - adcMinValY);
			}

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

	     if (percentageYdisplacement >= 55 || percentageYdisplacement <= 45) {

	    	 int16_t stepIncrement = calcStepIncrement(percentageYdisplacement);

	    	if (steps + stepIncrement < uint16MaxVal) {

	    	steps += stepIncrement;
	    	}

	     }

}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){



}
