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
 * @param dev Pointer to the device structure
 * @return Current LED state (0 = OFF, 1 = ON)
 */
int our_driver_get_led_state(const struct device *dev);

/**
 * @brief Custom extension API - set blink interval
 *        Changes the blink_interval_ms field in the dynamic data struct
 * @param dev Pointer to the device structure
 * @param interval_ms New blink interval in milliseconds
 * @return 0 on success, negative errno on failure
 */
int our_driver_set_blink_interval(const struct device *dev, uint32_t interval_ms);

/**
 * @brief Custom extension API - get blink interval
 * @param dev Pointer to the device structure
 * @return Current blink interval in milliseconds
 */
uint32_t our_driver_get_blink_interval(const struct device *dev);

#endif /* INCLUDE_OUR_DRIVERS_OUR_DRIVER_H_ */
