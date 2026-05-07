/*
 * Copyright (c) 2024 Swaroop
 * SPDX-License-Identifier: Apache-2.0
 *
 * l6-task1: LED Sensor Driver Demo
 * Uses sensor_sample_fetch to toggle LED
 * Uses sensor_channel_get to read LED state
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    const struct device *dev = DEVICE_DT_GET_ANY(our_driver);
    struct sensor_value val;

    if (!device_is_ready(dev)) {
        LOG_ERR("Our driver device not ready");
        return -ENODEV;
    }

    LOG_INF("LED Sensor Driver Demo started");

    while (1) {
        /* Toggle LED via sensor_sample_fetch */
        sensor_sample_fetch(dev);

        /* Read LED state via sensor_channel_get */
        sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);

        LOG_INF("LED state from sensor_channel_get: %s",
                val.val1 ? "ON" : "OFF");

        k_msleep(1000);
    }

    return 0;
}
