
/*
 * task_joystick.c
 * Joystick task module - reads X and Y axis ADC values using DMA
 * Authors: Ryan Teape, Felissa Chian
 * Date: 2026
 */


#include "adc.h"
#include "task_joystick.h"


static uint16_t raw_adc[2];

uint16_t getJoyStickX (void) {

	return raw_adc[1];
}

uint16_t getJoyStickY (void) {

	return raw_adc[0];
}


void joystick_task(void)
{
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)raw_adc, 2);

}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){



}
