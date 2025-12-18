#include "timer3_PwmBSP.h"


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

	//Program psc register so that we get 1kHZ frequency: 48MhzHz / ((psc+1)(arr+1)) = 1kHz ->   psc = 47, arr = 999
	WRITE_REG(TIM3->PSC, 0x2fU); //psc = 47

	//Program period in the arr register
	WRITE_REG(TIM3->ARR, 0x3e7U); //arr = 999

	//Program the duty cycle using the ccr register
	WRITE_REG(TIM3->CCR1, 0x3e7U);

	//Set the pre-load bit in the CCMRX register and the ARPE bit in the CR1 register
	SET_BIT(TIM3->CCMR1, TIM_CCMR1_OC1PE); //enable pre-load bit
	SET_BIT(TIM3->CR1, TIM_CR1_ARPE); //enable auto pre-load

	//Set the counting mode and direction
	CLEAR_BIT(TIM3->CR1,TIM_CR1_CMS); //edge aligned mode
	CLEAR_BIT(TIM3->CR1,TIM_CR1_DIR); //up counting

	//Generate an update event to load ARR and CCR1 immediately
	SET_BIT(TIM3->EGR, TIM_EGR_UG);

	//Enable the capture compare
	SET_BIT(TIM3->CCER,TIM_CCER_CC1E);

	//Enable the counter
	SET_BIT(TIM3->CR1, TIM_CR1_CEN);
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

/*
void brightenLED(void)
{
	volatile uint32_t period = TIM3->ARR; // Get the max period from the register
	volatile uint32_t i;
    // Note: The inner 'j' loop provides a simple, processor-specific delay
    // which may need adjustment for a visible dimming effect.

	// Sweep UP (Dim to Bright)
	for(i = 0U; i <= period; i++)
    {
		// FIX: Change the Capture/Compare Register (CCR1) to vary the duty cycle
		WRITE_REG(TIM3->CCR1, i);

        // A slight delay is needed to observe the change
        // You generally don't need an Update Generation (UG) here because
        // the OC1PE bit (Preload Enable) is set in initPWM.
        // The CCR value will update automatically on the next counter overflow.
        // We'll keep the delay loop for now.
		for(volatile uint16_t j = 0U; j < 10000U; j++)
			;
	}
}

void dimLED(void)
{
	volatile uint32_t period = TIM3->ARR;
	volatile uint32_t i;

    // Sweep DOWN (Bright to Dim)
    for(i = period; i > 0U; i--) // Stop when i hits 0 (misses CCR1=0)
    {
		WRITE_REG(TIM3->CCR1, i);
		for(volatile uint16_t j = 0U; j < 10000U; j++)
			;
	}

    // **Manually write the 0% duty cycle**
    WRITE_REG(TIM3->CCR1, 0U);
    // Add a final delay if needed, or simply let the loop continue
    for(volatile uint16_t j = 0U; j < 100U; j++)
    	;
}
*/

