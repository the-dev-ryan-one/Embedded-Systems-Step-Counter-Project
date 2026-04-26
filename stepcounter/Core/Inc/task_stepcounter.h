/*
 * task_stepcounter.h
 *
 *  Created on: 14/04/2026
 *      Author: Uni
 */

#include <stdbool.h>
#include <stdint.h>

#ifndef TASK_STEPCOUNTER_H
#define TASK_STEPCOUNTER_H

// in task_stepcounter.h
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin);

void checkGoalComplete(void);

#endif /* TASK_STEPCOUNTER_H */
