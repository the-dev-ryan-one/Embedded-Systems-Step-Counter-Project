#include "app.h"
#include "gpio.h"

void app_main(void) {

    HAL_Delay(200);
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11)) {
        rgb_colour_off(RGB_GREEN);
        rgb_colour_off(RGB_BLUE);
        rgb_colour_on(RGB_RED);
        rgb_led_on(RGB_LEFT);
    } else {
        rgb_led_off(RGB_LEFT);
    }

    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11)) {

    	 rgb_colour_on(RGB_RED);
    	 rgb_colour_on(RGB_GREEN);
    	 rgb_colour_on(RGB_BLUE);

        rgb_led_on(RGB_UP);
    } else {
        rgb_led_off(RGB_UP);
    }

    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)) {
        rgb_colour_on(RGB_RED);
        rgb_colour_on(RGB_GREEN);
        rgb_colour_on(RGB_BLUE);
        rgb_led_on(RGB_DOWN);
    } else {
        rgb_led_off(RGB_DOWN);
    }

    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10)) {
        rgb_colour_on(RGB_RED);
        rgb_colour_on(RGB_GREEN);
        rgb_colour_on(RGB_BLUE);
        rgb_led_on(RGB_RIGHT);
    } else {
        rgb_led_off(RGB_RIGHT);
    }

    if (!(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13))) {
        rgb_colour_on(RGB_RED);
        rgb_colour_on(RGB_GREEN);
        rgb_colour_on(RGB_BLUE);
        rgb_led_on(RGB_LEFT);
    } else {
        rgb_led_off(RGB_LEFT);
    }
}
