#include "accelerometer.h"
#include "imu_lsm6ds.h"

void initIMU(void)
{
	imu_lsm6ds_write_byte(CTRL1_XL, CTRL1_XL_HIGH_PERFORMANCE);
	imu_lsm6ds_write_byte(CTRL10_C , ENABLE_STEPCOUNTER);
	imu_lsm6ds_write_byte(INT1_CTRL , ENABLE_INTERRUPT_ON_INT1);
	imu_lsm6ds_write_byte(TAP_CFG, ENABLE_TAP_CFG);
}
