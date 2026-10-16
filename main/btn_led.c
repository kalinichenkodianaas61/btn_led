#include <stdio.h>
#include <stdbool.h>
#include <freertos/FreeRTOS.h> // must come first
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_log.h>

#define btn_gpio GPIO_NUM_4
#define led_gpio GPIO_NUM_18

static const char *TAG = "btn_led";

void app_main(void) {

    // button config with internal pull-up
    gpio_config_t btn_conf = {
        .pin_bit_mask = (1ULL << btn_gpio),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&btn_conf));

    // led config (output)
    gpio_config_t led_conf = {
        .pin_bit_mask = (1ULL << led_gpio),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&led_conf));

    bool last_state = 1; // 1 -> released

    gpio_set_level(led_gpio, 0); // led off at start

    while (1) {
        bool state = gpio_get_level(btn_gpio);

        gpio_set_level(led_gpio, state == 0); // pressed (0) -> on, released (1) -> off

        if (state != last_state) {  // check if pressed        
            if (state == 0) {
                ESP_LOGI(TAG, "LED ON\n");
            }  else {
                ESP_LOGI(TAG, "LED OFF\n");
            }
            last_state = state;
        }

        /* FOR TOGGLE
        static int led_on = 0; // declare variable before while
        
        if (last_state == 1 && state == 0) { // pressed
            led_on = !led_on; // toggle
            gpio_set_level(led_gpio, led_on);
            ESP_LOGI(TAG, "LED %s\n", led_on ? "on" : "off");
        }
        last_state = state;    */

        vTaskDelay(pdMS_TO_TICKS(20)); // poll + debounce
    }


}
