#include "timer3_BSP.h"

#define ARR_VALUE 999U
#define TMR_TICK_DIVISOR (ARR_VALUE +1)


/*****************Code to use the timer as a counter *****************************/

//Counter records the number of ms elapsed since start of program.
//Good up to ~49.772 days
static uint32_t volatile timer3Counter = 0U;

void TIM3_IRQHandler(void)
{
	__disable_irq();

	// Clear the Update Interrupt Flag (UIF) if it was set by the UEV
	CLEAR_BIT(TIM3->SR, TIM_SR_UIF);

	//Increment timer
	timer3Counter++;

	__enable_irq();
}

//Initialization code to setup a timer given a System core clock of 48Mhz
ErrorCode_t initCounter_Tmr3(uint32_t targetFreqHz)
{
//------1. PSC value calculations---------
	//Check that target frequency is within bounds
	if (targetFreqHz == 0)
		return E_INVALID_ARGUMENT;

	if(targetFreqHz > (SystemCoreClock/TMR_TICK_DIVISOR))
        return E_TIMER_FREQ_TOO_HIGH;

	//Caclulate and check that the psc value is within bounds
	uint32_t pscPlusOne = SystemCoreClock/(TMR_TICK_DIVISOR * targetFreqHz);

	if(pscPlusOne == 0)
		return E_TIMER_CALC_ERROR;

	uint32_t pscValue = pscPlusOne -1;

	// Check if the calculated PSC value fits in the 16-bit register
	if (pscValue > 0xFFFF)
		return E_TIMER_FREQ_TOO_LOW;

//------2. Hardware register setup---------
	//Enable timer 3 clock gate
	SET_BIT(RCC->APBENR1,RCC_APBENR1_TIM3EN);

	//Set to up counting and edge aligned mode
	CLEAR_BIT(TIM3->CR1, TIM_CR1_DIR);
	CLEAR_BIT(TIM3->CR1,TIM_CR1_CMS);

	//Update request only at counter overflow/underflow
	SET_BIT(TIM3->CR1, TIM_CR1_URS);

	//Set arr to 99, and psc to calculated value to achieve desired update frequency
	WRITE_REG(TIM3->PSC,pscValue);
	WRITE_REG(TIM3->ARR, ARR_VALUE);

	//Enable UEV(update events)
	CLEAR_BIT(TIM3->CR1,TIM_CR1_UDIS);

	//Force update to registers
	WRITE_REG(TIM3->EGR,TIM_EGR_UG);

	// Clear the Update Interrupt Flag (UIF) if it was set by the UEV
	CLEAR_BIT(TIM3->SR, TIM_SR_UIF);

	//Enable update interrupts
	SET_BIT(TIM3->DIER,TIM_DIER_UIE);

	//Enable timer3 interrupt
	__NVIC_EnableIRQ(TIM3_IRQn);

	//Enable the counter
	SET_BIT(TIM3->CR1, TIM_CR1_CEN);

	return E_OK;
}

ErrorCode_t elapsedTicks_Tmr3(uint32_t *milliSeconds)
{
	//Check for NULL Pointer
	if(milliSeconds == 0)
		return E_INVALID_ARGUMENT;

	uint32_t readTime;
    // Read the current time of the counter

	__disable_irq();
	readTime = timer3Counter;
	__enable_irq();

	*milliSeconds = readTime;

	return E_OK;
}

ErrorCode_t delayTicks_Tmr3(uint32_t milliSeconds)
{
	uint32_t startTime, currentTime;
	ErrorCode_t errorCheck;

	// Establish a starting time
	errorCheck = elapsedTicks_Tmr3(&startTime);

	if(errorCheck != E_OK)
		return errorCheck;

	// The only time we need to disable IRQs is when reading timeCounter3.
	while(1) // Loop forever until condition is met
	{
        // Read the current time
		errorCheck = elapsedTicks_Tmr3(&currentTime);
		if(errorCheck != E_OK)
			return errorCheck;

        // Check condition
        if (currentTime - startTime >= milliSeconds)
        {
            break; // Exit the function after 1000 milli-seconds have passed
        }
	}

	return E_OK;
}

/*****************Code to use the timer to drive a PWM signal*****************************/

//--------Timer 3 Channel 1---------

void initPWM_Tim3Ch1(void)
{
	/*Sets timer 3 to have a 1KHz output frequency. Kernel clock is expected to be 48MHz.
	 * Channel 3 is configured to output the pwm signal*/

	/********************PWM on GPIO PB4 settings*************************/
	//Enable PB4 clock gate
	SET_BIT(RCC->IOPENR,RCC_IOPENR_GPIOBEN);

	//Set to alternate function mode AF1
	CLEAR_BIT(GPIOB->MODER, GPIO_MODER_MODE4);
	SET_BIT(GPIOB->MODER, GPIO_MODER_MODE4_1);

	CLEAR_BIT(GPIOB->AFR[0], GPIO_AFRL_AFSEL4);
	SET_BIT(GPIOB->AFR[0], GPIO_AFRL_AFSEL4_0);

	/********************Timer 3 PWM settings*************************/
	//Enable timer 3 clock gate
	SET_BIT(RCC->APBENR1,RCC_APBENR1_TIM3EN);

	//initializing PWM on timer 3 Ch1 which corresponds to PB4

	//Configure output pin
	CLEAR_BIT(TIM3 -> CCMR1, TIM_CCMR1_CC1S); //configure pin as output
	CLEAR_BIT(TIM3 -> CCMR1, TIM_CCMR1_CC2S); //configure pin as output

	//Select the polarity writing the CCXP bit in CCER register
	CLEAR_BIT(TIM3 -> CCER, TIM_CCER_CC1P); //Set to active high output

	//Select pwm mode by writing to the OCXM bits in the CCMRX register
	CLEAR_BIT(TIM3->CCMR1, TIM_CCMR1_OC1M);
	SET_BIT(TIM3->CCMR1, TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);

	//Program the duty cycle using the ccr register
	WRITE_REG(TIM3->CCR1, 0x0U);

	//Set the pre-load bit in the CCMRX register and the ARPE bit in the CR1 register
	SET_BIT(TIM3->CCMR1, TIM_CCMR1_OC1PE); //enable pre-load bit
	SET_BIT(TIM3->CR1, TIM_CR1_ARPE); //enable auto pre-load

	//Enable the capture compare
	SET_BIT(TIM3->CCER,TIM_CCER_CC1E);
}

uint8_t setDutyCycle_Tim3Ch1(uint16_t percentage)
{
	uint16_t dutyCycle;

	if(percentage > 100U || percentage < 0U){
		return 0;
	}
	else{
		dutyCycle = (percentage*1000) / 100; //technically ((percentage/100) * 1000 but avoids truncating division
		WRITE_REG(TIM3->CCR1, dutyCycle);

		return 1;
	}

}
