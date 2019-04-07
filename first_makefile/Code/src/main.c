#include "main.h"

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
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  GPIOA->CRL &= ~(GPIO_CRL_CNF5_0|GPIO_CRL_CNF5_1);
  GPIOA->CRL |= GPIO_CRL_MODE5_1;
  GPIOA->BSRR |= GPIO_BSRR_BS5;

  while(1)
  {
    GPIOA->BSRR |= GPIO_BSRR_BS5;
    delay(100);
    GPIOA->BSRR |= GPIO_BSRR_BR5;
    delay(100);
  }
}
