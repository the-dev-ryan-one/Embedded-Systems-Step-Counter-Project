/*
 * filter.h
 * Moving average filter for accelerometer using a 16-sample circular buffer
 * Authors: Ryan Teape, Felissa Chian
 * Date: 14/04/2026
 */

#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>

typedef struct {
    int16_t vals[16]; // circular buffer of 16 samples
    uint8_t writeIndex;
} FilterAvg;

// initialises the filter buffer to zero
void initFilter(FilterAvg* pointer);

// adds a new reading to the filter and returns the updated average
int16_t updateFilter(FilterAvg* p, int16_t newReading);

#endif /* FILTER_H */