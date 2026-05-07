/*
 * Copyright (c) 2024 Swaroop
 * SPDX-License-Identifier: Apache-2.0
 *
 * Public C API for our custom LED sensor driver
 */

#ifndef INCLUDE_OUR_DRIVERS_OUR_DRIVER_H_
#define INCLUDE_OUR_DRIVERS_OUR_DRIVER_H_

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

/**
 * @brief Get the current LED state
 *
 * @param dev Pointer to the device structure
 * @return Current LED state (0 = OFF, 1 = ON)
 */
int our_driver_get_led_state(const struct device *dev);

#endif /* INCLUDE_OUR_DRIVERS_OUR_DRIVER_H_ */
