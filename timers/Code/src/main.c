#include "main.h"

volatile uint16_t cnt;


void TIM6_DAC_IRQHandler()
{

}
void TIM4_IRQHandler()
{
	cnt++;
}

void timer_cfg()
{
	//basic timer configuration

	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;		// enable TIM6 clocking
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;	// enable SYSCFG clocking (interrupts)

	TIM6->DIER |= TIM_DIER_UIE;				// enable update interrupt requests
	TIM6->SR &= ~TIM_SR_UIF;
	TIM6->PSC = 15999;						// TIM6 prescaler (16MHz/16000=1kHz)
	TIM6->EGR |= TIM_EGR_UG;				// init event generation
	TIM6->CR1 |= TIM_CR1_CEN;				// enable TIM6

	NVIC_EnableIRQ(TIM6_DAC_IRQn);			// enable NVIC
}

void pwm_cfg()
{
	//confiugre TIM4 & PD14 to control LED brightness

	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

	GPIOD->AFR[1] |= GPIO_AFRH_AFRH6_1;				//PD14 as AF PWM output
	GPIOD->MODER |= GPIO_MODER_MODER14_1;			//PD14 as AF
	GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR14_1;	//PD14 as high speed

	TIM4->PSC = 159;
	TIM4->ARR = 999;

	TIM4->CCMR2 |= (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1);	//output mode as PWM
	TIM4->CCER |= TIM_CCER_CC3E;							//connect timer signal to gpio

	TIM4->CR1 |= (TIM_CR1_CEN);								//enable TIM4
}

void delay(uint64_t time)
{
	time=time*250*16;
	while(time--)
	{
		__NOP();
		__NOP();
	}
}

int main(void)
{
	__enable_irq();
	timer_cfg();
	pwm_cfg();
	while(1)
	{
		for(uint8_t i=0;i<10;i++)
		{
			TIM4->CCR3=i*100;
			delay(300);
		}
	}
}
