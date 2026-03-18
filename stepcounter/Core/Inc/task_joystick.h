
#ifndef task_joystick_h
#define task_joystick_h

/*
 * task_joystick.h
 * Header file for the joystick task module
 * Authors: Ryan Teape, Felissa Chian
 * Date: 2026
 */

#include <stdint.h>

/* Runs the joystick task , reads the ADC values from the joystick */
void joystick_task(void);

/* Returns the last read X axis value from the joystick */
uint16_t getJoyStickX (void);

/* Returns the last read Y axis value from the joystick */
uint16_t getJoyStickY (void);


#endif
