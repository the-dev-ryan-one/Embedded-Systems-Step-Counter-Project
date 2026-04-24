#include "accelerometer.h"
#include "imu_lsm6ds.h"
#include "usart.h"
#include "filter.h"

static FilterAvg xFilter;
static FilterAvg yFilter;
static FilterAvg zFilter;


AccelVec currAccelVec = {0,0,0};

void initIMU(void) {

	imu_lsm6ds_write_byte(CTRL1_XL, CTRL1_XL_HIGH_PERFORMANCE);

	initFilter(&xFilter);
	initFilter(&yFilter);
	initFilter(&zFilter);

//	imu_lsm6ds_write_byte(CTRL10_C , ENABLE_STEPCOUNTER);
//	imu_lsm6ds_write_byte(MD1_CFG, ENABLE_STEPDETECT_INTERRUPT);
//	imu_lsm6ds_write_byte(TAP_CFG, ENABLE_TAP_CFG);

	imu_lsm6ds_write_byte(CTRL10_C , ENABLE_STEPCOUNTER);

}

void updateAccelVec(void) {

	uint8_t acc_x_low = imu_lsm6ds_read_byte(OUTX_L_XL);
	uint8_t acc_x_high = imu_lsm6ds_read_byte(OUTX_H_XL);
	int16_t xAccelVal = (int16_t)(acc_x_high << 8 | acc_x_low);

	currAccelVec.x = updatefilter(&xFilter, xAccelVal);

	uint8_t acc_y_low = imu_lsm6ds_read_byte(OUTY_L_XL);
	uint8_t acc_y_high = imu_lsm6ds_read_byte(OUTY_H_XL);
	int16_t yAccelVal = (int16_t)(acc_y_high << 8 | acc_y_low);
	currAccelVec.y = updatefilter(&yFilter, yAccelVal);

	uint8_t acc_z_low = imu_lsm6ds_read_byte(OUTZ_L_XL);
	uint8_t acc_z_high = imu_lsm6ds_read_byte(OUTZ_H_XL);
	int16_t zAccelVal = (int16_t)(acc_z_high << 8 | acc_z_low);
	currAccelVec.z = updatefilter(&zFilter, zAccelVal);

//	char buffer[32];
//	snprintf(buffer, sizeof(buffer), "IMUVec : %d , %d, %d", xAccelVal, yAccelVal ,zAccelVal );
//
//	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
//	HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 100);

}

uint16_t getStepCount(void) {

	uint8_t lowByte = imu_lsm6ds_read_byte(STEP_COUNTER_L);
	uint8_t highByte = imu_lsm6ds_read_byte(STEP_COUNTER_H);

	uint16_t currStepCount = (uint16_t)(highByte << 8 | lowByte);

	char buffer[32];
	snprintf(buffer, sizeof(buffer), "curr step: %d" , currStepCount );

	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
	HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 100);

	return currStepCount;

}
