/*
 * task_display.c
 * Display task module - updates the OLED screen with joystick values
 * and optionally outputs debug info over UART
 * Authors: Ryan Teape, Felissa Chian
 * Date: 2026
 */

#include <stdio.h>
#include "task_joystick.h"

#include "task_display.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "ssd1306_conf.h"
#include "usart.h"
#include "app.h"
#include <string.h>

void display_task_execute(void) {

    char buffer[32];

    uint16_t x = getJoyStickX();
    uint16_t maxXValue = 3850;
    uint16_t minXValue = 470;

    uint16_t percentageXdisplacement = ((x - minXValue) * 100) / (maxXValue - minXValue);
    if (percentageXdisplacement > 100) {
    	percentageXdisplacement = 100;
    }

    char* xJoyDirection;

    if (percentageXdisplacement > 60) {
    	xJoyDirection = "Left";
    } else if (percentageXdisplacement < 40) {
    	xJoyDirection = "Right";
    } else {
    	xJoyDirection = "Rest";
    }

    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    snprintf(buffer, sizeof(buffer), "Joy X : %s %u", xJoyDirection , percentageXdisplacement);
    ssd1306_WriteString(buffer, Font_7x10, White);

    if (serialDebugFlag) {
        HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
        HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 100);
    }

    uint16_t y = getJoyStickY();
    uint16_t maxYValue = 3900;
    uint16_t minYValue = 335;

    uint16_t percentageYdisplacement = ((y - minYValue) * 100) / (maxYValue - minYValue);
    if (percentageYdisplacement > 100) {
        percentageYdisplacement = 100;
    }

    char* yJoyDirection;

    if (percentageYdisplacement > 60) {
        yJoyDirection = "Down";
    } else if (percentageYdisplacement < 40) {
        yJoyDirection = "Up";
    } else {
        yJoyDirection = "Rest";
    }

    ssd1306_SetCursor(0, 16);
    snprintf(buffer, sizeof(buffer), "Joy Y : %s %u", yJoyDirection , percentageYdisplacement);
    ssd1306_WriteString(buffer, Font_7x10, White);
    ssd1306_UpdateScreen();

    if (serialDebugFlag) {
        HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
        HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 100);
    }
}
