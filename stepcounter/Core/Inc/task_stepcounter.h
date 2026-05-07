/*
 * task_stepcounter.h
 * Header for step counter task - handles step interrupts and goal completion alerts
 * Authors: Ryan Teape, Felissa Chian
 * Date: 14/04/2026
 */

#ifndef TASK_STEPCOUNTER_H
#define TASK_STEPCOUNTER_H

#include <stdint.h>

// notify user if goal is complete
void checkGoalComplete(void);

#endif /* TASK_STEPCOUNTER_H */
