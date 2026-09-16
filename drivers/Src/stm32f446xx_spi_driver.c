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
 *                    SPI_PeriClockControl() must be called BEFORE this function to enable
 *                    the peripheral clock. This function only configures CR1 — it does NOT
 *                    set the SPE (SPI Enable) bit; SPE must be set separately before
 *                    calling SPI_SendData().
 */
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	// Peripheral clock enable
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

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

	// 7. Configure the software slave management (SSM)
	if(pSPIHandle->SPIConfig.SPI_SSM == SPI_SSM_EN)
	{
		tempreg |= (1 << SPI_CR1_SSM);
	// Force internal NSS high so the master never sees a Mode Fault (MODF),
	// since the physical NSS pin is intentionally left unconfigured.
		tempreg |= (1 << SPI_CR1_SSI);
	}
	else
	{
		tempreg &= ~(1 << SPI_CR1_SSM);
	}

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
 * @fn				- SPI_SSIConfig
 *
 * @brief			- Configures the internal NSS level when software slave management is enabled
 *
 * @param[in]		- pSPIx : Pointer to the SPI peripheral register definition
 * @param[in]		- EnOrDi : ENABLE sets the SSI bit; DISABLE clears the SSI bit
 *
 * @return			- None
 *
 * @note			- SSI is used only when SSM = 1. It controls the internal NSS
 *                	  signal seen by the SPI peripheral and does not control the
 *                	  physical NSS GPIO pin.
 */
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	}else{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
	}
}

/************************************************************************************************
 * @fn				- SPI_SSOEConfig
 *
 * @brief			- Enables or disables the hardware NSS output in Master mode
 *
 * @param[in]		- pSPIx : Pointer to the SPI peripheral register definition
 * @param[in]		- EnOrDi : ENABLE sets the SSOE bit; DISABLE clears the SSOE bit
 *
 * @return			- None
 *
 * @note			- SSOE controls the NSS output when the SPI peripheral operates
 *                	  in Master mode with hardware NSS management. When SSOE = 1,
 *                	  enabling SPI (SPE = 1) drives the NSS output low, and disabling
 *                	  SPI (SPE = 0) releases the NSS output high.
 */
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR2 |= (1 << SPI_CR2_SSOE);
	}else{
		pSPIx->CR2 &= ~(1 << SPI_CR2_SSOE);
	}
}

/************************************************************************************************
 * @fn				- SPI_GetFlagStatus
 *
 * @brief			- Checks whether a given flag bit is set in the SPI_SR (Status Register)
 *
 * @param[in]		- pSPIx    : Pointer to the SPI peripheral register definition
 * @param[in]		- FlagName : Bitmask of the flag to check (e.g. SPI_TXE_FLAG, SPI_RXNE_FLAG, SPI_BSY_FLAG)
 *
 * @return			- FLAG_SET (1) if the flag bit is set, FLAG_RESET (0) otherwise
 *
 * @note			- None
 */
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint8_t FlagName)
{
	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/************************************************************************************************
 * @fn          - SPI_SendData
 *
 * @brief       - Sends data over SPI in blocking (polling) mode. The function
 *                does not return until all requested data has been written
 *                to the SPI data register.
 *
 * @param[in]   - pSPIx     : Pointer to the SPI peripheral register definition
 * @param[in]   - pTxBuffer : Pointer to the user data buffer (in SRAM) to be transmitted
 * @param[in]   - Len       : Number of bytes to transmit
 *
 * @return      - None
 *
 * @note        - This is a blocking call (polling-based). It busy-waits on the
 *                TXE flag before every write to DR.
 *                The function does not wait for BSY = 0.
 *                In 16-bit DFF mode, Len must be a multiple of 2.
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
    while (Len > 0)
    {
        // Wait until the Tx buffer is empty (TXE = 1)
        while (SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);

        // Check the DFF bit in CR1
        if (pSPIx->CR1 & (1 << SPI_CR1_DFF))
        {
            // Write 16 bits from the Tx buffer to DR
            pSPIx->DR = *((uint16_t*)pTxBuffer);

            Len -= 2;
            pTxBuffer += 2;
        }
        else
        {
            // Write 8 bits from the Tx buffer to DR
            pSPIx->DR = *pTxBuffer;

            Len--;
            pTxBuffer++;
        }
    }
}

/************************************************************************************************
 * @fn          - SPI_ReceiveData
 *
 * @brief       - Receives data over SPI in blocking (polling) mode. The function
 *                does not return until the requested number of bytes has been
 *                read from the SPI receive buffer.
 *
 * @param[in]   - pSPIx     : Pointer to the SPI peripheral register definition
 * @param[out]  - pRxBuffer : Pointer to the user buffer (in SRAM) where received data is stored
 * @param[in]   - Len       : Number of bytes to receive
 *
 * @return      - None
 *
 * @note        - This is a blocking call (polling-based). It busy-waits on the
 *                RXNE flag until received data is available.
 *                In 16-bit DFF mode, Len must be a multiple of 2.
 */
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{
    while (Len > 0)
    {
        // Wait until the Rx buffer is not empty (RXNE = 1)
        while (SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);

        // Check the DFF bit in CR1
        if (pSPIx->CR1 & (1 << SPI_CR1_DFF))
        {
            // Read 16 bits from DR into the Rx buffer
            *((uint16_t*)pRxBuffer) = pSPIx->DR;

            Len -= 2;
            pRxBuffer += 2;
        }
        else
        {
            // Read 8 bits from DR into the Rx buffer
            *pRxBuffer = pSPIx->DR;

            Len--;
            pRxBuffer++;
        }
    }
}

/************************************************************************************************
 * @fn          - SPI_PeripheralControl
 *
 * @brief       - Enables or disables the SPI peripheral
 *
 * @param[in]   - pSPIx  : Pointer to the SPI peripheral register definition
 * @param[in]   - EnOrDi : ENABLE sets the SPE bit; DISABLE clears the SPE bit
 *
 * @return      - None
 *
 * @note        - This function controls the SPE bit in SPI_CR1.
 *                Before disabling SPI, ensure that the current transmission
 *                is complete (BSY = 0).
 */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
    if (EnOrDi == ENABLE)
    {
        pSPIx->CR1 |= (1 << SPI_CR1_SPE);
    }
    else
    {
        pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
    }
}
