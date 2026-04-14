#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {

	int16_t x;
	int16_t y;
	int16_t z;

} AccelVec;

void initIMU(void);

int16_t fetchAndConcatXAcell(void);
