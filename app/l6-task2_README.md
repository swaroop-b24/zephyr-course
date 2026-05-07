# l6-task2: Custom Extension API for LED Sensor Driver

## Overview
This task extends the LED sensor driver from l6-task1 by adding a **custom extension API function** that modifies a parameter in the driver's dynamic data struct at runtime.

---

## What Was Added

### Custom Extension API Functions
Two new functions added to the driver:

| Function | Description |
|----------|-------------|
| `our_driver_set_blink_interval(dev, ms)` | Changes `blink_interval_ms` in the dynamic data struct |
| `our_driver_get_blink_interval(dev)` | Returns current blink interval from the dynamic data struct |

### Dynamic Data Struct Change
A new field `blink_interval_ms` was added to `our_driver_data`:

```c
struct our_driver_data {
    int led_state;               /* 0 = OFF, 1 = ON */
    int fetch_count;             /* number of fetches performed */
    uint32_t blink_interval_ms; /* NEW: custom extension parameter */
};
```

---

## Project Structure

```
our-driver-module/
├── CMakeLists.txt
├── Kconfig
├── zephyr/
│   └── module.yml
├── include/our_drivers/
│   └── our_driver.h        ← added set/get blink interval API
├── drivers/our_driver/
│   ├── our_driver.c        ← implemented custom extension functions
│   ├── CMakeLists.txt
│   └── Kconfig
└── dts/bindings/driver/
    └── our,driver.yaml

app/
├── CMakeLists.txt
├── app.overlay
├── prj.conf
└── src/
    └── main.c              ← calls our_driver_set_blink_interval()
```

---

## How It Works

### Driver Side (`our_driver.c`)
The `our_driver_set_blink_interval()` function directly modifies the `blink_interval_ms` field in the dynamic data struct:

```c
int our_driver_set_blink_interval(const struct device *dev, uint32_t interval_ms)
{
    struct our_driver_data *data = dev->data;
    data->blink_interval_ms = interval_ms;  /* changes dynamic data struct */
    return 0;
}
```

### Application Side (`main.c`)
The app calls the custom extension API to change the blink speed at runtime:

```c
/* Change blink interval to 500ms after 5 cycles */
our_driver_set_blink_interval(dev, 500);

/* Change blink interval to 2000ms after 10 cycles */
our_driver_set_blink_interval(dev, 2000);
```

---

## Expected Serial Output

```
Our Driver initialized. Default blink interval: 1000 ms
LED Sensor Driver Demo started
Initial blink interval: 1000 ms
LED toggled: ON  (fetch #1, interval: 1000 ms)
LED state: ON  | interval: 1000 ms
...
--- Changing blink interval to 500ms ---
LED toggled: ON  (fetch #6, interval: 500 ms)
LED state: ON  | interval: 500 ms
...
--- Changing blink interval to 2000ms ---
LED toggled: ON  (fetch #11, interval: 2000 ms)
LED state: ON  | interval: 2000 ms
```

---

## Build Instructions

```bat
west build -b nrf52840dk/nrf52840 app -p
west flash
```

---

## Key Concepts from Lecture 6

| Concept | Where Used |
|---------|-----------|
| `DT_DRV_COMPAT` | Links C code to DTS `compatible = "our,driver"` |
| `DT_INST_FOREACH_STATUS_OKAY` | Instantiates driver for each enabled DTS node |
| `sensor_sample_fetch` | Toggles LED ON/OFF |
| `sensor_channel_get` | Reads current LED state |
| **Custom extension API** | `our_driver_set_blink_interval()` changes dynamic data |
| `POST_KERNEL` init level | Driver initialized after kernel is running |

---

## Git Tag
```bash
git tag l6-task2
git push origin --tags
```
