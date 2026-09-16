/*
 * 006spi_tx_only_arduino.c
 */

#include<string.h>
#include "stm32f446xx.h"

static void delay(void)
{
    for (volatile uint32_t i = 0; i < 500000; i++);
}

/* PB12 --> SPI2_NSS
 * PB13 --> SPI2_SCLK
 * PB14 --> SPI2_MISO
 * PB15 --> SPI2_MOSI
 * ALT function mode: 5
 */

void SPI2_GPIOInits(void)
{
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_Pin_Config.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_Pin_Config.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_Pin_Config.GPIO_PinOPtype = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_Pin_Config.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_Pin_Config.GPIO_PinSpeed = GPIO_SPEED_FAST;

	// SCLK
	SPIPins.GPIO_Pin_Config.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	// MOSI
	SPIPins.GPIO_Pin_Config.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	// MISO
	// SPIPins.GPIO_Pin_Config.GPIO_PinNumber = GPIO_PIN_NO_14;
	// GPIO_Init(&SPIPins);

	// NSS
	SPIPins.GPIO_Pin_Config.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);
}

void SPI2_Inits(void)
{
	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2; // SCLK = PCLK1 / 2
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DI; // Disable software NSS management

	SPI_Init(&SPI2handle);
}

void GPIO_ButtonInit(void)
{
	GPIO_Handle_t GpioBtn;

	GpioBtn.pGPIOx = GPIOC;
	GpioBtn.GPIO_Pin_Config.GPIO_PinNumber = GPIO_PIN_NO_13;
	GpioBtn.GPIO_Pin_Config.GPIO_PinMode = GPIO_MODE_IN;
	GpioBtn.GPIO_Pin_Config.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&GpioBtn);
}

int main(void)
{
	char user_data[] = "Hello world";

	GPIO_ButtonInit();

	// Function to initialize the GPIO pins to behave as SPI2 pins
	SPI2_GPIOInits();

	// Function to initialize the SPI2 peripheral parameters
	SPI2_Inits();

	// Enable hardware NSS output in master mode
	SPI_SSOEConfig(SPI2, ENABLE);

	while(1)
	{
	// Wait until button is pressed
	while( ! (GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13)) );

	// Button debounce delay
	delay();

	// Enable SPI2; hardware NSS goes LOW
	SPI_PeripheralControl(SPI2, ENABLE);

	// Send data
	SPI_SendData(SPI2, (uint8_t*)user_data, strlen(user_data));

	// Wait until SPI transmission is complete
	while (SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));

	// Disable SPI2; hardware NSS goes HIGH
	SPI_PeripheralControl(SPI2, DISABLE);

	// Wait until button is released
	while (GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13));
	}
	return 0;
}
