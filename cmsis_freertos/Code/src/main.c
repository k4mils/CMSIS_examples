#include "../inc/main.h"

void delay(uint64_t time)
{
	time=time*250*16;
	while(time--)
	{
		__NOP();
		__NOP();
	}
}
void vTask1()
{
	for(;;)
	{
		GPIOA->BSRR |= GPIO_BSRR_BS5;
    vTaskDelay(pdMS_TO_TICKS(100));
    GPIOA->BSRR |= GPIO_BSRR_BR5;
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

int main(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  GPIOA->CRL &= ~(GPIO_CRL_CNF5_0|GPIO_CRL_CNF5_1);
  GPIOA->CRL |= GPIO_CRL_MODE5_1;
  GPIOA->BSRR |= GPIO_BSRR_BS5;
  vTaskDelayUntil(1,1);

	xTaskCreate(vTask1,"Task1",128,NULL,2,NULL);
	vTaskStartScheduler();
	while(1)
  {
    GPIOA->BSRR |= GPIO_BSRR_BS5;
    delay(400);
    GPIOA->BSRR |= GPIO_BSRR_BR5;
    delay(400);
  }

}
