#include <stdint.h>

#ifndef task_joystick_h
#define task_joystick_h

extern uint16_t maxXValue;
extern uint16_t minXValue;
extern uint16_t maxYValue;
extern uint16_t minYValue;

extern uint16_t rawPotVal;

typedef struct {

	uint16_t x;
	uint16_t y;
	uint16_t percentageXdisplacement;
	uint16_t percentageYdisplacement;
	char* xJoyDirection;
	char* yJoyDirection;
	char* previousJoyXDirection;
	char* previousJoyYDirection;

} currJoyStickState;


static currJoyStickState joyStick;


/*
 * task_joystick.h
 * Header file for the joystick task module
 * Authors: Ryan Teape, Felissa Chian
 * Date: 2026
 */

/* Runs the joystick task , reads the ADC values from the joystick */
void joystick_task(void);

const currJoyStickState* getCurrJoyStickState(void);

void testModeJoyStickTask(void);

/* Returns the last read X axis value from the joystick */
uint16_t getJoyStickX (void);

/* Returns the last read Y axis value from the joystick */
uint16_t getJoyStickY (void);


#endif
