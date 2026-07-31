/*
 * stm32f446xx.h
 */
#ifndef INC_STM32F446XX_H_
#define INC_STM32F446XX_H_

#include<stdint.h>

/*
 * Base addresses of Flash and SRAM memory
 */

#define FLASH_BASEADDR     0x08000000U		/* Base address of internal Flash memory	*/
#define SRAM1_BASEADDR     0x20000000U		/* Base address of internal SRAM1 (112 KB)	*/
#define SRAM               SRAM1_BASEADDR	/* Alias for the beginning of internal SRAM	*/
#define SRAM2_BASEADDR     0x2001C000U		/* Base address of internal SRAM2 (16 KB)	*/
#define ROM                0x1FFF0000U		/* Base address of System Memory (Boot ROM)	*/

/*
 * AHBx and APBx Bus Peripheral base addresses
 */

#define PERIPH_BASEADDR			0x40000000U
#define APB1PERIPH_BASEADDR		PERIPH_BASEADDR
#define APB2PERIPH_BASEADDR		0x40010000U
#define AHB1PERIPH_BASEADDR		0x40020000U
#define AHB2PERIPH_BASEADDR		0x50000000U

/*
 * Base addresses of peripherals connected to AHB1 bus
 */

#define GPIOA_BASEADDR (AHB1PERIPH_BASEADDR + 0x0000U)
#define GPIOB_BASEADDR (AHB1PERIPH_BASEADDR + 0x0400U)
#define GPIOC_BASEADDR (AHB1PERIPH_BASEADDR + 0x0800U)
#define GPIOD_BASEADDR (AHB1PERIPH_BASEADDR + 0x0C00U)
#define GPIOE_BASEADDR (AHB1PERIPH_BASEADDR + 0x1000U)
#define GPIOF_BASEADDR (AHB1PERIPH_BASEADDR + 0x1400U)
#define GPIOG_BASEADDR (AHB1PERIPH_BASEADDR + 0x1800U)
#define GPIOH_BASEADDR (AHB1PERIPH_BASEADDR + 0x1C00U)

#define RCC_BASEADDR   (AHB1PERIPH_BASEADDR + 0x3800U)

/*
 * Base addresses of peripherals connected to APB1 bus
 */

#define I2C1_BASEADDR		(APB1PERIPH_BASEADDR + 0x5400U)
#define I2C2_BASEADDR		(APB1PERIPH_BASEADDR + 0x5800U)
#define I2C3_BASEADDR		(APB1PERIPH_BASEADDR + 0x5C00U)

#define SPI2_BASEADDR		(APB1PERIPH_BASEADDR + 0x3800U)
#define SPI3_BASEADDR		(APB1PERIPH_BASEADDR + 0x3C00U)

#define USART2_BASEADDR		(APB1PERIPH_BASEADDR + 0x4400U)
#define USART3_BASEADDR		(APB1PERIPH_BASEADDR + 0x4800U)
#define UART4_BASEADDR		(APB1PERIPH_BASEADDR + 0x4C00U)
#define UART5_BASEADDR		(APB1PERIPH_BASEADDR + 0x5000U)

/*
 * Base addresses of peripherals connected to APB2 bus
 */

#define EXTI_BASEADDR		(APB2PERIPH_BASEADDR + 0x3C00U)
#define SPI1_BASEADDR		(APB2PERIPH_BASEADDR + 0x3000U)
#define SPI4_BASEADDR		(APB2PERIPH_BASEADDR + 0x3400U)
#define SYSCFG_BASEADDR		(APB2PERIPH_BASEADDR + 0x3800U)
#define USART1_BASEADDR		(APB2PERIPH_BASEADDR + 0x1000U)
#define USART6_BASEADDR		(APB2PERIPH_BASEADDR + 0x1400U)

