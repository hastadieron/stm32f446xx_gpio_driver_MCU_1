/*
 * stm32f446xx_gpio_driver.c
 */

#include "stm32f446xx_gpio_driver.h"

/************************************************************************************************
 * @fn          - GPIO_PeriClockControl
 *
 * @brief       - Enables or disables the peripheral clock for the specified GPIO port
 *
 * @param[in]   - pGPIOx : Pointer to the GPIO peripheral register definition
 * @param[in]   - EnorDi : ENABLE to enable the peripheral clock,
 *                         DISABLE to disable the peripheral clock
 *
 * @return      - None
 *
 * @note        - The GPIO peripheral clock must be enabled before accessing
 *                or configuring the GPIO registers.
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
 * @fn          - GPIO_Init
 *
 * @brief       - Initializes the specified GPIO pin according to the configuration
 *                stored in the GPIO handle
 *
 * @param[in]   - pGPIOHandle : Pointer to the GPIO handle containing the GPIO port
 *                              and pin configuration
 *
 * @return      - None
 *
 * @note        - The function enables the GPIO peripheral clock and configures:
 *                - GPIO mode
 *                - Output speed
 *                - Pull-up/Pull-down resistors
 *                - Output type
 *                - Alternate function, when GPIO_MODE_ALTFN is selected
 *
 *                For interrupt modes, the function additionally configures:
 *                - GPIO pin as input
 *                - EXTI rising/falling edge trigger
 *                - SYSCFG EXTICR GPIO port mapping
 *                - EXTI interrupt mask
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	uint32_t temp = 0; // temporary register value

	// Enable the peripheral clock
	GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);

	// 1. Configure the mode of GPIO pin
	if (pGPIOHandle->GPIO_Pin_Config.GPIO_PinMode <= GPIO_MODE_ANALOG) {
		// non-interrupt GPIO mode
		temp = (pGPIOHandle->GPIO_Pin_Config.GPIO_PinMode
				<< (2 * pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER &= ~(0x3
				<< (2 * pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber)); // clear
		pGPIOHandle->pGPIOx->MODER |= temp; // set
		temp = 0;

	} else {
		// Interrupt mode requires the GPIO pin to operate as an input
		    pGPIOHandle->pGPIOx->MODER &=
		            ~(0x3U << (2 * pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber));

		if (pGPIOHandle->GPIO_Pin_Config.GPIO_PinMode == GPIO_MODE_IT_FT)
		{
			// 1. Configure  Falling trigger selection register (EXTI_FTSR)
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber);
			// Clear the corresponding RTSR bit
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_Pin_Config.GPIO_PinNumber);

		} else if (pGPIOHandle->GPIO_Pin_Config.GPIO_PinMode == GPIO_MODE_IT_RT)
		{
			// 1. Configure  Rising trigger selection register (EXTI_RTSR)
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

		// Clear only the 4-bit field corresponding to this EXTI line
		SYSCFG->EXTICR[temp1] &= ~(0xFU << (temp2 * 4));

		// Set the GPIO port code for this EXTI line
		SYSCFG->EXTICR[temp1] |= ((uint32_t)portcode << (temp2 * 4));

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
 * @fn          - GPIO_DeInit
 *
 * @brief       - Resets the specified GPIO peripheral to its reset state
 *
 * @param[in]   - pGPIOx : Pointer to the GPIO peripheral register definition
 *
 * @return      - None
 *
 * @note        - This function resets the entire GPIO port by using the corresponding
 *                RCC peripheral reset bit.
 *
 *                EXTI and SYSCFG configurations associated with GPIO pins are not
 *                cleared by this function.
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
		GPIOH_REG_RESET();
	}
}


/**************************************************************************************
 * @fn          - GPIO_ReadFromInputPin
 *
 * @brief       - Reads the current logic level of the specified GPIO pin
 *
 * @param[in]   - pGPIOx    : Pointer to the GPIO peripheral register definition
 * @param[in]   - PinNumber : GPIO pin number (0 to 15)
 *
 * @return      - GPIO_PIN_SET (1) if the pin input is HIGH,
 *                GPIO_PIN_RESET (0) if the pin input is LOW
 *
 * @note        - The function reads the corresponding bit from the GPIO input
 *                data register (IDR).
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;
	value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 1U);
	return value;
}


/**************************************************************************************
 * @fn          - GPIO_ReadFromInputPort
 *
 * @brief       - Reads the current input state of all pins in the specified GPIO port
 *
 * @param[in]   - pGPIOx : Pointer to the GPIO peripheral register definition
 *
 * @return      - 16-bit value representing the current logic levels of GPIO pins 0 to 15
 *
 * @note        - The function reads the GPIO input data register (IDR).
 *                Bit 0 corresponds to Pin 0, bit 1 to Pin 1, ..., bit 15 to Pin 15.
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t value;
	value = (uint16_t)(pGPIOx->IDR);
	return value;
}


/**************************************************************************************
 * @fn          - GPIO_WriteToOutputPin
 *
 * @brief       - Writes a logic level to the specified GPIO output pin
 *
 * @param[in]   - pGPIOx    : Pointer to the GPIO peripheral register definition
 * @param[in]   - PinNumber : GPIO pin number (0 to 15)
 * @param[in]   - Value     : GPIO_PIN_SET or GPIO_PIN_RESET
 *
 * @return      - None
 *
 * @note        - The pin should be configured in GPIO output mode.
 *                The function updates the corresponding bit in the output
 *                data register (ODR).
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
 * @fn          - GPIO_WriteToOutputPort
 *
 * @brief       - Writes a 16-bit value to the output data register of the specified
 *                GPIO port
 *
 * @param[in]   - pGPIOx : Pointer to the GPIO peripheral register definition
 * @param[in]   - Value  : 16-bit value to be written to the GPIO output data register
 *
 * @return      - None
 *
 * @note        - Each bit of Value controls the corresponding GPIO pin:
 *                bit 0 -> Pin 0, bit 1 -> Pin 1, ..., bit 15 -> Pin 15.
 *                The affected pins should be configured in GPIO output mode.
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{
	pGPIOx->ODR = Value;
}


/**************************************************************************************
 * @fn          - GPIO_ToggleOutputPin
 *
 * @brief       - Toggles the logic level of the specified GPIO output pin
 *
 * @param[in]   - pGPIOx    : Pointer to the GPIO peripheral register definition
 * @param[in]   - PinNumber : GPIO pin number (0 to 15)
 *
 * @return      - None
 *
 * @note        - The pin should be configured in GPIO output mode.
 *                The function toggles the corresponding bit in the output
 *                data register (ODR).
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR ^= (1 << PinNumber);
}


/**************************************************************************************
 * @fn          - GPIO_IRQInterruptConfig
 *
 * @brief       - Enables or disables the specified IRQ in the NVIC
 *
 * @param[in]   - IRQNumber : IRQ number from the STM32F446xx interrupt vector table
 * @param[in]   - EnorDi    : ENABLE to enable the IRQ,
 *                            DISABLE to disable the IRQ
 *
 * @return      - None
 *
 * @note        - This function configures only the NVIC interrupt enable state.
 *                IRQ priority is configured separately by GPIO_IRQPriorityConfig().
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
 * @fn          - GPIO_IRQPriorityConfig
 *
 * @brief       - Configures the priority of the specified IRQ in the NVIC
 *
 * @param[in]   - IRQNumber   : IRQ number from the STM32F446xx interrupt vector table
 * @param[in]   - IRQPriority : Interrupt priority value from 0 to 15
 *
 * @return      - None
 *
 * @note        - STM32F446xx implements 4 priority bits for each IRQ.
 *                Priority 0 is the highest priority and priority 15 is the lowest.
 *                The priority value is stored in the upper 4 bits of the corresponding
 *                8-bit NVIC priority field.
 */
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
	// 1. Determine the NVIC priority register and byte position for this IRQ
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;
	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

	// Clear the 8-bit priority field for this IRQ
	*(NVIC_PR_BASE_ADDR + iprx) &= ~(0xFFU << (8 * iprx_section));

	// Set the new priority value
	*(NVIC_PR_BASE_ADDR + iprx) |= ((IRQPriority & 0x0FU) << shift_amount);
}

/**************************************************************************************
 * @fn          - GPIO_IRQHandling
 *
 * @brief       - Clears the pending interrupt flag for the specified EXTI line
 *
 * @param[in]   - PinNumber : GPIO pin number corresponding to the EXTI line (0 to 15)
 *
 * @return      - None
 *
 * @note        - EXTI_PR is a write-1-to-clear register.
 *                Writing 1 to the corresponding pending bit clears that EXTI interrupt
 *                request, while writing 0 leaves other pending bits unchanged.
 */
void GPIO_IRQHandling(uint8_t PinNumber)
{
	// Check whether the EXTI pending flag for this line is set
	if(EXTI->PR & (1 << PinNumber))
	{
	    // EXTI_PR is write-1-to-clear: write 1 only to the target bit
	    EXTI->PR = (1 << PinNumber);
	}
}
