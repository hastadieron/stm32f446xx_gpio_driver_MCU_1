/*
 * stm32f446xx_spi_driver.h
 */

#ifndef INC_STM32F446XX_SPI_DRIVER_H_
#define INC_STM32F446XX_SPI_DRIVER_H_

#include "stm32f446xx.h"

/*
 * Configuration structure for SPIx peripheral
 */
typedef struct
{
	uint8_t SPI_DeviceMode;		/*!< SPI device mode: Master or Slave */
	uint8_t SPI_BusConfig;		/*!< SPI bus configuration: Full-Duplex, Half-Duplex or Simplex Receive Only */
	uint8_t SPI_SclkSpeed;		/*!< SPI serial clock speed configuration using baud rate prescaler */
	uint8_t SPI_DFF;			/*!< SPI data frame format: 8-bit or 16-bit */
	uint8_t SPI_CPOL;			/*!< SPI clock polarity configuration */
	uint8_t SPI_CPHA;			/*!< SPI clock phase configuration */
	uint8_t SPI_SSM;			/*!< SPI software slave management configuration */
}SPI_Config_t;

/*
 * Handle structure for SPIx peripheral
 */
typedef struct
{
	SPI_RegDef_t *pSPIx;		/*!< Base address of the SPIx peripheral */
	SPI_Config_t SPIConfig;		/*!< SPI peripheral configuration settings */
}SPI_Handle_t;

/**********************************************************************************
 *                        APIs supported by this driver
 *        For more information about the APIs check the function definitions

 **********************************************************************************/
/*
 * Peripheral clock setup
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);


/*
 * Init and De- init
 */
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

/*
 * Data send and receive
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);

/*
 * IRQ Configuration and ISR handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);

/*
 * Other peripheral control APIs
 */



#endif /* INC_STM32F446XX_SPI_DRIVER_H_ */
