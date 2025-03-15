#include "spi.h"
#include "dma.h"

/*
 * PA5 -> CLK
 * PA6 -> MISO
 * PA7 -> MOSI
 * PA9 -> Slave Select
 */

volatile uint8_t dma_tx_complete = 0;
volatile uint8_t dma_rx_complete = 0;

void spi_gpio_init(void)
{
	/* Enable clock access to GPIOA */
	RCC->AHB1ENR |= GPIOA_EN;

	/* Set PA5, PA6, PA7 mode to alternate function */

	// PA5
	GPIOA->MODER &= ~(1U << 10);
	GPIOA->MODER |= (1U << 11);

	// PA6
	GPIOA->MODER &= ~(1U << 12);
	GPIOA->MODER |= (1U << 13);

	// PA7
	GPIOA->MODER &= ~(1U << 14);
	GPIOA->MODER |= (1U << 15);

	// Set PA9 as output pin
	GPIOA->MODER |= (1U << 18);
	GPIOA->MODER &= ~(1U << 19);

	/* Set PA5, PA6, PA7 alternate function type to SPI1 */
	// PA5: AF5
	GPIOA->AFR[0] |= (1U << 20);
	GPIOA->AFR[0] &= ~(1U << 21);
	GPIOA->AFR[0] |= (1U << 22);
	GPIOA->AFR[0] &= ~(1U << 23);

	// PA6: AF5
	GPIOA->AFR[0] |= (1U << 24);
	GPIOA->AFR[0] &= ~(1U << 25);
	GPIOA->AFR[0] |= (1U << 26);
	GPIOA->AFR[0] &= ~(1U << 27);

	// PA7: AF5
	GPIOA->AFR[0] |= (1U << 28);
	GPIOA->AFR[0] &= ~(1U << 29);
	GPIOA->AFR[0] |= (1U << 30);
	GPIOA->AFR[0] &= ~(1U << 31);
}

void spi1_config(void)
{
	/* Enable clock access to SPI1 module */
	RCC->APB2ENR |= SPI1_EN;

	/* Set clock to fPCLK/2 */
	SPI1->CR1 |= (F_PCLK_DIV_2 << CR1_BR_BIT);

	/* Set CPOL to 1 and CPHA to 1 */
	SPI1->CR1 |= (1U << 0);
	SPI1->CR1 |= (1U << 1);

	/* Enable full duplex */
	SPI1->CR1 &= ~(1U << 10);

	/* Set MSB first */
	SPI1->CR1 &= ~(1U << 7);

	/* Set mode to MASTER */
	SPI1->CR1 |= (1U << 2);

	/* Set 8 bit data mode */
	SPI1->CR1 &= ~(1U << 11);

	/* Select software slave management by setting SSM = 1 and SSI = 1 */
	SPI1->CR1 |= (1U << 8);
	SPI1->CR1 |= (1U << 9);

	/* Enable DMA TX/RX */
	SPI1->CR2 |= (CR2_TXDMAEN | CR2_RXDMAEN);

	/* Enable SPI module */
	SPI1->CR1 |= (1U << 6);
}

void spi1_transmit(uint8_t *data, uint32_t size)
{
	uint32_t i = 0;
	uint8_t temp;
	while (i < size)
	{
		/* Wait until TXE is set */
		while (!(SPI1->SR && SR_TXE)) {}

		/* Write the data to the data register */
		SPI1->DR = data[i];
		i++;
	}
	/* Wait until TXE is set */
	while (!(SPI1->SR && SR_TXE)) {}

	/* Wait for BUSY flag to reset */
	while ((SPI1->SR && SR_BSY)) {}

	/* Clear OVR flag */
	temp = SPI1->DR;
	temp = SPI1->SR;
}

void spi1_receive(uint8_t *data, uint32_t size)
{
	while (size)
	{
		/* Send dummy data */
		SPI1->SR = 0;

		/* Wait for RXNE flag to be set */
		while (!(SPI1->SR & SR_RXNE)) {}

		/* Read data from data register */
		*data++ = (SPI1->DR);
		size--;
	}
}

void cs_enable(void)
{
	GPIOA->ODR &= ~SPI_CS;
}

/* Pull high to disable */
void cs_disable(void)
{
	GPIOA->ODR |= SPI_CS;
}

void ce_low(void)
{
	GPIOA->ODR &= ~SPI_CE;
}

void ce_high(void)
{
	GPIOA->ODR |= SPI_CE;
}

void spi1_write_byte(uint8_t data)
{
	// Wait TX buffer clear
	while (!(SPI1->SR & SR_TXE));

	SPI1->DR = data;

	// Complete waiting transmit
	while (!(SPI1->SR & SR_TXE));
}

uint8_t spi1_read_byte(void)
{
	// Send dummy byte to create clock
	while (!(SPI1->SR & SR_TXE));
	SPI1->DR = 0xFF;

	// Wait receiving data
	while (!(SPI1->SR & SR_RXNE));

	return (uint8_t)SPI1->DR;
}
uint8_t spi1_transfer(uint8_t data)
{
	// Wait TX buffer clear
	while (!(SPI1->SR & SR_TXE));

	SPI1->DR = data;

	// Wait receiving data
	while (!(SPI1->SR & SR_RXNE));

	return (uint8_t)SPI1->DR;
}

void spi1_transfer_dma(uint8_t *tx_buffer, uint8_t *rx_buffer, uint16_t len)
{
	dma_tx_complete = 0;
	dma_rx_complete = 0;

	// Configure DMA RX
	DMA2_Stream2->M0AR = (uint32_t)rx_buffer;
	DMA2_Stream2->NDTR = len;
	DMA2_Stream2->CR |= DMA_EN;

	// Configure DMA TX
	DMA2_Stream3->M0AR = (uint32_t)tx_buffer;
	DMA2_Stream3->NDTR = len;
	DMA2_Stream3->CR |= DMA_EN;

	/* Wait transfer completely */
	while (!dma_tx_complete || !dma_rx_complete);
}

