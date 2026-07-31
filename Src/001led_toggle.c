/*
 * 001led_toggle.c
 */

#include <stdint.h>

#include "stm32f446xx.h"

/* Simple busy-wait delay (not calibrated, just for visible blink) */
static void delay(void)
{
    for (uint32_t i = 0; i < 500000; i++);
}

int main(void)
{
    GPIO_Handle_t GpioLed;
    GpioLed.pGPIOx = GPIOA;
    GpioLed.GPIO_Pin_Config.GPIO_PinNumber = GPIO_PIN_NO_5;
    GpioLed.GPIO_Pin_Config.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_Pin_Config.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed.GPIO_Pin_Config.GPIO_PinOPtype = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_Pin_Config.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOA, ENABLE);

	GPIO_Init(&GpioLed);
	for(;;)
	{
		GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5);
		delay();
	}
}
