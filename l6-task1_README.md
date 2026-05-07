# l6-task1: LED Sensor Driver using Zephyr Driver Model

## Overview
This task creates a custom sensor driver for an on-board LED following the Zephyr driver development structure taught in Lecture 6. The driver implements the standard Zephyr Sensor API — `sensor_sample_fetch` and `sensor_channel_get` — to toggle and read the LED state.

---

## Project Structure

```
zephyr-course/
├── app/
│   ├── CMakeLists.txt       ← includes driver module via ZEPHYR_EXTRA_MODULES
│   ├── app.overlay          ← adds our,driver DTS node
│   ├── prj.conf             ← enables CONFIG_SENSOR
│   └── src/
│       └── main.c           ← uses sensor API to control LED
└── our-driver-module/
    ├── CMakeLists.txt       ← module root, delegates to drivers/our_driver
    ├── Kconfig              ← module root config
    ├── zephyr/
    │   └── module.yml       ← registers module with West
    ├── include/our_drivers/
    │   └── our_driver.h     ← public C API
    ├── drivers/our_driver/
    │   ├── our_driver.c     ← driver implementation
    │   ├── CMakeLists.txt   ← compiles our_driver.c if CONFIG_OUR_DRIVER=y
    │   └── Kconfig          ← auto-enables when DTS node present
    └── dts/bindings/driver/
        └── our,driver.yaml  ← DTS binding for compatible = "our,driver"
```

---

## Driver Lifecycle (from Lecture 6)

1. `app.overlay` declares DTS node with `compatible = "our,driver"`
2. `our,driver.yaml` binding defines the compatible string and properties
3. Kconfig symbol `OUR_DRIVER` auto-enables via `DT_HAS_OUR_DRIVER_ENABLED`
4. CMake compiles `our_driver.c`
5. Driver instance created from DTS via `DT_INST_FOREACH_STATUS_OKAY`
6. App gets device handle via `DEVICE_DT_GET_ANY` and uses Sensor API

---

## API Used

| API | What it does |
|-----|-------------|
| `sensor_sample_fetch(dev)` | Toggles the LED ON/OFF |
| `sensor_channel_get(dev, chan, val)` | Returns current LED state (0=OFF, 1=ON) |

---

## Key Files Explained

### `our,driver.yaml` — DTS Binding
Links the `compatible = "our,driver"` string to the driver and defines properties.

### `our_driver.c` — Driver Implementation
```c
#define DT_DRV_COMPAT our_driver  /* links C to DTS compatible */

/* Toggles LED on each fetch */
static int our_driver_sample_fetch(...) { ... }

/* Returns LED state as sensor value */
static int our_driver_channel_get(...) { ... }

/* Instantiates driver for each enabled DTS node */
DT_INST_FOREACH_STATUS_OKAY(OUR_DRIVER_DEFINE)
```

### `app.overlay` — DTS Node
```dts
our_led_sensor: our_led_sensor {
    compatible = "our,driver";
    gpios = <&gpio0 13 GPIO_ACTIVE_LOW>;
    status = "okay";
};
```

### `main.c` — Application
```c
const struct device *dev = DEVICE_DT_GET_ANY(our_driver);

sensor_sample_fetch(dev);              /* toggles LED */
sensor_channel_get(dev, chan, &val);   /* reads LED state */
```

---

## Expected Serial Output

```
Our Driver initialized
LED Sensor Driver Demo started
LED toggled: ON  (fetch #1)
LED state from sensor_channel_get: ON
LED toggled: OFF (fetch #2)
LED state from sensor_channel_get: OFF
...
```

---

## Build Instructions

```bat
west build -b nrf52840dk/nrf52840 app -p
west flash
```

---

## Git Tag
```bash
git tag l6-task1
git push origin --tags
```
