/*
 * Copyright (c) 2024 Swaroop
 * SPDX-License-Identifier: Apache-2.0
 *
 * l7-task1: Shell integration for LED sensor driver
 * Adds "sensor" root shell command with subcommands:
 *   fetch - calls sensor_sample_fetch()
 *   read  - calls sensor_channel_get() and prints result
 *   info  - prints device name and ready state
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

/* Get our driver device */
static const struct device *get_sensor_dev(void)
{
    return DEVICE_DT_GET_ANY(our_driver);
}

/* sensor fetch subcommand - calls sensor_sample_fetch() */
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

/* sensor read subcommand - calls sensor_channel_get() and prints result */
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

/* sensor info subcommand - prints device name and ready state */
static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    const struct device *dev = get_sensor_dev();

    shell_print(sh, "Device name  : %s", dev->name);
    shell_print(sh, "Device ready : %s", device_is_ready(dev) ? "YES" : "NO");
    return 0;
}

/* Register subcommands under "sensor" root command */
SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
    SHELL_CMD(fetch, NULL, "Call sensor_sample_fetch() - toggles LED", cmd_sensor_fetch),
    SHELL_CMD(read,  NULL, "Call sensor_channel_get() - prints LED state", cmd_sensor_read),
    SHELL_CMD(info,  NULL, "Print device name and ready state", cmd_sensor_info),
    SHELL_SUBCMD_SET_END
);

/* Register root "sensor" command */
SHELL_CMD_REGISTER(sensor, &sub_sensor, "LED Sensor Driver commands", NULL);

int main(void)
{
    const struct device *dev = get_sensor_dev();

    if (!device_is_ready(dev)) {
        LOG_ERR("Sensor device not ready");
        return -ENODEV;
    }

    LOG_INF("LED Sensor Shell Demo started");
    LOG_INF("Try: sensor fetch / sensor read / sensor info");

    return 0;
}
