#include <stdint.h>

#ifndef task_joystick_h
#define task_joystick_h

extern uint16_t maxXValue;
extern uint16_t minXValue;
extern uint16_t maxYValue;
extern uint16_t minYValue;

extern uint16_t rawPotVal;


typedef enum {

	JOY_LEFT = 0,
	JOY_RIGHT,
	JOY_UP,
	JOY_DOWN,
	JOY_REST

} joyStickDirections;

typedef struct {

	uint16_t x;
	uint16_t y;
	uint16_t percentageXdisplacement;
	uint16_t percentageYdisplacement;
	joyStickDirections xJoyDirection;
	joyStickDirections yJoyDirection;
	joyStickDirections previousJoyXDirection;
	joyStickDirections previousJoyYDirection;

} currJoyStickState;

static currJoyStickState joyStick;


/*
 * task_joystick.h
 * Header file for the joystick task module
 * Authors: Ryan Teape, Felissa Chian
 * Date: 2026
 */

void checkGoalComplete(void);

/* Runs the joystick task , reads the ADC values from the joystick */
void joystick_task(void);

const currJoyStickState* getCurrJoyStickState(void);

void testModeJoyStickTask(void);

/* Returns the last read X axis value from the joystick */
uint16_t getJoyStickX (void);

/* Returns the last read Y axis value from the joystick */
uint16_t getJoyStickY (void);


#endif
