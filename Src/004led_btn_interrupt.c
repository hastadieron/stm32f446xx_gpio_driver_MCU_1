/*
 * 004led_btn_interrupt.c
 *
 * External button: PA1, internal pull-up, button connects to GND
 * External LED:    PA0,  Push-Pull output, LED + resistor to GND
 *
 * Button logic: HIGH = idle (pull-up), LOW = pressed
 */

#include <stdint.h>
#include <string.h>

#include "stm32f446xx.h"

/* PA1 reads LOW when button is pressed */
#define BTN_PRESSED GPIO_PIN_RESET

/* Simple busy-wait delay ~200ms for debounce */
static void delay(void)
{
    for (uint32_t i = 0; i < 500000/2; i++);
}

int main(void)
{
    GPIO_Handle_t GpioLed, GpioBtn;
    memset(&GpioLed, 0, sizeof(GpioLed));
    memset(&GpioBtn, 0, sizeof(GpioBtn));


    GpioLed.pGPIOx = GPIOA;
    GpioLed.GPIO_Pin_Config.GPIO_PinNumber = GPIO_PIN_NO_5;
    GpioLed.GPIO_Pin_Config.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_Pin_Config.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed.GPIO_Pin_Config.GPIO_PinOPtype = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_Pin_Config.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOA, ENABLE);
	GPIO_Init(&GpioLed);

	GpioBtn.pGPIOx                              = GPIOD;
	GpioBtn.GPIO_Pin_Config.GPIO_PinNumber      = GPIO_PIN_NO_5;
	GpioBtn.GPIO_Pin_Config.GPIO_PinMode        = GPIO_MODE_IT_FT;
	GpioBtn.GPIO_Pin_Config.GPIO_PinSpeed       = GPIO_SPEED_FAST;
	GpioBtn.GPIO_Pin_Config.GPIO_PinPuPdControl = GPIO_PIN_PU;

	GPIO_PeriClockControl(GPIOD, ENABLE);
	GPIO_Init(&GpioBtn);

	// IRQ configurations
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI9_5, NVIC_IRQ_PRI15);
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI9_5, ENABLE);

	while(1);
}

void EXTI9_5_IRQHandler(void)
{
	delay();
	GPIO_IRQHandling(GPIO_PIN_NO_5);			// clear the pending event from EXTI line
	GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5);
}
