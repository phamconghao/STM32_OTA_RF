
#ifndef SPI_H_
#define SPI_H_

#include "stm32f4xx.h"
#include <stdint.h>

#define SPI1_EN			(1U << 12)
#define GPIOA_EN		(1U << 0)
#define SR_RXNE			(1U << 0)
#define SR_TXE			(1U << 1)
#define SR_BSY			(1U << 7)
#define CR1_BR_BIT		(1U << 3)
#define CR2_TXDMAEN		(1U << 1)
#define CR2_RXDMAEN		(1U << 0)
#define GPIOA_PIN9		(1U << 9)
#define GPIOA_PIN4		(1U << 4)
#define SPI_CS			GPIOA_PIN9
#define SPI_CE			GPIOA_PIN4

enum
{
	F_PCLK_DIV_2,
	F_PCLK_DIV_4,
	F_PCLK_DIV_8,
	F_PCLK_DIV_16,
	F_PCLK_DIV_32,
	F_PCLK_DIV_64,
	F_PCLK_DIV_128,
	F_PCLK_DIV_256,
};

void spi_gpio_init(void);
void spi1_config(void);
void spi1_transmit(uint8_t *data, uint32_t size);
void spi1_receive(uint8_t *data, uint32_t size);
void cs_enable(void);
void cs_disable(void);
void ce_low(void);
void ce_high(void);
void spi1_write_byte(uint8_t data);
uint8_t spi1_read_byte(void);
uint8_t spi1_transfer(uint8_t data);
void spi1_transfer_dma(uint8_t *tx_buffer, uint8_t *rx_buffer, uint16_t len);

#endif /* SPI_H_ */