/*****peripheral register definition structure*****/
typedef struct
{
	volatile uint32_t MODER;		/* GPIO port mode register, Address offset 0x00 						*/
	volatile uint32_t OTYPER;		/* GPIO port output type register, Address offset 0x04					*/
	volatile uint32_t OSPEEDR;		/* GPIO port output speed register, Address offset 0x08  				*/
	volatile uint32_t PUPDR;		/* GPIO port pull-up/pull-down register, address offset 0x0C 			*/
	volatile uint32_t IDR;			/* GPIO port input data register, address offset 0x10					*/
	volatile uint32_t ODR;			/* GPIO port output data register, address offset 0x14 					*/
	volatile uint32_t BSRR;			/* GPIO port bit set/reset register, address offset 0x18				*/
	volatile uint32_t LCKR;			/* GPIO port configuration lock register, address offset 0x1C			*/
	volatile uint32_t AFR[2];		/* AFR[0] GPIO alternate function low register, address offset 0x20	*/
									/* AFR[1] GPIO alternate function high register, address offset 0x24	*/

} GPIO_RegDef_t;

/*****peripheral register definition structure for RCC*****/
typedef struct
{
	volatile uint32_t CR;			/* RCC clock control register 					address offset 0x00 */
	volatile uint32_t PLLCFGR;		/* RCC PLL configuration register 				address offset 0x04 */
	volatile uint32_t CFGR;			/* RCC clock configuration register 			address offset 0x08 */
	volatile uint32_t CIR;			/* RCC clock interrupt register					address offset 0x0C */
	volatile uint32_t AHB1RSTR;		/* RCC AHB1 peripheral reset register			address offset 0x10 */
	volatile uint32_t AHB2RSTR;		/* RCC AHB2 peripheral reset register 			address offset 0x14	*/
	volatile uint32_t AHB3RSTR;		/* RCC AHB3 peripheral reset register 			address offset 0x18	*/
	volatile uint32_t RESERVED0;	/* Reserved 									address offset 0x1C */
	volatile uint32_t APB1RSTR;		/* RCC APB1 peripheral reset register			address offset 0x20 */
	volatile uint32_t APB2RSTR;		/* RCC APB2 peripheral reset register			address offset 0x24	*/
	volatile uint32_t RESERVED1[2];	/* Reserved 									address offset 0x28 */
	 	 	 	 	 	 	 	 	/* Reserved 									address offset 0x2C */
	volatile uint32_t AHB1ENR;		/* RCC AHB1 peripheral clock enable register	address offset 0x30 */
	volatile uint32_t AHB2ENR;		/* RCC AHB2 peripheral clock enable register	address offset 0x34 */
	volatile uint32_t AHB3ENR;		/* RCC AHB3 peripheral clock enable register	address offset 0x38 */
	volatile uint32_t RESERVED2;	/* Reserved 									address offset 0x3C */
	volatile uint32_t APB1ENR;		/* RCC APB1 peripheral clock enable register	address offset 0x40 */
	volatile uint32_t APB2ENR;		/* RCC APB2 peripheral clock enable register	address offset 0x44 */
	volatile uint32_t RESERVED3[2];	/* Reserved 									address offset 0x48 */
 	 	 	 	 	 	 			/* Reserved 									address offset 0x4C */
 	volatile uint32_t AHB1LPENR;	/* RCC AHB1 peripheral clock enable in low power mode register address offset 0x50 */
	volatile uint32_t AHB2LPENR;	/* RCC AHB2 peripheral clock enable in low power mode register address offset 0x54 */
	volatile uint32_t AHB3LPENR;	/* RCC AHB3 peripheral clock enable in low power mode register address offset 0x58 */
	volatile uint32_t RESERVED4;	/* Reserved 									address offset 0x5C */
	volatile uint32_t APB1LPENR;	/* RCC APB1 peripheral clock enable in low power mode register address offset 0x60 */
	volatile uint32_t APB2LPENR;	/* RCC APB2 peripheral clock enable in low power mode register address offset 0x64 */
	volatile uint32_t RESERVED5[2];	/* Reserved 									address offset 0x68 */
	 	 	 	 	 	 	 		/* Reserved 									address offset 0x6C */
	volatile uint32_t BDCR;			/* RCC Backup domain control register			address offset 0x70 */
	volatile uint32_t CSR;			/* RCC clock control and status register		address offset 0x74 */
	volatile uint32_t RESERVED6[2];	/* Reserved 									address offset 0x78 */
		 	 	 	 	 	 	 	/* Reserved 									address offset 0x7C */
	volatile uint32_t SSCGR;		/* RCC spread spectrum clock generation register address offset 0x80 */
	volatile uint32_t PLLI2SCFGR;	/* RCC PLLI2S configuration register 			address offset 0x84 */
	volatile uint32_t PLLSAICFGR;	/* RCC PLLSAI configuration register			address offset 0x88 */
	volatile uint32_t DCKCFGR;		/* RCC dedicated clock configuration register	address offset 0x8C */
	volatile uint32_t CKGATENR;		/* RCC clocks gated enable register 			address offset 0x90 */
	volatile uint32_t DCKCFGR2;		/* RCC dedicated clocks configuration register 2 address offset 0x94 */
} RCC_RegDef_t;

