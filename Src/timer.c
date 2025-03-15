#include "stm32f4xx.h"
#include "timer.h"

void timer_init(void)
{
	/* Enable clock access to tim2 */
	RCC->APB1ENR |= TIM2EN;

	/* Set prescaler value */
	TIM2->PSC = 16 - 1;	// Prescaler = 16 (16Mhz / 16 = 1Mhz)

	/* Set auto-reload value */
	TIM2->ARR = 1000 - 1;	// Auto-reload = 1000 cycle (1ms)

	/* Clear counter */
	TIM2->CNT = 0;

	/* Enable timer */
	TIM2->CR1 = CR1_CEN;
}

void delay_ms(uint32_t ms)
{
	for (uint32_t i = 0; i < ms; i++)
	{
		TIM2->CNT = 0;			// Reset counter
		TIM2->SR &= ~SR_UIF;	// Clear update interrupt flag
		while (!(TIM2->SR & SR_UIF));
	}
}


