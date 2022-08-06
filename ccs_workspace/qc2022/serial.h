/// Header file for serial communication module for the arm contacts.
/**
 ** \file serial.h
 ** \author George Louthan
 ** \date   2022
 ** \copyright (c) 2022 George Louthan @duplico. MIT License.
 */

#ifndef SERIAL_H_
#define SERIAL_H_

// Serial protocol details
/// Sync "word" (byte) that precedes all serial messages.
#define SERIAL_PHY_SYNC_WORD 0xAC

/// Common seed for CRC16s.
#define CRC_SEED 0xA6F8

/// Opcode for a pairing request message.
#define SERIAL_OPCODE_HELO      0x01
/// Opcode for an acknowledgment message, which has several purposes.
#define SERIAL_OPCODE_ACK       0x02
/// Opcode for a control message to set the badge ID.
#define SERIAL_OPCODE_SETID     0x0C
/// Opcode for a status request control message to dump stats.
#define SERIAL_OPCODE_STATQ     0x0D

// Serial PHY states and configuration:
/// Idle state of the serial PHY (either TX or RX status).
#define SERIAL_PHY_STATE_IDLE 0
/// Active state of the RX serial PHY state machine.
#define SERIAL_PHY_STATE_RX 1
/// Active state of the TX serial PHY state machine.
#define SERIAL_PHY_STATE_TX 2

/// Number of system ticks to wait for a link-layer timeout.
#define SERIAL_TIMEOUT_TICKS 10

// Types and structs
typedef struct {
    uint16_t from_id;
    __packed uint8_t opcode;
    __packed uint8_t clock_is_set;
    uint32_t last_clock;
    uint32_t payload;
    uint16_t crc16;
} serial_message_t;


extern volatile uint8_t f_serial_phy;
extern volatile uint8_t serial_phy_state;
extern volatile serial_message_t serial_message_in;
extern serial_message_t serial_message_out;
extern uint8_t serial_plugged;

void serial_tick();
void serial_phy_handle_rx();
void serial_init();
uint16_t crc16_buf(volatile uint8_t *sbuf, uint8_t len);
void crc16_apply(serial_message_t *message);
uint8_t validate_message(serial_message_t *message);

#endif /* SERIAL_H_ */
