/*
 * badge.h
 *
 *  Created on: Jun 26, 2022
 *      Author: george
 */

#ifndef BADGE_H_
#define BADGE_H_

#define BADGES_IN_SYSTEM 170
#define BADGE_ID_UBER_START 151
#define BADGE_ID_UBER_COUNT 20
#define BADGE_ID_UNASSIGNED 150
#define CONTROLLER_ID 251

// SMCLK rate in Hz:
#define SMCLK_RATE_HZ 8000000

// Dot correction configuration for the three colors:
#define TLC_DC_RED 0x20
#define TLC_DC_GRN 0x19
#define TLC_DC_BLU 0x2a

// Global brightness correct:
#define TLC_BC 0x00 // 25%

// Badge config struct definition
typedef struct {
    /// The badge's ID, between CBADGE_ID_START and CBADGE_ID_MAX_UNASSIGNED
    uint16_t badge_id;
    /// Whether the badge has a config created for it.
    uint8_t initialized;
    /// Whether the badge has been assigned an ID or is otherwise "in use"
    uint8_t in_service;
    /// The currently selected element on this badge
//    element_type element_selected;
    uint8_t badges_seen[32];
} badge_conf_t;

// Global-ish variables:
extern badge_conf_t badge_conf;
extern volatile uint8_t f_time_loop;

// Utility functions:
uint8_t check_id_buf(uint8_t id, uint8_t *buf);
void set_id_buf(uint8_t id, uint8_t *buf);
uint8_t byte_rank(uint8_t v);
uint16_t buffer_rank(uint8_t *buf, uint8_t len);

#endif /* BADGE_H_ */
