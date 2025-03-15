#include "uart.h"

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t baudrate);
static uint16_t compute_uart_baudrate(uint32_t PeriphClk, uint32_t baudrate);
void uart2_write(int ch);

int __io_putchar(int ch)
{
	uart2_write(ch);
	return ch;
}

void uart2_tx_init(void)
{
	/*********************** Configure uart gpio pin ***********************/
	/* Enable clock access to gpioa */
	RCC->AHB1ENR |= GPIOA_EN;
	/* Set PA2 mode to alternate function mode */
	GPIOA->MODER &= ~(1U << 4);
	GPIOA->MODER |= (1U << 5);
	/* Set PA2 alternate function type to UART_TX (AF07) */
	GPIOA->AFR[0] |= (1U << 10);
	GPIOA->AFR[0] |= (1U << 9);
	GPIOA->AFR[0] |= (1U << 8);
	GPIOA->AFR[0] &= ~(1U << 11);

	/*********************** Configure uart module ***********************/
	/* Enable clock access to uart2 */
	RCC->APB1ENR |= UART2_EN;

	/* Configure baudrate */
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	/* Configure the transfer direction */
	USART2->CR1 |= CR1_TE;
	/* Enable uart module*/
	USART2->CR1 |= CR1_UE;
}

void uart2_rxtx_init(void)
{
	/*********************** Configure uart gpio pin ***********************/
	/* Enable clock access to gpioa */
	RCC->AHB1ENR |= GPIOA_EN;

	/* Set PA2 mode to alternate function mode */
	GPIOA->MODER &= ~(1U << 4);
	GPIOA->MODER |= (1U << 5);

	/* Set PA2 alternate function type to UART_TX (AF07) */
	GPIOA->AFR[0] |= ((1U << 10)|(1U << 9)|(1U << 8));
	GPIOA->AFR[0] &= ~(1U << 11);

	/* Set PA2 mode to alternate function mode */
	GPIOA->MODER &= ~(1U << 6);
	GPIOA->MODER |= (1U << 7);

	/* Set PA2 alternate function type to UART_TX (AF07) */
	GPIOA->AFR[0] |= ((1U << 14)|(1U << 13)|(1U << 12));
	GPIOA->AFR[0] &= ~(1U << 15);

	/*********************** Configure uart module ***********************/
	/* Enable clock access to uart2 */
	RCC->APB1ENR |= UART2_EN;

	/* Configure baudrate */
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	/* Configure the transfer direction */
	USART2->CR1 |= CR1_TE | CR1_RE;
	/* Enable uart module*/
	USART2->CR1 |= CR1_UE;
}

void uart2_write(int ch)
{
	/* Make sure the transmit data register is empty */
	while(!(USART2->SR & SR_TXE)) {}
	/* Write to transmit data register */
	USART2->DR = (ch & 0xFF);
}

char uart2_read(void)
{
	/* Make sure the receive data register is not empty */
	while(!(USART2->SR & SR_RXNE)) {}

	/* Read data */
	return USART2->DR;
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t baudrate)
{
	USARTx->BRR = compute_uart_baudrate(PeriphClk, baudrate);
}

static uint16_t compute_uart_baudrate(uint32_t PeriphClk, uint32_t baudrate)
{
	return ((PeriphClk + (baudrate / 2U)) / baudrate);
}


