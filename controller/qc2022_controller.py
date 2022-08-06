import struct
from collections import namedtuple

import serial
import click

HEADER_FMT_NOCRCs   = '<BBHH'
HEADER_FMT   = '<BBHBBHH'
SerialHeader = namedtuple('Message', 'opcode payload_len from_id badge_type spare crc16_payload crc16_header')
CRC_FMT = '<H'

QC16_CRC_SEED = 0xA6F8

def crc16_buf(sbuf):
    crc = QC16_CRC_SEED

    for b in sbuf:
        crc = (0xFF & (crc >> 8)) | ((crc & 0xFF) << 8)
        crc ^= b
        crc ^= (crc & 0xFF) >> 4
        crc ^= 0xFFFF & ((crc << 8) << 4)
        crc ^= ((crc & 0xff) << 4) << 1

    return crc

def validate_message(msg):
    if len(msg) < 11:
        raise TimeoutError("No response from badge.")
    if (msg[0] != 0xAC):
        raise ValueError("Bad sync byte received.")
    if crc16_buf(msg[1:-2]) != struct.unpack(CRC_FMT, msg[-2:])[0]:
        print(crc16_buf(msg[1:-2]))
        print(struct.unpack(CRC_FMT, msg[-2:]))
        print(msg)
        raise ValueError("Bad CRC from badge.")

def await_serial(ser, opcode=None):
    resp = ser.read(11)
    validate_message(resp)
    msg = SerialHeader._make(struct.unpack(HEADER_FMT, resp[1:]))
    if opcode and msg.opcode != opcode:
        raise ValueError("Unexpected opcode received: %d" % msg.opcode)
    return msg

def await_serial(ser, opcode=None):
    resp = ser.read(11)
    validate_message(resp)
    msg = SerialHeader._make(struct.unpack(HEADER_FMT, resp[1:]))
    if opcode and msg.opcode != opcode:
        raise ValueError("Unexpected opcode received: %d" % msg.opcode)
    if msg.payload_len:
        payload = ser.read(msg.payload_len)
        if len(payload) != msg.payload_len:
            raise TimeoutError()
        # TODO: payload_struct
        return msg, payload
    return msg, None

def await_ack(ser):
    header, payload = await_serial(ser, opcode=SERIAL_OPCODE_ACK)
    return header.from_id

def send_message(ser, opcode, payload=b'', src_id=CONTROLLER_ID, dst_id=SERIAL_ID_ANY):
    msg = struct.pack(HEADER_FMT_NOCRCs, opcode, len(payload), src_id, dst_id)
    msg += struct.pack(CRC_FMT, crc16_buf(payload) if payload else 0x00) # No payload.
    msg += struct.pack(CRC_FMT, crc16_buf(msg))
    msg += payload
    ser.write(b'\xAC') # SYNC byte
    ser.write(msg)

def connect(ser):
    """Perform an initial connection to a badge.
    
    Raises all errors that `validate_header` can raise.
    """
    send_message(ser, SERIAL_OPCODE_HELO)
    return await_ack(ser)

def connect_poll(ser):
    """Attempt to connect to a badge, returning True if successful and False on timeout."""
    try:
        return connect(ser)
    except TimeoutError:
        return None

if __name__ == '__main__':
     # pyserial object, with a 1 second timeout on reads.
    ser = serial.Serial(args.port, 230400, parity=serial.PARITY_NONE, timeout=args.timeout)
