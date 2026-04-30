/*
 * accelerometer.c
 * Configures the LSM6DS IMU sensor registers on startup
 * Authors: Ryan Teape, Felissa Chian
 * Date: 14/04/2026
 */
 
#include "accelerometer.h"
#include "imu_lsm6ds.h"

void initIMU(void)
{
	// Enable accelerometer in high-performance mode
	imu_lsm6ds_write_byte(CTRL1_XL, CTRL1_XL_HIGH_PERFORMANCE);
	
	// Enable the step counter feature
	imu_lsm6ds_write_byte(CTRL10_C , ENABLE_STEPCOUNTER);
	
    // Route step counter interrupt signal to INT1 pin
	imu_lsm6ds_write_byte(INT1_CTRL , ENABLE_INTERRUPT_ON_INT1);
	
    // Enable tap detection
	imu_lsm6ds_write_byte(TAP_CFG, ENABLE_TAP_CFG);
}
