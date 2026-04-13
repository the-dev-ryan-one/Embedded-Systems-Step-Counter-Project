#include "accelerometer.h"
#include "imu_lsm6ds.h"
#include "usart.h"


void initIMU(void) {

	imu_lsm6ds_write_byte(CTRL1_XL, CTRL1_XL_HIGH_PERFORMANCE);

}

int16_t fetchAndConcatXAcell(void) {

	uint8_t acc_x_low = imu_lsm6ds_read_byte(OUTX_L_XL);
	uint8_t acc_x_high = imu_lsm6ds_read_byte(OUTX_H_XL);
	int16_t xAccelVal = (int16_t)(acc_x_high << 8 | acc_x_low);

	char buffer[32];
	snprintf(buffer, sizeof(buffer), "X accel val : %d", xAccelVal );

	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
	HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 100);

	return xAccelVal;
}