/*
 * Peripheral definitions (Peripheral base addresses typecasted to xxxRegDef_t)
 */
#define GPIOA ((GPIO_RegDef_t*) GPIOA_BASEADDR)
#define GPIOB ((GPIO_RegDef_t*) GPIOB_BASEADDR)
#define GPIOC ((GPIO_RegDef_t*) GPIOC_BASEADDR)
#define GPIOD ((GPIO_RegDef_t*) GPIOD_BASEADDR)
#define GPIOE ((GPIO_RegDef_t*) GPIOE_BASEADDR)
#define GPIOF ((GPIO_RegDef_t*) GPIOF_BASEADDR)
#define GPIOG ((GPIO_RegDef_t*) GPIOG_BASEADDR)
#define GPIOH ((GPIO_RegDef_t*) GPIOH_BASEADDR)

#define RCC ((RCC_RegDef_t*) RCC_BASEADDR)

/*
 * Clock Enable Macros for GPIOx peripherals
 */
#define GPIOA_PCLK_EN()	(RCC->AHB1ENR |= (1U << 0))
#define GPIOB_PCLK_EN()	(RCC->AHB1ENR |= (1U << 1))
#define GPIOC_PCLK_EN()	(RCC->AHB1ENR |= (1U << 2))
#define GPIOD_PCLK_EN()	(RCC->AHB1ENR |= (1U << 3))
#define GPIOE_PCLK_EN()	(RCC->AHB1ENR |= (1U << 4))
#define GPIOF_PCLK_EN()	(RCC->AHB1ENR |= (1U << 5))
#define GPIOG_PCLK_EN()	(RCC->AHB1ENR |= (1U << 6))
#define GPIOH_PCLK_EN()	(RCC->AHB1ENR |= (1U << 7))

/*
 * Clock Enable Macros for I2Cx peripherals
 */

#define I2C1_PCLK_EN()	(RCC->APB1ENR |= (1U << 21))
#define I2C2_PCLK_EN()	(RCC->APB1ENR |= (1U << 22))
#define I2C3_PCLK_EN()	(RCC->APB1ENR |= (1U << 23))

/*
 * Clock Enable Macros for SPIx peripherals
 */
#define SPI1_PCLK_EN()	(RCC->APB2ENR |= (1U << 12))
#define SPI2_PCLK_EN()	(RCC->APB1ENR |= (1U << 14))
#define SPI3_PCLK_EN()	(RCC->APB1ENR |= (1U << 15))
#define SPI4_PCLK_EN()	(RCC->APB2ENR |= (1U << 13))

/*
 * Clock Enable Macros for USARTx peripherals
 */
#define USART1_PCLK_EN()	(RCC->APB2ENR |= (1U << 4))
#define USART2_PCLK_EN()	(RCC->APB1ENR |= (1U << 17))
#define USART3_PCLK_EN()	(RCC->APB1ENR |= (1U << 18))
#define UART4_PCLK_EN()		(RCC->APB1ENR |= (1U << 19))
#define UART5_PCLK_EN()		(RCC->APB1ENR |= (1U << 20))
#define USART6_PCLK_EN()	(RCC->APB2ENR |= (1U << 5))

/*
 * Clock Enable Macros for SYSCFG peripheral
 */
#define SYSCFG_PCLK_EN() (RCC->APB2ENR |= (1U << 14))

/*
 * Clock Disable Macros for GPIOx peripherals
 */
