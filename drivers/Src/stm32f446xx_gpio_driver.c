/*
 * stm32f446xx_gpio_driver.c
 */

#include "stm32f446xx_gpio_driver.h"

/************************************************************************************************
 * @fn				- GPIO_PeriClockControl
 *
 * @brief			- Enables or disables the peripheral clock for the specified GPIO port
 *
 * @param[in]		- pGPIOx : Pointer to the GPIO peripheral register structure
 * @param[in]		- EnorDi : ENABLE or DISABLE
 *
 * @return			- None
 *
 * @note			- The GPIO peripheral clock must be enabled before accessing the GPIO registers
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi) {
	if (EnorDi == ENABLE) {
		if (pGPIOx == GPIOA) {
			GPIOA_PCLK_EN();
		} else if (pGPIOx == GPIOB) {
			GPIOB_PCLK_EN();
		} else if (pGPIOx == GPIOC) {
			GPIOC_PCLK_EN();
		} else if (pGPIOx == GPIOD) {
			GPIOD_PCLK_EN();
		} else if (pGPIOx == GPIOE) {
			GPIOE_PCLK_EN();
		} else if (pGPIOx == GPIOF) {
			GPIOF_PCLK_EN();
		} else if (pGPIOx == GPIOG) {
			GPIOG_PCLK_EN();
		} else if (pGPIOx == GPIOH) {
			GPIOH_PCLK_EN();
		}
	} else {
		if (pGPIOx == GPIOA) {
			GPIOA_PCLK_DI();
		} else if (pGPIOx == GPIOB) {
			GPIOB_PCLK_DI();
		} else if (pGPIOx == GPIOC) {
			GPIOC_PCLK_DI();
		} else if (pGPIOx == GPIOD) {
			GPIOD_PCLK_DI();
		} else if (pGPIOx == GPIOE) {
			GPIOE_PCLK_DI();
		} else if (pGPIOx == GPIOF) {
			GPIOF_PCLK_DI();
		} else if (pGPIOx == GPIOG) {
			GPIOG_PCLK_DI();
		} else if (pGPIOx == GPIOH) {
			GPIOH_PCLK_DI();
		}
	}
}

/**************************************************************************************
 * @fn				- GPIO_Init
 *
 * @brief			- Initializes the specified GPIO pin with the given configuration
 *
 * @param[in]		- pGPIOHandle : Pointer to the GPIO handle structure containing
 * 									the GPIO port and pin configuration
 *
 * @return			- None
 *
 * @note			- Configures the following GPIO settings:
 *                    - Pin mode
 *                    - Output speed
 *                    - Pull-up/Pull-down
 *                    - Output type
 *                    - Alternate function (if selected)
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	uint32_t temp = 0; //temp.register

	// Enable the peripheral clock
	GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);

	// 1. Configure the mode of GPIO pin
	if (pGPIOHandle->GPIO_Pin_Config.GPIO_PinMode <= GPIO_MODE_ANALOG) {
		// the non interrupt mode
		temp = (pGPIOHandle->GPIO_Pin_Config.GPIO_PinMode
				<< (2 * pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER &= ~(0x3
				<< (2 * pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber)); // clear
		pGPIOHandle->pGPIOx->MODER |= temp; // set
		temp = 0;

	} else {
		// interrupt mode
		if (pGPIOHandle->GPIO_Pin_Config.GPIO_PinMode == GPIO_MODE_IT_FT)
		{
			// 1. Configure  Falling trigger selection register (EXTI_FTSR)
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber);
			// Clear the corresponding RTSR bit
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber);

		} else if (pGPIOHandle->GPIO_Pin_Config.GPIO_PinMode == GPIO_MODE_IT_RT)
		{
			// 1. Configure  Rising trigger selection register (EXTI_FTSR)
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber);
			// Clear the corresponding FTSR bit
			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber);

		} else if (pGPIOHandle->GPIO_Pin_Config.GPIO_PinMode == GPIO_MODE_IT_RFT)
		{
			// 1. Configure both EXTI_FTSR and EXTI_RTSR
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber);
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber);
		}

		// 2. Configure the GPIO port selection in SYSCFG_EXTICR
		uint8_t temp1 = pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber / 4;
		uint8_t temp2 = pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber % 4;
		uint8_t portcode = GPIOA_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[temp1] = portcode << (temp2 * 4);

		// 3. Enable the EXTI interrupt delivery using IMR
		EXTI->IMR |= (1 << pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber);
	}
	temp = 0;

	// 2. Configure the speed
	temp = (pGPIOHandle->GPIO_Pin_Config.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << (2 * pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber)); // clear
	pGPIOHandle->pGPIOx->OSPEEDR |= temp; // set

	temp = 0;

	// 3. Configure pull-up / pull-down settings
	temp = (pGPIOHandle->GPIO_Pin_Config.GPIO_PinPuPdControl << (2 * pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << (2 * pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber)); // clear
	pGPIOHandle->pGPIOx->PUPDR |= temp; // set

	temp = 0;

	// 4. Configure the output type
	temp = (pGPIOHandle->GPIO_Pin_Config.GPIO_PinOPtype << pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber); // clear
	pGPIOHandle->pGPIOx->OTYPER |= temp; // set

	// 5. Configure the alternate functionality
	if (pGPIOHandle->GPIO_Pin_Config.GPIO_PinMode == GPIO_MODE_ALTFN) {
		// configure the alternate function registers
		uint32_t temp1, temp2;
		temp1 = pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber / 8;
		temp2 = pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber % 8;
		pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF << (4 * temp2));
		pGPIOHandle->pGPIOx->AFR[temp1] |=
				(pGPIOHandle->GPIO_Pin_Config.GPIO_PinAltFunMode << (4 * temp2));
	}
}

/**************************************************************************************
 * @fn				- GPIO_DeInit
 *
 * @brief			- Resets the specified GPIO peripheral
 *
 * @param[in]		- pGPIOx : Pointer to the GPIO peripheral register structure
 *
 * @return			- None
 *
 * @note			- The GPIO peripheral is returned to its default reset state
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
	if (pGPIOx == GPIOA)
	{
		GPIOA_REG_RESET();
	} else if (pGPIOx == GPIOB)
	{
		GPIOB_REG_RESET();
	} else if (pGPIOx == GPIOC)
	{
		GPIOC_REG_RESET();
	} else if (pGPIOx == GPIOD)
	{
		GPIOD_REG_RESET();
	} else if (pGPIOx == GPIOE)
	{
		GPIOE_REG_RESET();
	} else if (pGPIOx == GPIOF)
	{
		GPIOF_REG_RESET();
	} else if (pGPIOx == GPIOG)
	{
		GPIOG_REG_RESET();
	} else if (pGPIOx == GPIOH)
	{
		GPIOH_REG_RESET(); }
}


/**************************************************************************************
 * @fn				- GPIO_ReadFromInputPin
 *
 * @brief			- Reads the current state of the specified GPIO input pin
 *
 * @param[in]		- pGPIOx : Pointer to the GPIO peripheral register structure
 * @param[in]		- PinNumber : GPIO pin number
 *
 * @return			- Current logic level of the pin (0 or 1)
 *
 * @note			- None
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;
	value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 1U);
	return value;
}


/**************************************************************************************
 * @fn				- GPIO_ReadFromInputPort
 *
 * @brief			- Reads the current input state of the specified GPIO port
 *
 * @param[in]		- pGPIOx : Pointer to the GPIO peripheral register structure
 *
 * @return			- 16-bit value containing the current state of all GPIO input pins
 *
 * @note			- Only the lower 16 bits are valid since each GPIO port has 16 pins
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t value;
	value = (uint16_t)(pGPIOx->IDR);
	return value;
}


/**************************************************************************************
 * @fn				- GPIO_WriteToOutputPin
 *
 * @brief			- Writes a logic level to the specified GPIO output pin
 *
 * @param[in]		- pGPIOx : Pointer to the GPIO peripheral register structure
 * @param[in]		- PinNumber : GPIO pin number
 * @param[in]		- Value : GPIO_PIN_SET or GPIO_PIN_RESET
 *
 * @return			-
 *
 * @note			- The pin must be configured as an output or in an alternate function mode with output capability
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value)
{
	if(Value == GPIO_PIN_SET)
	{
		// write 1 to the output data register at the bit field corresponding to the pin number
		pGPIOx->ODR |= (1 << PinNumber);
	}else
	{
		// write 0
		pGPIOx->ODR &= ~(1 << PinNumber);
	}
}


/**************************************************************************************
 * @fn				- GPIO_WriteToOutputPort
 *
 * @brief			- Writes a value to the specified GPIO output port
 *
 * @param[in]		- pGPIOx : Pointer to the GPIO peripheral register structure
 * @param[in]		- Value : 16-bit value to be written to the GPIO output data register
 *
 * @return			- None
 *
 * @note			- Each bit of Value corresponds to one GPIO pin:
 *                    bit0 -> Pin0, bit1 -> Pin1, ..., bit15 -> Pin15.
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{
	pGPIOx->ODR = Value;
}


/**************************************************************************************
 * @fn				- GPIO_ToggleOutputPin
 *
 * @brief			- Toggles the logic level of the specified GPIO output pin
 *
 * @param[in]		- pGPIOx : Pointer to the GPIO peripheral register structure
 * @param[in]		- PinNumber : GPIO pin number
 *
 * @return			- None
 *
 * @note			- The pin must be configured as an output or in an alternate function mode with output capability
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR ^= (1 << PinNumber);
}


/**************************************************************************************
 * @fn				- GPIO_IRQInterruptConfig
 *
 * @brief			- Configures IRQ number in NVIC: enable/disable and priority
 *
 * @param[in]		- IRQNumber  : IRQ position number from vector table (RM0390 Table 38)
 * @param[in]		- IRQPriority: Priority level 0-15 (0 = highest, 15 = lowest)
 * @param[in]		- EnorDi     : ENABLE or DISABLE
 *
 * @return			- None
 *
 * @note			- Processor-side configuration (ARM Cortex-M4 NVIC registers)
 *                	  STM32F446xx implements 4 priority bits in upper nibble of IPR byte
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
    /* --- Enable / Disable --- */
    if (EnorDi == ENABLE)
    {
        if      (IRQNumber <= 31)  *NVIC_ISER0 |= (1U << (IRQNumber % 32));
        else if (IRQNumber <= 63)  *NVIC_ISER1 |= (1U << (IRQNumber % 32));
        else if (IRQNumber <= 95)  *NVIC_ISER2 |= (1U << (IRQNumber % 32));
        else                       *NVIC_ISER3 |= (1U << (IRQNumber % 32));
    }
    else
    {
        if      (IRQNumber <= 31)  *NVIC_ICER0 |= (1U << (IRQNumber % 32));
        else if (IRQNumber <= 63)  *NVIC_ICER1 |= (1U << (IRQNumber % 32));
        else if (IRQNumber <= 95)  *NVIC_ICER2 |= (1U << (IRQNumber % 32));
        else                       *NVIC_ICER3 |= (1U << (IRQNumber % 32));
    }
}

/**************************************************************************************
 * @fn				- GPIO_IRQPriorityConfig
 *
 * @brief			-
 *
 * @param[in]		-
 * @param[in]		-
 * @param[in]		-
 *
 * @return			-
 *
 * @note			-
 */
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
	// 1. first lets find out the IPR register
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;
	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
}

/**************************************************************************************
 * @fn				-
 *
 * @brief			-
 *
 * @param[in]		-
 * @param[in]		-
 * @param[in]		-
 *
 * @return			-
 *
 * @note			-
 */
void GPIO_IRQHandling(uint8_t PinNumber)
{
	// clear the EXTI PR register corresponding to the pin number
	if(EXTI->PR & (1 << PinNumber))
	{
		// clear
		EXTI->PR |= (1 << PinNumber);
	}
}


