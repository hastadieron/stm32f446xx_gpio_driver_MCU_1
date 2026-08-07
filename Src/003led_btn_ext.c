/*
 * 003led_btn_ext.c
 *
 * External button: PA1, internal pull-up, button connects to GND
 * External LED:    PA0,  Push-Pull output, LED + resistor to GND
 *
 * Button logic: HIGH = idle (pull-up), LOW = pressed
 */

#include <stdint.h>

#include "stm32f446xx.h"

/* PA1 reads LOW when button is pressed */
#define BTN_PRESSED GPIO_PIN_RESET

/* Simple busy-wait delay for debounce */
static void delay(void)
{
    for (uint32_t i = 0; i < 500000; i++);
}

int main(void)
{
    GPIO_Handle_t GpioLed, GpioBtn;

    GpioLed.pGPIOx = GPIOA;
    GpioLed.GPIO_Pin_Config.GPIO_PinNumber = GPIO_PIN_NO_0;
    GpioLed.GPIO_Pin_Config.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_Pin_Config.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed.GPIO_Pin_Config.GPIO_PinOPtype = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_Pin_Config.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GpioBtn.pGPIOx                              = GPIOA;
	GpioBtn.GPIO_Pin_Config.GPIO_PinNumber      = GPIO_PIN_NO_1;
	GpioBtn.GPIO_Pin_Config.GPIO_PinMode        = GPIO_MODE_IN;
	GpioBtn.GPIO_Pin_Config.GPIO_PinPuPdControl = GPIO_PIN_PU;

	GPIO_PeriClockControl(GPIOA, ENABLE);

	GPIO_Init(&GpioLed);
	GPIO_Init(&GpioBtn);

	while(1)
	{
		if(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_1) == BTN_PRESSED)
		{
			delay();
			GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_0);
		}else
		{
		    GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_0, GPIO_PIN_RESET);
		}
	}
}

