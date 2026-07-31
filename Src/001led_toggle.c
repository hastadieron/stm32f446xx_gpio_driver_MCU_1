/*
 * 001led_toggle.c
 */

#include <stdint.h>

#include "stm32f446xx.h"

int main(void)
{
    GPIO_Handle_t GpioLed;
    GpioLed.pGPIOx = GPIOA;
    GpioLed.GPIO_Pin_Config.GPIO_PinNumber = GPIO_PIN_NO_5;

	/* Loop forever */
	for(;;);
}
