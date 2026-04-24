
#ifndef task_button_h
#define task_button_h

//extern uint16_t steps;
//extern uint16_t distance;
//extern uint16_t stepGoal;


/*
 * task_button.h
 * Header file for the button task module
 * Authors: Ryan Teape, Felissa Chian
 * Date: 2026
 */

/* polls buttons and triggers necessary actions  */
void button_task_execute(void);

/* Initialises the PWM */
void initialisePWM(void);

void resetSW2PressesAfterDelay(void);

#endif
