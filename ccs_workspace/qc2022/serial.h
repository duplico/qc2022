/*
 * serial.h
 *
 *  Created on: Jul 17, 2022
 *      Author: george
 */

#ifndef SERIAL_H_
#define SERIAL_H_

// Serial protocol details
#define SERIAL_PHY_SYNC_WORD 0xAC
#define CRC_SEED 0xA6F8

#define SERIAL_OPCODE_HELO      0x01
#define SERIAL_OPCODE_ACK       0x02
#define SERIAL_OPCODE_SETID     0x0C

#define SERIAL_RX_DONE 1

// Serial LL (link-layer) state machine states:
#define SERIAL_LL_STATE_IDLE 0
#define SERIAL_LL_STATE_NC1 1
#define SERIAL_LL_STATE_NC2 2
#define SERIAL_LL_STATE_C 3
#define SERIAL_LL_STATE_BLOCK 4

// Serial PHY states and configuration:
#define SERIAL_PHY_STATE_IDLE 0
#define SERIAL_PHY_STATE_RX 1
#define SERIAL_PHY_STATE_TX 2

#define SERIAL_IDLE_TIMEOUT_TICKS 50
#define SERIAL_NC_TIMEOUT_TICKS 10
#define SERIAL_C_TIMEOUT_TICKS 10
#define SERIAL_BLOCK_TIMEOUT_TICKS 500

// Types and structs
typedef struct {
    uint16_t proto_version;
    __packed uint8_t opcode;
    __packed uint8_t from_id;
    __packed uint32_t last_clock;
    __packed uint8_t clock_is_set;
    uint32_t payload;
    uint16_t crc16;
} serial_message_t;

// Variables
extern volatile uint8_t f_serial_phy;

extern volatile uint8_t serial_phy_state;
extern volatile serial_message_t serial_message_in;
extern serial_message_t serial_message_out;
extern uint8_t serial_ll_state;
extern uint16_t connected_badge_id;
extern uint8_t connected_badge_type;

// Functions
void serial_tick();
void serial_phy_handle_rx();
void serial_init();
uint16_t crc16_buf(volatile uint8_t *sbuf, uint8_t len);
uint16_t crc_build(uint8_t data, uint8_t start_over);
void crc16_apply(serial_message_t *message);
uint8_t validate_message(serial_message_t *message);

#endif /* SERIAL_H_ */
