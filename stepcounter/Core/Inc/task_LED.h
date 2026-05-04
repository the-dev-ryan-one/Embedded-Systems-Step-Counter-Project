/*
 * task_LED.h
 * Header file for the LED task module
 * Authors: Ryan Teape, Felissa Chian
 * Date: 12/03/2026
 */

#ifndef TASK_LED_H
#define TASK_LED_H

void blinky_task_execute(void);

// updates DS3 PWM brightness and RGB LEDs based on step goal completion percentage
void updateProgressLEDs(void);

#endif /* TASK_LED_H */
