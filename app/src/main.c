/*
 * Copyright (c) 2024 Swaroop
 * SPDX-License-Identifier: Apache-2.0
 *
 * l7-task2: Shell integration with custom extension API
 * - Background thread blinks LED at current blink_interval_ms
 * - Shell "sensor set <value>" changes interval dynamically
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>
#include "our_drivers/our_driver.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#define BLINK_INTERVAL_MIN  100
#define BLINK_INTERVAL_MAX  2000

/* Background blink thread */
#define BLINK_STACK_SIZE 512
#define BLINK_PRIORITY   5

K_THREAD_STACK_DEFINE(blink_stack, BLINK_STACK_SIZE);
static struct k_thread blink_thread;

static void blink_thread_fn(void *arg1, void *arg2, void *arg3)
{
    const struct device *dev = (const struct device *)arg1;

    while (1) {
        /* Fetch toggles the LED */
        sensor_sample_fetch(dev);

        /* Sleep for current blink interval */
        uint32_t interval = our_driver_get_blink_interval(dev);
        k_msleep(interval);
    }
}

/* Get our driver device */
static const struct device *get_sensor_dev(void)
{
    return DEVICE_DT_GET_ANY(our_driver);
}

/* sensor fetch subcommand */
static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    const struct device *dev = get_sensor_dev();

    if (!device_is_ready(dev)) {
        shell_error(sh, "Sensor device not ready");
        return -ENODEV;
    }

    int ret = sensor_sample_fetch(dev);
    if (ret < 0) {
        shell_error(sh, "sensor_sample_fetch failed: %d", ret);
        return ret;
    }

    shell_print(sh, "Fetch OK - LED toggled");
    return 0;
}

/* sensor read subcommand */
static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    const struct device *dev = get_sensor_dev();
    struct sensor_value val;

    if (!device_is_ready(dev)) {
        shell_error(sh, "Sensor device not ready");
        return -ENODEV;
    }

    int ret = sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
    if (ret < 0) {
        shell_error(sh, "sensor_channel_get failed: %d", ret);
        return ret;
    }

    shell_print(sh, "LED state: %s (%d)", val.val1 ? "ON" : "OFF", val.val1);
    return 0;
}

/* sensor info subcommand */
static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    const struct device *dev = get_sensor_dev();

    shell_print(sh, "Device name    : %s", dev->name);
    shell_print(sh, "Device ready   : %s", device_is_ready(dev) ? "YES" : "NO");
    shell_print(sh, "Blink interval : %d ms",
                our_driver_get_blink_interval(dev));
    return 0;
}

/* sensor set <value> subcommand */
static int cmd_sensor_set(const struct shell *sh, size_t argc, char **argv)
{
    if (argc != 2) {
        shell_error(sh, "Usage: sensor set <value>");
        shell_error(sh, "Value range: %d - %d ms",
                    BLINK_INTERVAL_MIN, BLINK_INTERVAL_MAX);
        return -EINVAL;
    }

    char *endptr;
    long value = strtol(argv[1], &endptr, 10);

    if (*endptr != '\0') {
        shell_error(sh, "Invalid value: '%s' is not a number", argv[1]);
        return -EINVAL;
    }

    if (value < BLINK_INTERVAL_MIN || value > BLINK_INTERVAL_MAX) {
        shell_error(sh, "Value %ld out of range [%d - %d]",
                    value, BLINK_INTERVAL_MIN, BLINK_INTERVAL_MAX);
        return -EINVAL;
    }

    const struct device *dev = get_sensor_dev();

    if (!device_is_ready(dev)) {
        shell_error(sh, "Sensor device not ready");
        return -ENODEV;
    }

    int ret = our_driver_set_blink_interval(dev, (uint32_t)value);
    if (ret < 0) {
        shell_error(sh, "Failed to set blink interval: %d", ret);
        return ret;
    }

    shell_print(sh, "Blink interval set to %ld ms - LED speed changed!", value);
    return 0;
}

/* Register subcommands */
SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
    SHELL_CMD(fetch, NULL,
              "Call sensor_sample_fetch() - toggles LED",
              cmd_sensor_fetch),
    SHELL_CMD(read,  NULL,
              "Call sensor_channel_get() - prints LED state",
              cmd_sensor_read),
    SHELL_CMD(info,  NULL,
              "Print device name, ready state and blink interval",
              cmd_sensor_info),
    SHELL_CMD_ARG(set, NULL,
              "Set blink interval (100-2000 ms): sensor set <value>",
              cmd_sensor_set, 2, 0),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sub_sensor, "LED Sensor Driver commands", NULL);

int main(void)
{
    const struct device *dev = get_sensor_dev();

    if (!device_is_ready(dev)) {
        LOG_ERR("Sensor device not ready");
        return -ENODEV;
    }

    LOG_INF("LED Sensor Shell Demo started (l7-task2)");
    LOG_INF("Commands: sensor fetch / read / info / set <100-2000>");

    /* Start background blink thread */
    k_thread_create(&blink_thread, blink_stack,
                    K_THREAD_STACK_SIZEOF(blink_stack),
                    blink_thread_fn,
                    (void *)dev, NULL, NULL,
                    BLINK_PRIORITY, 0, K_NO_WAIT);

    k_thread_name_set(&blink_thread, "blink");

    return 0;
}
