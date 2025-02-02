#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

#define LED_RED 13
#define LED_YELLOW 12
#define LED_GREEN 11
#define BUTTON 5

volatile bool leds_on = false;

int64_t turn_off_callback(alarm_id_t id, void *user_data);
//void turn_on_leds();

void turn_on_leds() {
    if (!leds_on) { //se estão desligados os leds entra no if
        leds_on = true;
        gpio_put(LED_RED, 1);
        gpio_put(LED_YELLOW, 1);
        gpio_put(LED_GREEN, 1);
        add_alarm_in_ms(3000, turn_off_callback, (void*)LED_RED, false);
 
    }
}

int64_t turn_off_callback(alarm_id_t id, void *user_data) {
    uint led = (uint) user_data;
    gpio_put(led, 0);
    
    if (led == LED_RED) {
        add_alarm_in_ms(3000, turn_off_callback, (void*)LED_YELLOW, false);
    } else if (led == LED_YELLOW) {
        add_alarm_in_ms(3000, turn_off_callback, (void*)LED_GREEN, false);
    } else if (led == LED_GREEN) {
        leds_on = false;
    }
    
    return 0;
}

int main() {
    stdio_init_all();
    
    gpio_init(LED_RED);
    gpio_init(LED_YELLOW);
    gpio_init(LED_GREEN);
    gpio_init(BUTTON);
    
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_YELLOW, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(BUTTON, GPIO_IN);
    gpio_pull_up(BUTTON);
    
    while (1) {
        if (gpio_get(BUTTON) == 0) {
            //sleep_ms(50);
            //if (gpio_get(BUTTON) == 0) {
                turn_on_leds();
            //}
        }
    }
}
