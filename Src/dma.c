#include "stm32f446xx.h"
#include "dma.h"


void dma_init(void)
{
	uint32_t idx, bit_pos;

	/* Enable clock for DMA2 */
	RCC->AHB1ENR |= DMA2_EN;

	/* Configure DMA2 Stream 3 Channel 3: SPI_TX */
	DMA2_Stream3->CR |= 0x00;	// Clear
	DMA2_Stream3->CR |= (CHANNEL_3 << DMA_CHANNEL_SEL_BIT);	// Channel 3 (SPI_TX)
	DMA2_Stream3->CR |= (MEM_TO_PERIP << DMA_DIR_BIT);	// Memory-to-peripheral
	DMA2_Stream3->CR |= DMA_MINC;	// Increment memory address
	DMA2_Stream3->CR |= DMA_TCIE;	// Transfer complete interrupt enable
	DMA2_Stream3->PAR |= (uint32_t)&SPI1->DR;	// Peripheral address
	DMA2_Stream3->NDTR |= 0x00;

	/* Configure DMA2 Stream 3 Channel 3: SPI_RX */
	DMA2_Stream2->CR |= 0x00;	// Clear
	DMA2_Stream2->CR |= (CHANNEL_3 << DMA_CHANNEL_SEL_BIT);	// Channel 3 (SPI_RX)
	DMA2_Stream2->CR |= DMA_MINC;	// Increment memory address
	DMA2_Stream2->CR |= DMA_TCIE;	// Transfer complete interrupt enable
	DMA2_Stream2->PAR |= (uint32_t)&SPI1->DR;	// Peripheral address
	DMA2_Stream2->NDTR |= 0x00;

	/* Enable interrupt DMA in NVIC */
	idx = DMA2_Stream2_IRQn / 32;
	bit_pos = DMA2_Stream2_IRQn % 32;
	NVIC->ISER[idx] |= (1 << bit_pos);

	idx = DMA2_Stream3_IRQn / 32;
	bit_pos = DMA2_Stream3_IRQn % 32;
	NVIC->ISER[idx] |= (1 << bit_pos);
}

