#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

#define LED_RED 13
#define LED_YELLOW 12
#define LED_GREEN 11
#define BUTTON 5

volatile bool leds_on = false; //variavel que inicialmente indica que os leds estão desligados

#define DEBOUNCE_DELAY 100  // Para o botão: Tempo de debounce para evitar múltiplas detecções de um único toque
volatile uint32_t last_interrupt_time=0; // Para o botão: Armazena o tempo da última interrupção

int64_t turn_off_callback(alarm_id_t id, void *user_data);
//void turn_on_leds();

void turn_on_leds() {
    if (!leds_on) { //se estão desligados os leds entra no if
        leds_on = true;
        gpio_put(LED_RED, 1);
        gpio_put(LED_YELLOW, 1);
        gpio_put(LED_GREEN, 1);
        add_alarm_in_ms(3000, turn_off_callback, (void*)LED_RED, false); //alarma e ponteiro para desligar o LED
    }
}

int64_t turn_off_callback(alarm_id_t id, void *user_data) {
    uint led = (uint) user_data;
    gpio_put(led, 0);
    
    if (led == LED_RED) {
        add_alarm_in_ms(3000, turn_off_callback, (void*)LED_YELLOW, false); //alarma e ponteiro para desligar o LED
    } else if (led == LED_YELLOW) {
        add_alarm_in_ms(3000, turn_off_callback, (void*)LED_GREEN, false); ///alarma e ponteiro para desligar o LED
    } else if (led == LED_GREEN) {
        leds_on = false;
    }
    return 0;
}

// Função de interrupção para tratar eventos nos botões
void gpio_irq_handler(uint gpio,uint32_t events)
{
    uint32_t current_time=to_ms_since_boot(get_absolute_time()); // Obtém o tempo atual em milissegundos
    if(current_time-last_interrupt_time>DEBOUNCE_DELAY){
        last_interrupt_time=current_time; // Atualiza o tempo da última interrupção
        turn_on_leds();  
    }
}

void config(){
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
}    

int main() {
    config();
    gpio_set_irq_enabled_with_callback(BUTTON,GPIO_IRQ_EDGE_FALL, true,&gpio_irq_handler);

    while (true) {
        sleep_ms(10);  // Evita uso excessivo de CPU
    }
}
