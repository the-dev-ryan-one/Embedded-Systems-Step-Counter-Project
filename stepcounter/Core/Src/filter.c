
#include "filter.h"

void initFilter(FilterAvg* pointer) {

	for (int i=0 ; i<16 ; i++) {
		pointer->vals[i] = 0;
	}
	pointer->currIndex = 0;

}

int16_t updatefilter(FilterAvg* p , int16_t newReading) {

	p->vals[p->currIndex] = newReading;

	int32_t sum = 0;

	for (int i=0 ; i<16 ; i++) {
		sum += p->vals[i];
	}

	int16_t average = (int16_t)(sum/16);

	p->currIndex +=1;
	if (p->currIndex >= 16) p->currIndex = 0;

	return average;

	}

