#include "main.h"

volatile uint8_t it;

void USART6_IRQHandler(void)
{
	if(USART6->SR & USART_SR_RXNE)
	{	//After reception RXNE bit in SR register is set

		if((USART6->DR & 0xFF)=='a')
		{	// DR register contains received data

			if(GPIOD->ODR & GPIO_ODR_OD12)
			{
				GPIOD->BSRR |= GPIO_BSRR_BR12;
			}
			else
				GPIOD->BSRR |= GPIO_BSRR_BS12;

				//Pin toggling -> GPIO_BSRR_(BR/BS)_(PIN_NO)
		}
		USART6->SR &= ~(USART_SR_RXNE);
	}
}

void EXTI0_IRQHandler()
{
	EXTI->PR |= EXTI_PR_PR0;

	if(USART6->SR |= USART_SR_TXE)
	{
		USART6->DR = it;
		it++;
	}

}

void uart_cfg()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;   //enable gpio clocking for usart6 pins
	RCC->APB2ENR |= RCC_APB2ENR_USART6EN;  //enable usart6 clocking

	GPIOC->MODER |= (GPIO_MODER_MODER6_1 | GPIO_MODER_MODE7_1);    //enable AF | TX->PC6 RX->PC7
	GPIOC->AFR[0] |= GPIO_AFRL_AFRL6_3;							//set USART6 AF / pin 6
	GPIOC->AFR[0] |= GPIO_AFRL_AFRL7_3;							//set USART6 AF / pin 7

	uint32_t const baudrate = 9600U;
	uint32_t const hsi_ck = 16000000U;
	USART6->BRR |= (uint32_t)((hsi_ck + baudrate/2)/baudrate);
	USART6->CR1 |= (USART_CR1_TE | USART_CR1_RE | USART_CR1_UE | USART_CR1_RXNEIE);

	// RE - Receive Enable			TE - Transmit Enable
	// UE - USART Enable			RXNEIE - Receive Interrupt Enable

	NVIC_EnableIRQ(USART6_IRQn);
}


void exti_cfg()
{
	RCC->APB2ENR |= RCC_AHB1ENR_GPIOAEN;

	GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_0;

	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;	//enable syscfg clocking

	EXTI->IMR |= EXTI_IMR_MR0;				//enable external interrupts
	EXTI->FTSR |= EXTI_FTSR_TR0;			//enable falling edge trigger
//	EXTI->RTSR |= EXTI_RTSR_TR0;			//enable rising edge trigger

	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;	//connect PA0 to interrupt line

	NVIC_EnableIRQ(EXTI0_IRQn);				//enable interrupt controller
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
	uart_cfg();
	exti_cfg();
	__enable_irq();

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= GPIO_MODER_MODER12_0;
	GPIOD->BSRR |= GPIO_BSRR_BS_12;

	while(1)
	{

	}
}
