/*
 * Copyright (c) 2024 Swaroop
 * SPDX-License-Identifier: Apache-2.0
 *
 * l6-task2: LED Sensor Driver with Custom Extension API
 * - sensor_sample_fetch toggles LED
 * - sensor_channel_get reads LED state
 * - our_driver_set_blink_interval changes blink_interval_ms in data struct
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include "our_drivers/our_driver.h"

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
    LOG_INF("Initial blink interval: %d ms",
            our_driver_get_blink_interval(dev));

    int cycle = 0;

    while (1) {
        /* Every 5 cycles change the blink interval via custom extension API */
        if (cycle == 5) {
            LOG_INF("--- Changing blink interval to 500ms ---");
            our_driver_set_blink_interval(dev, 500);
        } else if (cycle == 10) {
            LOG_INF("--- Changing blink interval to 2000ms ---");
            our_driver_set_blink_interval(dev, 2000);
            cycle = 0;
        }

        /* Toggle LED via sensor_sample_fetch */
        sensor_sample_fetch(dev);

        /* Read LED state via sensor_channel_get */
        sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
        LOG_INF("LED state: %s | interval: %d ms",
                val.val1 ? "ON" : "OFF",
                our_driver_get_blink_interval(dev));

        k_msleep(our_driver_get_blink_interval(dev));
        cycle++;
    }

    return 0;
}
