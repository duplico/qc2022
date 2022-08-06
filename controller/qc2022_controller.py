import struct
from collections import namedtuple

import serial
import click

CONTROLLER_ID = 251
CRC_SEED = 0xA6F8

SERIAL_OPCODE_HELO = 0x01
SERIAL_OPCODE_ACK = 0x02
SERIAL_OPCODE_SETID = 0x0C
SERIAL_OPCODE_STATQ = 0x0D

MESSAGE_FMT = '<HBBIIH'
MESSAGE_FMT_NOCRC = '<HBBII'
SerialHeader = namedtuple('Message', 'from_id opcode clock_is_set last_clock payload crc16')
CRC_FMT = '<H'

def crc16_buf(sbuf):
    crc = CRC_SEED

    for b in sbuf:
        crc = (0xFF & (crc >> 8)) | ((crc & 0xFF) << 8)
        crc ^= b
        crc ^= (crc & 0xFF) >> 4
        crc ^= 0xFFFF & ((crc << 8) << 4)
        crc ^= ((crc & 0xff) << 4) << 1

    return crc

# def validate_message(msg):
#     if len(msg) < 11:
#         raise TimeoutError("No response from badge.")
#     if (msg[0] != 0xAC):
#         raise ValueError("Bad sync byte received.")
#     if crc16_buf(msg[1:-2]) != struct.unpack(CRC_FMT, msg[-2:])[0]:
#         print(crc16_buf(msg[1:-2]))
#         print(struct.unpack(CRC_FMT, msg[-2:]))
#         print(msg)
#         raise ValueError("Bad CRC from badge.")

# def await_serial(ser, opcode=None):
#     resp = ser.read(11)
#     validate_message(resp)
#     msg = SerialHeader._make(struct.unpack(HEADER_FMT, resp[1:]))
#     if opcode and msg.opcode != opcode:
#         raise ValueError("Unexpected opcode received: %d" % msg.opcode)
#     return msg

# def await_serial(ser, opcode=None):
#     resp = ser.read(11)
#     validate_message(resp)
#     msg = SerialHeader._make(struct.unpack(HEADER_FMT, resp[1:]))
#     if opcode and msg.opcode != opcode:
#         raise ValueError("Unexpected opcode received: %d" % msg.opcode)
#     if msg.payload_len:
#         payload = ser.read(msg.payload_len)
#         if len(payload) != msg.payload_len:
#             raise TimeoutError()
#         # TODO: payload_struct
#         return msg, payload
#     return msg, None

# def await_ack(ser):
#     header, payload = await_serial(ser, opcode=SERIAL_OPCODE_ACK)
#     return header.from_id

def time_seconds():
    return 0

def send_message(ser, opcode, payload=0x00000000, src_id=CONTROLLER_ID):
    msg = struct.pack(MESSAGE_FMT_NOCRC, src_id, opcode, 1, time_seconds(), payload)
    msg += struct.pack(CRC_FMT, crc16_buf(msg))
    ser.write(b'\xAC') # SYNC byte
    ser.write(msg)

def serial_obj(port):
    return serial.Serial(port, 230400, parity=serial.PARITY_NONE)

@click.command()
@click.argument('port')
def set_time(port):
    # TODO: Send a HELO with the time
    pass

@click.command()
@click.argument('port')
@click.argument('id', type=int)
def set_id(port, id):
    send_message(
        serial_obj(port),
        SERIAL_OPCODE_SETID,
        payload=id
    )
    # TODO: await ACK with payload of ID
    pass

@click.command()
@click.argument('port')
def statq(port):
    pass

@click.group(commands=[set_id, statq, set_time])
def controller():
    pass

if __name__ == '__main__':
    controller()
