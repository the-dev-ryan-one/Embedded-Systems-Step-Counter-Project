/*
 * task_stepcounter.h
 * Header for step counter task - handles step interrupts and goal completion alerts
 * Authors: Ryan Teape, Felissa Chian
 * Date: 14/04/2026
 */

#ifndef TASK_STEPCOUNTER_H
#define TASK_STEPCOUNTER_H

#include <stdint.h>

// Increments step count on rising edge interrupt from IMU INT1 pin
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin);

void checkGoalComplete(void);

#endif /* TASK_STEPCOUNTER_H */
