/*
 * potentiometer.h
 * Header for potentiometer task that maps ADC readings to a step goal value
 * Authors: Ryan Teape, Felissa Chian
 * Date: 28/04/2026
 */

#ifndef INC_POTENTIOMETER_H_
#define INC_POTENTIOMETER_H_

// Reads the potentiometer ADC value and updates the current step goal
void potentiometer_task(void);

#endif /* INC_POTENTIOMETER_H_ */
