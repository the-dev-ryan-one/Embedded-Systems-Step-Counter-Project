#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {

	int16_t vals[16];
	uint8_t currIndex;

} FilterAvg;

void initFilter(FilterAvg* pointer);

int16_t updatefilter(FilterAvg* p , int16_t newReading);
