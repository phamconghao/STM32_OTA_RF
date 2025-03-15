
#ifndef TIMER_H_
#define TIMER_H_

#define TIM2EN			(1U << 0)
#define CR1_CEN			(1U << 0)
#define SR_UIF			(1U << 0)

void timer_init(void);
void delay_ms(uint32_t ms);

#endif /* TIMER_H_ */
