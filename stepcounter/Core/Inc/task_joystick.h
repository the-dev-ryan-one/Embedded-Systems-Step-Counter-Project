#ifndef TASK_JOYSTICK_H
#define TASK_JOYSTICK_H

#include <stdint.h>

extern uint16_t rawPotVal;
extern uint16_t raw_adc[3];

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

void joystick_task(void);
const currJoyStickState* getCurrJoyStickState(void);
void testModeJoyStickTask(void);

#endif /* TASK_JOYSTICK_H */
