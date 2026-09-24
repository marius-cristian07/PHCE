#include <cstdint>
#include <iostream>

#include "hardware/gpio.h"
#include "hardware/sync.h"  // Needed for critical section protection
#include "pico/stdlib.h"

constexpr uint LED_PIN {7};
constexpr uint BUTTON_PIN {10};
constexpr std::uint32_t WORK_MS {500};
constexpr uint32_t DEBOUNCE_MS {50};

// Variables shared between main loop and ISR
volatile bool g_led_state {false};
volatile unsigned int g_pending_presses {0};
volatile unsigned int g_total_presses {0};

// TODO 1: Interrupt Service Routine (ISR) body
void button_isr(uint gpio, uint32_t events)
{
    static uint32_t last_time_ms = 0;
    uint32_t now_ms = to_ms_since_boot(get_absolute_time());

    if (gpio == BUTTON_PIN && (events & GPIO_IRQ_EDGE_RISE))
    {
        // Simple software debounce inside ISR
        if (now_ms - last_time_ms >= DEBOUNCE_MS)
        {
            last_time_ms = now_ms;
            
            // LED reacts INSTANTLY inside the interrupt
            g_led_state = !g_led_state;
            gpio_put(LED_PIN, g_led_state);

            ++g_pending_presses;
            ++g_total_presses;
        }
    }
}

// TODO 3: Critical section protection
unsigned int take_pending_presses()
{
    // Save interrupt status and disable interrupts so ISR can't interrupt this read/write gap
    uint32_t status = save_and_disable_interrupts();

    unsigned int count = g_pending_presses;
    g_pending_presses = 0;

    // Restore original interrupt state
    restore_interrupts(status);

    return count;
}

int main()
{
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, false);

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_down(BUTTON_PIN);

    // TODO 2: Register the ISR callback for rising edge on BUTTON_PIN
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_RISE, true, &button_isr);

    std::cout << "irq_led_toggle started. Press the button." << std::endl;

    while (true)
    {
        unsigned int pending = take_pending_presses();
        if (pending > 0)
        {
            std::cout << "Pending presses processed: " << pending 
                      << " | Total: " << g_total_presses << std::endl;
        }

        sleep_ms(WORK_MS);
    }
}