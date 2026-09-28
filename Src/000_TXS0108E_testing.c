#include "stm32f446xx.h"

void SPI2_GPIOInits(void)
{
    GPIO_Handle_t SPIPins;

    SPIPins.pGPIOx = GPIOB;

    SPIPins.GPIO_Pin_Config.GPIO_PinMode = GPIO_MODE_ALTFN;
    SPIPins.GPIO_Pin_Config.GPIO_PinAltFunMode = 5;
    SPIPins.GPIO_Pin_Config.GPIO_PinOPtype = GPIO_OP_TYPE_PP;
    SPIPins.GPIO_Pin_Config.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    SPIPins.GPIO_Pin_Config.GPIO_PinSpeed = GPIO_SPEED_FAST;

    // PB13 = SPI2_SCK
    SPIPins.GPIO_Pin_Config.GPIO_PinNumber = GPIO_PIN_NO_13;
    GPIO_Init(&SPIPins);
}

void SPI2_Inits(void)
{
    SPI_Handle_t SPI2handle;

    SPI2handle.pSPIx = SPI2;

    SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
    SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
    SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;
    SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
    SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
    SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
    SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN;

    SPI_Init(&SPI2handle);
}

int main(void)
{
    uint8_t data[100];

    for(uint8_t i = 0; i < 100; i++)
    {
        data[i] = 0xAA;
    }

    SPI2_GPIOInits();
    SPI2_Inits();

    SPI_PeripheralControl(SPI2, ENABLE);

    while(1)
    {
        SPI_SendData(SPI2, data, 100);
    }
}
