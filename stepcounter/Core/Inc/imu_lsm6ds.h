/*
 * imu_lsm6ds.h
 *
 *  Created on: Nov 27, 2024
 *      Author: fsy13
 */

#ifndef INC_IMU_LSM6DS_H_
#define INC_IMU_LSM6DS_H_

#include <stdint.h>

typedef enum {
	CTRL1_XL = 0x10,
	OUTX_L_XL = 0x28,
	OUTX_H_XL,
	OUTY_L_XL,
	OUTY_H_XL,
	OUTZ_L_XL,
	OUTZ_H_XL,
} imu_register_t;

// Standard options
#define CTRL1_XL_HIGH_PERFORMANCE 0xA0U

void imu_lsm6ds_write_byte(imu_register_t register_address, uint8_t value);

uint8_t imu_lsm6ds_read_byte(imu_register_t register_address);

#endif /* INC_IMU_LSM6DS_H_ */