#define GPIOA_PCLK_DI()	(RCC->AHB1ENR &= ~(1U << 0))
#define GPIOB_PCLK_DI()	(RCC->AHB1ENR &= ~(1U << 1))
#define GPIOC_PCLK_DI()	(RCC->AHB1ENR &= ~(1U << 2))
#define GPIOD_PCLK_DI()	(RCC->AHB1ENR &= ~(1U << 3))
#define GPIOE_PCLK_DI()	(RCC->AHB1ENR &= ~(1U << 4))
#define GPIOF_PCLK_DI()	(RCC->AHB1ENR &= ~(1U << 5))
#define GPIOG_PCLK_DI()	(RCC->AHB1ENR &= ~(1U << 6))
#define GPIOH_PCLK_DI()	(RCC->AHB1ENR &= ~(1U << 7))

/*
 * Clock Disable Macros for I2Cx peripherals
 */
#define I2C1_PCLK_DI()	(RCC->APB1ENR &= ~(1U << 21))
#define I2C2_PCLK_DI()	(RCC->APB1ENR &= ~(1U << 22))
#define I2C3_PCLK_DI()	(RCC->APB1ENR &= ~(1U << 23))

/*
 * Clock Disable Macros for SPIx peripherals
 */
#define SPI1_PCLK_DI()	(RCC->APB2ENR &= ~(1U << 12))
#define SPI2_PCLK_DI()	(RCC->APB1ENR &= ~(1U << 14))
#define SPI3_PCLK_DI()	(RCC->APB1ENR &= ~(1U << 15))
#define SPI4_PCLK_DI()	(RCC->APB2ENR &= ~(1U << 13))

/*
 * Clock Disable Macros for USARTx peripherals
 */
#define USART1_PCLK_DI()	(RCC->APB2ENR &= ~(1U << 4))
#define USART2_PCLK_DI()	(RCC->APB1ENR &= ~(1U << 17))
#define USART3_PCLK_DI()	(RCC->APB1ENR &= ~(1U << 18))
#define UART4_PCLK_DI()		(RCC->APB1ENR &= ~(1U << 19))
#define UART5_PCLK_DI()		(RCC->APB1ENR &= ~(1U << 20))
#define USART6_PCLK_DI()	(RCC->APB2ENR &= ~(1U << 5))

/*
 * Clock Disable Macros for SYSCFG peripheral
 */
#define SYSCFG_PCLK_DI() (RCC->APB2ENR &= ~(1U << 14))

/*
 * Macros to reset GPIOx peripheral
 */
#define GPIOA_REG_RESET()		do { (RCC->AHB1RSTR |= (1 << 0)); (RCC->AHB1RSTR &= ~(1 << 0)); } while (0)
#define GPIOB_REG_RESET()		do { (RCC->AHB1RSTR |= (1 << 1)); (RCC->AHB1RSTR &= ~(1 << 1)); } while (0)
#define GPIOC_REG_RESET()		do { (RCC->AHB1RSTR |= (1 << 2)); (RCC->AHB1RSTR &= ~(1 << 2)); } while (0)
#define GPIOD_REG_RESET()		do { (RCC->AHB1RSTR |= (1 << 3)); (RCC->AHB1RSTR &= ~(1 << 3)); } while (0)
#define GPIOE_REG_RESET()		do { (RCC->AHB1RSTR |= (1 << 4)); (RCC->AHB1RSTR &= ~(1 << 4)); } while (0)
#define GPIOF_REG_RESET()		do { (RCC->AHB1RSTR |= (1 << 5)); (RCC->AHB1RSTR &= ~(1 << 5)); } while (0)
#define GPIOG_REG_RESET()		do { (RCC->AHB1RSTR |= (1 << 6)); (RCC->AHB1RSTR &= ~(1 << 6)); } while (0)
#define GPIOH_REG_RESET()		do { (RCC->AHB1RSTR |= (1 << 7)); (RCC->AHB1RSTR &= ~(1 << 7)); } while (0)

// some generic macros
#define ENABLE			1
#define DISABLE			0
#define SET				ENABLE
#define RESET			DISABLE
#define GPIO_PIN_SET	SET
#define GPIO_PIN_RESET	RESET

#include "stm32f446xx_gpio_driver.h"

#endif /* INC_STM32F446XX_H_ */
