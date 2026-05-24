#ifndef INCLUDE_OUR_DRIVERS_OUR_DRIVER_H_ 
#define INCLUDE_OUR_DRIVERS_OUR_DRIVER_H_ 
#include <zephyr/device.h> 
#include <zephyr/drivers/sensor.h> 
int our_driver_get_led_state(const struct device *dev); 
int our_driver_set_blink_interval(const struct device *dev, uint32_t interval_ms); 
uint32_t our_driver_get_blink_interval(const struct device *dev); 
#endif 
