#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

/* Use app-led alias defined in app.overlay (points to led0 on your board) */
#define LED_NODE  DT_ALIAS(app_led)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    bool led_state = true;

    if (!gpio_is_ready_dt(&led)) return 0;
    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    LOG_INF("Heartbeat LED started. Period: %d ms", CONFIG_APP_HEARTBEAT_PERIOD_MS);

    while (1) {
        if (gpio_pin_toggle_dt(&led) < 0) return 0;
        led_state = !led_state;

        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");

        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }

    return 0;
}