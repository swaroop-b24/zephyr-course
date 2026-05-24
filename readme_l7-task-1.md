# l7-task1: Shell Integration for LED Sensor Driver

## Overview
This task adds a **Zephyr Shell** interface to the LED sensor driver from l6-task1. Instead of automatic blinking in a loop, the LED is now controlled interactively via UART shell commands.

---

## Shell Commands

```
uart:~$ sensor info
uart:~$ sensor fetch
uart:~$ sensor read
```

| Command | What it does |
|---------|-------------|
| `sensor info` | Prints device name and ready state |
| `sensor fetch` | Calls `sensor_sample_fetch()` — toggles LED ON/OFF |
| `sensor read` | Calls `sensor_channel_get()` — prints current LED state |

---

## Project Structure

```
app/
├── CMakeLists.txt
├── app.overlay          ← our,driver DTS node
├── prj.conf             ← Shell + Sensor enabled
└── src/
    └── main.c           ← Shell commands registered here

our-driver-module/
├── drivers/our_driver/
│   └── our_driver.c     ← sensor_sample_fetch / sensor_channel_get
└── include/our_drivers/
    └── our_driver.h
```

---

## Key Concepts from Lecture 7

### Shell Command Registration
```c
/* Subcommands array */
SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
    SHELL_CMD(fetch, NULL, "Toggle LED", cmd_sensor_fetch),
    SHELL_CMD(read,  NULL, "Read LED state", cmd_sensor_read),
    SHELL_CMD(info,  NULL, "Print device info", cmd_sensor_info),
    SHELL_SUBCMD_SET_END
);

/* Root command */
SHELL_CMD_REGISTER(sensor, &sub_sensor, "LED Sensor commands", NULL);
```

### Command Handler Pattern
```c
static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv)
{
    shell_print(sh, "Fetch OK");   /* use shell_print, never printf */
    return 0;
}
```

### prj.conf — Enabling Shell
```
CONFIG_SHELL=y
CONFIG_SHELL_BACKEND_SERIAL=y   /* UART backend */
CONFIG_SHELL_LOG_BACKEND=y      /* logs appear in shell */
CONFIG_UART_CONSOLE=n           /* shell takes over UART */
```

---

## Build and Flash

```bat
west build -b nrf52840dk/nrf52840 app -p
west flash
```

---

## Connect via Serial Terminal

Open PuTTY or any serial terminal:
- Port: **COM10** or **COM11** (JLink CDC UART)
- Baud: **115200**
- Press **Enter** to get the `uart:~$` prompt

---

## Expected Output

```
*** Booting Zephyr OS build v4.4.0-rc1 ***
[00:00:00.393] <inf> our_driver: Our Driver initialized
[00:00:00.393] <inf> main: LED Sensor Shell Demo started
[00:00:00.393] <inf> main: Try: sensor fetch / sensor read / sensor info

uart:~$ sensor info
Device name  : our_led_sensor
Device ready : YES

uart:~$ sensor fetch
Fetch OK - LED toggled

uart:~$ sensor read
LED state: ON (1)

uart:~$ sensor fetch
Fetch OK - LED toggled

uart:~$ sensor read
LED state: OFF (0)
```

---

## Git Tag
```bash
git tag l7-task1
git push origin --tags
```
