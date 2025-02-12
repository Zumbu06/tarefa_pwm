#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 22
#define LED_PIN 12
#define PWM_FREQ 50

int main() {
    stdio_init_all();

    // Configura o PWM para o servo no pino especificado
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(SERVO_PIN);

    // Divide o clock e configura o wrap para 50Hz (20ms)
    pwm_set_clkdiv(slice, 125.0);
    pwm_set_wrap(slice, 20000);
    pwm_set_enabled(slice, true);

    // Movimentos iniciais do servo
    pwm_set_gpio_level(SERVO_PIN, 2400);  // 180°
    sleep_ms(5000);
    pwm_set_gpio_level(SERVO_PIN, 1470);  // 90°
    sleep_ms(5000);
    pwm_set_gpio_level(SERVO_PIN, 500);   // 0°
    sleep_ms(5000);

    // Variável para controlar a movimentação suave do servo
    uint16_t fade = 500;

    while (1) {
        // Movimento de 0° para 180°
        while (fade != 2400) {
            pwm_set_gpio_level(SERVO_PIN, fade);
            fade += 5;
            sleep_ms(10);
        }

        // Movimento de 180° para 0°
        while (fade != 500) {
            pwm_set_gpio_level(SERVO_PIN, fade);
            fade -= 5;
            sleep_ms(10);
        }
    }

    return 0;
}