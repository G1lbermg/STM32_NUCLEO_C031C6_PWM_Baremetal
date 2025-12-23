#include <adc_BSP.h>

static volatile uint16_t dataADC1;
static volatile uint8_t flagADC1Data = 0;


/***************ADC interrupt will alert us when there is new data***************************/
void ADC1_IRQHandler (void)
{
	if(ADC1->ISR & ADC_ISR_EOC){
		dataADC1 = ADC1->DR;
		flagADC1Data = 1;
	}
}

/****************Steps to initialize PA0 to use as ADC_IN0*******************/
ErrorCode_t initADC_Ch0(void)
{
	/***************Setup GPIO pin to be used as adc***************************/

	//enable Port A clock
	SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN);

	//Set pin to analog function via moder register
	SET_BIT(GPIOA->MODER, GPIO_MODER_MODE0);


	/********************Setup the ADC****************************************/

	//Enable ADC clock and configure to use PCLK w/ 1/2 prescaler
	SET_BIT(RCC->APBENR2, RCC_APBENR2_ADCEN);

	CLEAR_BIT(ADC1->CFGR2, ADC_CFGR2_CKMODE);
	SET_BIT(ADC1->CFGR2, ADC_CFGR2_CKMODE_0);

	//Enable the end of conversion interrupt
	SET_BIT(ADC1->IER, ADC_IER_EOCIE);
	NVIC_EnableIRQ(ADC1_IRQn);

	//Set sample time to 160.5 ADC clock cycles
	SET_BIT(ADC1->SMPR, ADC_SMPR_SMP1);
	CLEAR_BIT(ADC1->SMPR, ADC_SMPR_SMPSEL0);

	//Select the ADC channel
	CLEAR_BIT(ADC1->CFGR1, ADC_CFGR1_CHSELRMOD);
	SET_BIT(ADC1->CHSELR, ADC_CHSELR_CHSEL0);

	while(!(ADC1->ISR & ADC_ISR_CCRDY))
		;

	//Enable internal ADC voltage regulator then wait 20 micro sec for it to startup
	SET_BIT(ADC1->CR, ADC_CR_ADVREGEN);
	LL_mDelay(1);

	//Calibrate ADC via, check it has completed
	SET_BIT(ADC1->CR, ADC_CR_ADCAL);

	while((ADC1->CR & ADC_CR_ADCAL))
		;

	//Set to single conversion mode and have conversions be set by software
	CLEAR_BIT(ADC1->CFGR1, ADC_CFGR1_CONT);
	CLEAR_BIT(ADC1->CFGR1, ADC_CFGR1_EXTEN);


	//Enable ADC1 by clearing ADRDY and setting ADEN, then wait for ADRDY flag
	SET_BIT(ADC1->ISR, ADC_ISR_ADRDY);
	SET_BIT(ADC1->CR, ADC_CR_ADEN);

	while(!(ADC1->ISR & ADC_ISR_ADRDY))
		;

	return E_OK;

}

/***************ADC will start a conversion***************************/
ErrorCode_t runADC_Ch0(void)
{
	NVIC_DisableIRQ(ADC1_IRQn);
    // Clear the flag before starting a new conversion
    flagADC1Data = 0;

    // Start the ADC conversion by setting the ADSTART bit in the Control Register
    SET_BIT(ADC1->CR, ADC_CR_ADSTART);

	NVIC_EnableIRQ(ADC1_IRQn);

	return E_OK;
}

/***************Retreive data after conversion**************************/
ErrorCode_t readDataADC_Ch0(uint16_t *data)
{
	  while(!flagADC1Data);

	  NVIC_DisableIRQ(ADC1_IRQn);

	  *data = dataADC1;
	  flagADC1Data = 0;

	  NVIC_EnableIRQ(ADC1_IRQn);

	  return E_OK;
}


