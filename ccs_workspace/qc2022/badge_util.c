/*
 * badge_util.c
 *
 *  Created on: Jul 17, 2022
 *      Author: george
 */

#include <stdint.h>

#include "badge.h"

/// Given a standard buffer of bitfields, check whether ``id``'s bit is set.
uint8_t check_id_buf(uint8_t id, uint8_t *buf) {
    uint8_t byte;
    uint8_t bit;
    byte = id / 8;
    bit = id % 8;
    return (buf[byte] & (0x01 << bit)) ? 1 : 0;
}

/// In a standard buffer of bitfields, set ``id``'s bit.
void set_id_buf(uint8_t id, uint8_t value, uint8_t *buf) {
    uint8_t byte;
    uint8_t bit;
    byte = id / 8;
    bit = id % 8;
    if (value) {
        buf[byte] |= (0x01 << bit);
    } else {
        buf[byte] &= ~(0x01 << bit);
    }
}

/// Counts the bits set in a byte and return the total.
/**
 ** This is the Brian Kernighan, Peter Wegner, and Derrick Lehmer way of
 ** counting bits in a bitstring. See _The C Programming Language_, 2nd Ed.,
 ** Exercise 2-9; or _CACM 3_ (1960), 322.
 */
uint8_t byte_rank(uint8_t v) {
    uint8_t c;
    for (c = 0; v; c++) {
        v &= v - 1; // clear the least significant bit set
    }
    return c;
}

/// Counts the bits set in all the bytes of a buffer and returns it.
/**
 ** This is the Brian Kernighan, Peter Wegner, and Derrick Lehmer way of
 ** counting bits in a bitstring. See _The C Programming Language_, 2nd Ed.,
 ** Exercise 2-9; or _CACM 3_ (1960), 322.
 */
uint16_t buffer_rank(uint8_t *buf, uint8_t len) {
    uint16_t count = 0;
    uint8_t c, v;
    for (uint8_t i=0; i<len; i++) {
        v = buf[i];
        for (c = 0; v; c++) {
            v &= v - 1; // clear the least significant bit set
        }
        count += c;
    }
    return count;
}

uint8_t is_uber(uint8_t id) {
    return (id > BADGE_ID_UBER_START) && (id < BADGE_ID_UBER_START + BADGE_ID_UBER_COUNT);
}
