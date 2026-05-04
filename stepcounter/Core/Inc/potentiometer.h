/*
 * potentiometer.h
 * Header for potentiometer task that maps ADC readings to a step goal value
 * Authors: Ryan Teape, Felissa Chian
 * Date: 28/04/2026
 */

#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

// reads the potentiometer ADC value and updates the current step goal
void potentiometer_task(void);

#endif /* POTENTIOMETER_H */
