/*
 * stm32f446xx_spi_driver.c
 */

#include "stm32f446xx_spi_driver.h"

/************************************************************************************************
 * @fn				- SPI_PeriClockControl
 *
 * @brief			- Enables or disables the peripheral clock for the specified SPI peripheral
 *
 * @param[in]		- pSPIx : Pointer to the SPI peripheral register structure (SPI1, SPI2, SPI3, or SPI4)
 * @param[in]		- EnorDi : ENABLE or DISABLE the peripheral clock
 *
 * @return			- None
 *
 * @note			- The SPI peripheral clock must be enabled before accessing the SPI registers
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi) {
	if (EnorDi == ENABLE) {
			if (pSPIx == SPI1) {
				SPI1_PCLK_EN();
			} else if (pSPIx == SPI2) {
				SPI2_PCLK_EN();
			} else if (pSPIx == SPI3) {
				SPI3_PCLK_EN();
			} else if (pSPIx == SPI4) {
				SPI4_PCLK_EN();
			}
		} else {
			if (pSPIx == SPI1) {
				SPI1_PCLK_DI();
			} else if (pSPIx == SPI2) {
				SPI2_PCLK_DI();
			} else if (pSPIx == SPI3) {
				SPI3_PCLK_DI();
			} else if (pSPIx == SPI4) {
				SPI4_PCLK_DI();
			}
	}
}

/************************************************************************************************
 * @fn				- SPI_Init
 *
 * @brief			- Initializes the SPI peripheral with the specified configuration
 *
 * @param[in]		- pSPIHandle : Pointer to the SPI handle structure
 *
 * @return			- None
 *
 * @note			- This function configures the SPI_CR1 register according
 *                    to the settings specified in the SPI handle.
 */
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	// Configure the SPI_CR1 register
	uint32_t tempreg = 0;

	// 1. Configure the device mode (Master/Slave)
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << 2;

	// 2. Configure the SPI bus configuration
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		// Select 2-line unidirectional data mode for Full-Duplex operation
		tempreg &= ~(1 << 15);

	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		// Select 1-line bidirectional data mode for Half-Duplex operation
		tempreg |= (1 << 15);

	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		// Select 2-line unidirectional data mode
		tempreg &= ~(1 << 15);

		// Enable receive-only mode
		tempreg |= (1 << 10);
	}

	// 3. Configure the SPI baud rate
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << 3;

	// 4. Configure the data frame format (8-bit/16-bit)
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << 11;

	// 5. Configure the clock polarity (CPOL)
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

	// 6. Configure the clock phase (CPHA)
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

	pSPIHandle->pSPIx->CR1 = tempreg;
}

/************************************************************************************************
 * @fn				- SPI_DeInit
 *
 * @brief			- Resets the specified SPI peripheral
 *
 * @param[in]		- pSPIx : Pointer to the SPI peripheral register definition
 *
 * @return			- None
 *
 * @note			- This function resets the SPI peripheral to its default reset state.
 */
void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
	// Reset the specified SPI peripheral
	if(pSPIx == SPI1)
	{
		SPI1_REG_RESET();
	}
	else if(pSPIx == SPI2)
	{
		SPI2_REG_RESET();
	}
	else if(pSPIx == SPI3)
	{
		SPI3_REG_RESET();
	}
	else if(pSPIx == SPI4)
	{
		SPI4_REG_RESET();
	}
}

/************************************************************************************************
 * @fn				- SPI_SendData
 *
 * @brief			-
 *
 * @param[in]		- pSPIx : Pointer to the SPI peripheral register definition
 * @param[in]		- pTxBuffer : Pointer to ........
 * @param[in]		- Len :  ........
 *
 * @return			- None
 *
 * @note			- This function resets the SPI peripheral to its default reset state.
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
	//
}
