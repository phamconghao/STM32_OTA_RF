#include "nrf24l01.h"
#include "spi.h"
#include "timer.h"

static NRF24L01_Config current_config;

static void write_register(uint8_t reg, uint8_t value)
{
	cs_disable();
	spi1_write_byte(CMD_W_REGISTER | reg);
	spi1_write_byte(value);
	cs_enable();
}

static uint8_t read_register(uint8_t reg)
{
	uint8_t result;

	cs_disable();
	spi1_write_byte(CMD_R_REGISTER | reg);
	result = spi1_transfer(0xFF);
	cs_enable();

	return result;
}

static void write_register_multi(uint8_t reg, uint8_t *data, uint8_t length)
{
	cs_disable();
	spi1_write_byte(CMD_W_REGISTER | reg);
	for (uint8_t i = 0; i < length; i++)
	{
		spi1_write_byte(data[i]);
	}
	cs_enable();
}

void nrf24l01_init(NRF24L01_Config *config)
{
	current_config = *config;

	ce_low();
	cs_enable();

	delay_ms(100);

	// Configure
	write_register(REG_CONFIG, 0x08);			// CRC 1 byte, power down
	write_register(REG_EN_AA, 0x01);			// Enable auto_ack pipe 0
	write_register(REG_EN_RXADDR, 0x01);		// Enable pipe 0
	write_register(REG_SETUP_AW, 0x03);			// 5 bytes address
	write_register(REG_SETUP_RETR, 0x1A);		// 500us, 10 retries
	write_register(REG_RF_CH, config->channel);
	write_register(REG_RF_SETUP, 0x07);			// 1Mbps, 0dBm

	// Address configure
	write_register_multi(REG_TX_ADDR, config->tx_address, 5);
	write_register_multi(REG_RX_ADDR_P0, config->rx_address, 5);

	// Payload size configure
	write_register(REG_RX_PW_P0, config->payload_size);
}

void nrf24l01_set_tx_mode(void)
{
	ce_low();
	write_register(REG_CONFIG, 0x0A);	// PWR_UP, PRIM_TX
	delay_ms(2);
}

void nrf24l01_set_rx_mode(void)
{
	ce_low();
	write_register(REG_CONFIG, 0x0B);	// PWR_UP, PRIM_RX
	ce_high();
	delay_ms(2);
}

bool nrf24l01_data_available(void)
{
	uint8_t status = read_register(REG_STATUS);
	return (status & (1 << RX_DR));
}

void nrf24l01_send_data(uint8_t *data, uint8_t length)
{
	cs_disable();
	spi1_write_byte(CMD_W_TX_PAYLOAD);
	for (uint8_t i = 0; i < length; i++)
	{
		spi1_write_byte(data[i]);
	}
	cs_enable();

	ce_high();
	delay_ms(1);
	ce_low();

	// Clear interrupt flag
	write_register(REG_STATUS, 0x70);
}

void nrf24l01_receive_data(uint8_t *data, uint8_t *length)
{
	cs_disable();
	spi1_write_byte(CMD_R_RX_PAYLOAD);
	for (uint8_t i = 0; i < current_config.payload_size; i++)
	{
		data[i] = spi1_read_byte();
	}
	cs_enable();

	*length = current_config.payload_size;

	// Clear interrupt flag
	write_register(REG_STATUS, 0x70);
}

void nrf24l01_set_channel(uint8_t channel)
{
	write_register(REG_RF_CH, channel);
	current_config.channel = channel;
}

void nrf24l01_power_up(void)
{
	uint8_t config = read_register(REG_CONFIG);
	write_register(REG_CONFIG, config | (1 << PWR_UP));	// Set PWR_UP bit
	delay_ms(5);
}

void nrf24l01_power_down(void)
{
	uint8_t config = read_register(REG_CONFIG);
	write_register(REG_CONFIG, config & ~(1 << PWR_UP)); // Clear PWR_UP bit
}


