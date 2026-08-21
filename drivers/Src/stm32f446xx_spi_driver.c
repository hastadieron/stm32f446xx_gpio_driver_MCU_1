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
 * @brief			- Initializes the specified SPI with the given configuration
 *
 * @param[in]		- pSPIx : Pointer to the SPI ..................
 *
 * @return			- None
 *
 * @note			- TODO
 */
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	// Configure the SPI_CR1 register
	uint32_t tempreg = 0;

	// 1. Configure the device mode
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << 2;

	// 2. Configure the bus config
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		// BIDIMODE bit should be cleared
	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		// BIDIMODE bit should be set
	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		// BIDIMODE bit should be cleared
		// RXONLY bit must be set
	}

}

