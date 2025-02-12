#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Definições
#define SERVO_PIN 22  // GPIO para PWM do servomotor
#define LED_PIN 12     // GPIO para LED RGB
#define PWM_FREQ 50    // Frequência de 50Hz (período de 20ms)

// Função para calcular o duty cycle correspondente ao tempo de pulso desejado
void set_servo_pulse(uint gpio, uint32_t pulse_us) {
    uint slice = pwm_gpio_to_slice_num(gpio);
    uint wrap = pwm_get_wrap(slice);
    
    // Converte microssegundos para duty cycle proporcional ao período do PWM
    uint32_t duty = (pulse_us * wrap) / 20000;  // 20000us = 20ms (1 ciclo completo)

    pwm_set_chan_level(slice, pwm_gpio_to_channel(gpio), duty);
}

// Configura o PWM na GPIO especificada
void setup_pwm(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(gpio);

    // Define o divisor de clock para alcançar 50Hz
    pwm_set_wrap(slice, 25000); // PWM de 20ms com clock padrão de 125MHz / 50Hz
    pwm_set_enabled(slice, true);
}

// Função para mover o servo para um ângulo específico
void move_servo(uint gpio, uint32_t pulse_us, uint delay_ms) {
    set_servo_pulse(gpio, pulse_us);
    sleep_ms(delay_ms);
}

int main() {
    stdio_init_all();
    
    setup_pwm(SERVO_PIN);
    setup_pwm(LED_PIN);

    // Movimentos iniciais do servo
    move_servo(SERVO_PIN, 2400, 5000);  // 180°
    move_servo(SERVO_PIN, 1470, 5000);  // 90°
    move_servo(SERVO_PIN, 500, 5000);   // 0°

    // Rotina de movimentação suave do servo entre 0° e 180°
    while (1) {
        // Movimento de 0° para 180°
        for (uint32_t pulse = 500; pulse <= 2400; pulse += 5) {
            set_servo_pulse(SERVO_PIN, pulse);
            sleep_ms(10);
        }
        
        // Movimento de 180° para 0°
        for (uint32_t pulse = 2400; pulse >= 500; pulse -= 5) {
            set_servo_pulse(SERVO_PIN, pulse);
            sleep_ms(10);
        }
    }

    return 0;
}
