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

char buffer[32];

void printInputToScreen(uint8_t line , char* inputStr , uint16_t valToPrint ) {

//	char buffer[32];

	ssd1306_SetCursor(0, (line*16));
	snprintf(buffer, sizeof(buffer), inputStr, valToPrint);
	ssd1306_WriteString(buffer, Font_7x10, White);


}

void display_task_execute(void) {


	ssd1306_Fill(Black);


    ssd1306_SetCursor(0, 0);
    snprintf(buffer, sizeof(buffer), "Joy X : %s %u", xJoyDirection , percentageXdisplacement);
    ssd1306_WriteString(buffer, Font_7x10, White);

    if (serialDebugFlag) {

    	snprintf(buffer, sizeof(buffer), "Joy X : %s %u", xJoyDirection , x);

        HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
        HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 100);
    }


    ssd1306_SetCursor(0, 16);
    snprintf(buffer, sizeof(buffer), "Joy Y : %s %u", yJoyDirection , percentageYdisplacement);
    ssd1306_WriteString(buffer, Font_7x10, White);
    ssd1306_UpdateScreen();


    printInputToScreen(2, "Step Goal: %u", stepGoal);

	ssd1306_SetCursor(0, 48);
	snprintf(buffer, sizeof(buffer), "steps:%u,dist:%u", steps, distance);
	ssd1306_WriteString(buffer, Font_7x10, White);
	ssd1306_UpdateScreen();



    if (serialDebugFlag) {

    	snprintf(buffer, sizeof(buffer), "Joy Y : %s %u", yJoyDirection , currYVal);
        HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
        HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 100);
    }
}
