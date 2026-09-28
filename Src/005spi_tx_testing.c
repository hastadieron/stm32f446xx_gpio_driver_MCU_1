/*
 * 005spi_tx_testing.c
 */

#include<string.h>
#include "stm32f446xx.h"

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
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV16; // SCLK = PCLK1 / 16 ≈ 2.8 MHz
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
//	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN; // software slave management enabled for NSS pin
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DI; // software slave management disabled for NSS pin

	SPI_Init(&SPI2handle);
}

int main(void)
{
	char user_data[] = "Hello world";

	// Function to initialize the GPIO pins to behave as SPI2 pins
	SPI2_GPIOInits();

	// Function to initialize the SPI2 peripheral parameters
	SPI2_Inits();

	// Enable hardware NSS output in Master mode
	SPI_SSOEConfig(SPI2, ENABLE);

	uint8_t dataLen = strlen(user_data);

	while(1)
	{
	// Enable the SPI2 peripheral
	SPI_PeripheralControl(SPI2, ENABLE);

	// Send payload length
	SPI_SendData(SPI2, &dataLen, 1);

	// Send payload
	SPI_SendData(SPI2, (uint8_t*)user_data, dataLen);

	// Wait until SPI transmission is complete
	while (SPI_Get_Flag_Status(SPI2, SPI_BUSY_FLAG));

	// Disable the SPI2 peripheral
	SPI_PeripheralControl(SPI2, DISABLE);

	for(volatile uint32_t i = 0; i < 500000; i++);
	}

	return 0;
}
