/*
 * task_joystick.h
 * Header for joystick task module - reads joystick position and direction
 * Authors: Ryan Teape, Felissa Chian
 * Date: 07/05/2026
 */

#ifndef TASK_JOYSTICK_H
#define TASK_JOYSTICK_H

#include <stdint.h>

// shared ADC buffer for potentiometer & joystick X/Y readings via DMA
extern uint16_t raw_adc[3];

typedef enum {

	JOY_LEFT = 0,
	JOY_RIGHT,
	JOY_UP,
	JOY_DOWN,
	JOY_REST

} joyStickDirections;

typedef struct {

	uint16_t x; // raw ADC value for X axis
	uint16_t y; // raw ADC value for Y axis
	uint16_t percentageXdisplacement;
	uint16_t percentageYdisplacement;
	joyStickDirections xJoyDirection;
	joyStickDirections yJoyDirection;
	joyStickDirections previousJoyXDirection;
	joyStickDirections previousJoyYDirection;

} currJoyStickState;

// reads joystick ADC values and updates direction, display state, units, and goal setting
void joystick_task(void);

// returns a pointer to the current joystick state
const currJoyStickState* getCurrJoyStickState(void);

// increments or decrements steps based on joystick Y position, only active in test mode
void testModeJoyStickTask(void);

#endif /* TASK_JOYSTICK_H */
