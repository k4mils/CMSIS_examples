#include "../inc/main.h"

#define PCLK1 8000000
#define baudrate 9600
#define baud (PCLK1+baudrate/2)/baudrate

volatile uint32_t counter;

void blinkDiode()
{
   if(GPIOA->ODR & GPIO_ODR_ODR5)
   {
      GPIOA->BSRR |= GPIO_BSRR_BR5;
   }
   else GPIOA->BSRR |= GPIO_BSRR_BS5;
}

void EXTI15_10_IRQHandler()
{
   if(EXTI->PR & EXTI_PR_PR13)
   {
      //tu bedzie komenda wyslania znaku
      EXTI->PR |= EXTI_PR_PR13;
   } 
}

void USART2_IRQHandler()
{
   if(USART2->SR & USART_SR_RXNE)
   {
      char c = USART2->DR;
      if(c == 'c')
      {
         blinkDiode();
      }
      USART2->SR &= ~(USART_SR_RXNE);
   }
}
void TIM2_IRQHandler()
{
   TIM2->SR &= ~TIM_SR_UIF;
   blinkDiode();
   USART2->DR = 'a';
}

void configButton()
{
   RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
   GPIOA->CRL &= ~(GPIO_CRL_CNF5_0|GPIO_CRL_CNF5_1);
   GPIOA->CRL |= GPIO_CRL_MODE5_1;
   
   RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
   RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
   GPIOC->CRH |= GPIO_CRH_CNF13_0;
   GPIOC->CRH &= ~(GPIO_CRH_CNF13_1);
   GPIOC->CRH &= ~(GPIO_CRH_MODE13);
   
   AFIO->EXTICR[3] |= AFIO_EXTICR4_EXTI13_PC;
   EXTI->IMR |= EXTI_IMR_IM13;
   EXTI->RTSR |= EXTI_RTSR_RT13;
   EXTI->FTSR |= EXTI_FTSR_TR13;
   
   NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void configTimer()
{
   RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
   TIM2->PSC = 7999;
   TIM2->ARR = 999;
   TIM2->DIER |= TIM_DIER_UIE;
   TIM2->SR &= ~TIM_SR_UIF;
   TIM2->EGR |= TIM_EGR_UG;
   TIM2->CR1 |= TIM_CR1_CEN;
   NVIC_EnableIRQ(TIM2_IRQn);
}

void configUART()
{
   RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
   GPIOA->CRL |= GPIO_CRL_CNF3_0;
   GPIOA->CRL |= GPIO_CRL_CNF2_1;
   GPIOA->CRL |= GPIO_CRL_MODE2;
   USART2->CR1 |= USART_CR1_TXEIE | USART_CR1_RXNEIE | USART_CR1_TCIE;
   USART2->CR1 |= USART_CR1_RE | USART_CR1_TE;
   USART2->BRR = baud;
   USART2->CR1 |= USART_CR1_UE;
   NVIC_EnableIRQ(USART2_IRQn);
}

int main(void)
{
   __enable_irq();
   configButton(); 
   configTimer();
   configUART();
   
   while(1);
}
