
#include "adc.h"

#include "task_joystick.h"


static uint16_t raw_adc[2];


void joystick_task(void)
{
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)raw_adc, 2);

}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){



}
