/*
 * Copyright (c) 2024 Swaroop
 * SPDX-License-Identifier: Apache-2.0
 *
 * LED Sensor Driver Implementation
 * - sensor_sample_fetch: toggles the LED ON/OFF
 * - sensor_channel_get:  returns current LED state
 */

#define DT_DRV_COMPAT our_driver

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include "our_drivers/our_driver.h"

LOG_MODULE_REGISTER(our_driver, CONFIG_SENSOR_LOG_LEVEL);

/* Dynamic data struct - holds runtime state */
struct our_driver_data {
    int led_state;       /* 0 = OFF, 1 = ON */
    int fetch_count;     /* number of fetches performed */
};

/* Static config struct - holds compile-time config from DTS */
struct our_driver_config {
    struct gpio_dt_spec led_gpio;
};

/* sensor_sample_fetch - toggles the LED */
static int our_driver_sample_fetch(const struct device *dev,
                                   enum sensor_channel chan)
{
    const struct our_driver_config *cfg = dev->config;
    struct our_driver_data *data = dev->data;

    if (chan != SENSOR_CHAN_ALL) {
        return -ENOTSUP;
    }

    /* Toggle LED state */
    data->led_state = !data->led_state;
    data->fetch_count++;

    gpio_pin_set_dt(&cfg->led_gpio, data->led_state);

    LOG_INF("LED toggled: %s (fetch #%d)",
            data->led_state ? "ON" : "OFF",
            data->fetch_count);

    return 0;
}

/* sensor_channel_get - returns LED state as sensor value */
static int our_driver_channel_get(const struct device *dev,
                                  enum sensor_channel chan,
                                  struct sensor_value *val)
{
    struct our_driver_data *data = dev->data;

    if (chan != SENSOR_CHAN_ALL) {
        return -ENOTSUP;
    }

    val->val1 = data->led_state;  /* 0 = OFF, 1 = ON */
    val->val2 = 0;

    return 0;
}

/* Public API: get LED state directly */
int our_driver_get_led_state(const struct device *dev)
{
    struct our_driver_data *data = dev->data;
    return data->led_state;
}

/* Driver init function */
static int our_driver_init(const struct device *dev)
{
    const struct our_driver_config *cfg = dev->config;
    struct our_driver_data *data = dev->data;

    if (!gpio_is_ready_dt(&cfg->led_gpio)) {
        LOG_ERR("LED GPIO not ready");
        return -ENODEV;
    }

    gpio_pin_configure_dt(&cfg->led_gpio, GPIO_OUTPUT_INACTIVE);

    data->led_state = 0;
    data->fetch_count = 0;

    LOG_INF("Our Driver initialized");
    return 0;
}

/* Sensor API vtable */
static const struct sensor_driver_api our_driver_api = {
    .sample_fetch = our_driver_sample_fetch,
    .channel_get  = our_driver_channel_get,
};

/* Instantiation macro */
#define OUR_DRIVER_DEFINE(inst)                                         \
    static struct our_driver_data data_##inst;                          \
    static const struct our_driver_config cfg_##inst = {               \
        .led_gpio = GPIO_DT_SPEC_INST_GET(inst, gpios),                \
    };                                                                  \
    DEVICE_DT_INST_DEFINE(inst,                                        \
        our_driver_init, NULL,                                         \
        &data_##inst, &cfg_##inst,                                     \
        POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY,                      \
        &our_driver_api);

DT_INST_FOREACH_STATUS_OKAY(OUR_DRIVER_DEFINE)
