/*
 * 002Led_btn.c
 */

#include <stdint.h>

#include "stm32f446xx.h"

#define BTN_PRESSED GPIO_PIN_RESET

/* Simple busy-wait delay (not calibrated, just for visible blink) */
static void delay(void)
{
    for (uint32_t i = 0; i < 500000; i++);
}

int main(void)
{
    GPIO_Handle_t GpioLed, GpioBtn;

    GpioLed.pGPIOx = GPIOA;
    GpioLed.GPIO_Pin_Config.GPIO_PinNumber = GPIO_PIN_NO_5;
    GpioLed.GPIO_Pin_Config.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_Pin_Config.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed.GPIO_Pin_Config.GPIO_PinOPtype = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_Pin_Config.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOA, ENABLE);

	GpioBtn.pGPIOx = GPIOC;
	GpioBtn.GPIO_Pin_Config.GPIO_PinNumber = GPIO_PIN_NO_13;
	GpioBtn.GPIO_Pin_Config.GPIO_PinMode = GPIO_MODE_IN;
	GpioBtn.GPIO_Pin_Config.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOC, ENABLE);

	GPIO_Init(&GpioLed);
	GPIO_Init(&GpioBtn);

	while(1)
	{
		if(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) == BTN_PRESSED)
		{
			delay();
			GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5);
		}else
		{
		    GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_5, GPIO_PIN_RESET);
		}
	}
}

