

#ifndef NRF24L01_H_
#define NRF24L01_H_

#include <stdint.h>
#include <stdbool.h>

// Declaring some register of nRF24L01
#define REG_CONFIG		(0x00)
#define REG_EN_AA		(0x01)
#define REG_EN_RXADDR	(0x02)
#define REG_SETUP_AW	(0x03)
#define REG_SETUP_RETR	(0x04)
#define REG_RF_CH		(0x05)
#define REG_RF_SETUP	(0x06)
#define REG_STATUS		(0x07)
#define REG_RX_ADDR_P0	(0x0A)
#define REG_TX_ADDR		(0x10)
#define REG_RX_PW_P0	(0x11)
#define REG_FIFO_STATUS	(0x17)


// Declaring commands
#define CMD_R_REGISTER		(0x00)
#define CMD_W_REGISTER		(0x20)
#define CMD_R_RX_PAYLOAD	(0x61)
#define CMD_W_TX_PAYLOAD	(0xA0)
#define CMD_FLUSH_TX		(0xE1)
#define CMD_FLUSH_RX		(0xE2)
#define CMD_NOP				(0xFF)

// Config payload frame size
#define NRF24L01_PAYLOAD_SIZE	(32)

#define RX_DR	(6)
#define PWR_UP	(1)


// Storing configuration structure
typedef struct
{
	uint8_t channel;
	uint8_t payload_size;
	uint8_t address_width;
	uint8_t tx_address[5];
	uint8_t rx_address[5];
} NRF24L01_Config;

// Functions
void nrf24l01_init(NRF24L01_Config *config);
void nrf24l01_set_tx_mode(void);
void nrf24l01_set_rx_mode(void);
bool nrf24l01_data_available(void);
void nrf24l01_send_data(uint8_t *data, uint8_t length);
void nrf24l01_receive_data(uint8_t *data, uint8_t *length);
void nrf24l01_set_channel(uint8_t channel);
void nrf24l01_power_up(void);
void nrf24l01_power_down(void);

#endif /* NRF24L01_H_ */
