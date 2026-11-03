#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"

int main() {
    // TODO 1: Initialize stdio so printf reaches Serial Monitor over UART
    stdio_init_all();

    // TODO 2: Initialize PicoBricks LED (GPIO 7) as output
    gpio_init(7);
    gpio_set_dir(7, GPIO_OUT);

    printf("BUILD: FLASH\n");

    // Initialize ADC temperature sensor
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    while (true) {
        // Read raw sensor value and compute temperature in Celsius
        uint16_t raw = adc_read();
        const float voltage = raw * 3.3f / (1 << 12);
        const float temp_c = 27.0f - (voltage - 0.706f) / 0.001721f;

        // Calculate blink delay (hotter chip = faster blink)
        int delay_ms = static_cast<int>(1000 - 50 * (temp_c - 20));
        if (delay_ms < 100) delay_ms = 100;
        if (delay_ms > 950) delay_ms = 950;

        sleep_ms(500);

        // TODO 3: Print temperature and raw ADC value
        printf("raw: %u, temp_c: %.2f C\n", raw, temp_c);

        // TODO 4: Blink LED on/off using GPIO 7
        gpio_put(7, 1);
        sleep_ms(delay_ms);
        gpio_put(7, 0);
        sleep_ms(delay_ms);
    }
    return 0;
}