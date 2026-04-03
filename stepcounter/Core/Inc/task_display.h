#ifndef TASK_DISPLAY_H
#define TASK_DISPLAY_H

extern uint16_t steps;
extern uint16_t distance;
extern uint16_t stepGoal;


/*
 * task_display.h
 * Header file for the display task module
 * Authors: Ryan Teape, Felissa Chian
 * Date: 2026
 */

/* updates OLED display */
void display_task_execute(void);

#endif
