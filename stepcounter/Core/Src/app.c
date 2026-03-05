#include "app.h"
#include "gpio.h"
#include "buttons.h"

void app_main(void) {

	buttons_update ();

//	buttons_checkButton (UP);
//	buttons_checkButton (UP);
//	buttons_checkButton (UP);
//	buttons_checkButton (UP);

    HAL_Delay(18);
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

    if (buttons_checkButton (LEFT) == PUSHED) {

        rgb_colour_all_on();
        rgb_led_on(RGB_LEFT);
    } else {
        rgb_led_off(RGB_LEFT);
    }

    if (buttons_checkButton (UP) == PUSHED) {

    	rgb_colour_all_on();

        rgb_led_on(RGB_UP);
    } else {
        rgb_led_off(RGB_UP);
    }

    if (buttons_checkButton (DOWN) == PUSHED) {

    	rgb_colour_all_on();
        rgb_led_on(RGB_DOWN);
    } else {
        rgb_led_off(RGB_DOWN);
    }

    if (buttons_checkButton (RIGHT) == PUSHED) {

    	rgb_colour_all_on();
        rgb_led_on(RGB_RIGHT);
    } else {
        rgb_led_off(RGB_RIGHT);
    }

//    if (buttons_checkButton (LEFT) == RELEASED) {
//    	rgb_color_all_on();
//        rgb_led_on(RGB_LEFT);
//    } else {
//        rgb_led_off(RGB_LEFT);
//    }
}
