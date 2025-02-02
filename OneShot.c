#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define LED_RED 13
#define LED_YELLOW 12
#define LED_GREEN 11

volatile int state = 0; // Controla o estado do semáforo

bool repeating_timer_callback(struct repeating_timer *t) {
    // Desliga todos os LEDs
    gpio_put(LED_RED, 0);
    gpio_put(LED_YELLOW, 0);
    gpio_put(LED_GREEN, 0);

    // Acende o LED correspondente ao estado
    if (state == 0) {
        gpio_put(LED_RED, 1);
    } else if (state == 1) {
        gpio_put(LED_YELLOW, 1);
    } else {
        gpio_put(LED_GREEN, 1);
    }

    // Avança para o próximo estado
    state = (state + 1) % 3; // O operador módulo (% 3) garante que state sempre fique entre 0, 1 e 2:
    return true; // Mantém o temporizador repetindo
}

void config(){
    stdio_init_all();
    // Configuração dos pinos dos LEDs
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_init(LED_YELLOW);
    gpio_set_dir(LED_YELLOW, GPIO_OUT);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
}

int main() {
    config();

    struct repeating_timer timer;
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);

    while (true) {
        printf("Mais um segundo passou e nem viu\n");
        sleep_ms(1000);
    }
}
