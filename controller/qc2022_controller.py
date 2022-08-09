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
SerialMessage = namedtuple('Message', 'from_id opcode clock_is_set last_clock payload crc16')
CRC_FMT = '<H'

ser = None

def crc16_buf(sbuf):
    crc = CRC_SEED

    for b in sbuf:
        crc = (0xFF & (crc >> 8)) | ((crc & 0xFF) << 8)
        crc ^= b
        crc ^= (crc & 0xFF) >> 4
        crc ^= 0xFFFF & ((crc << 8) << 4)
        crc ^= ((crc & 0xff) << 4) << 1

    return crc

def validate_message(msg):
    if len(msg) < 15:
        raise TimeoutError("No response from badge.")
    if (msg[0] != 0xAC):
        raise ValueError("Bad sync byte received.")
    if crc16_buf(msg[1:-2]) != struct.unpack(CRC_FMT, msg[-2:])[0]:
        print(crc16_buf(msg[1:-2]))
        print(struct.unpack(CRC_FMT, msg[-2:]))
        print(msg)
        raise ValueError("Bad CRC from badge.")

def await_serial(ser, opcode=None):
    resp = ser.read(14+1)
    validate_message(resp)
    msg = SerialMessage._make(struct.unpack(MESSAGE_FMT, resp[1:]))
    if opcode and msg.opcode != opcode:
        raise ValueError("Unexpected opcode received: %d" % msg.opcode)
    return msg

def await_ack(ser, payload=None):
    msg = await_serial(ser, opcode=SERIAL_OPCODE_ACK)
    return msg

def time_seconds():
    # TODO:
    return 0

def send_message(ser, opcode, payload=0x00000000, src_id=CONTROLLER_ID):
    msg = struct.pack(MESSAGE_FMT_NOCRC, src_id, opcode, 1, time_seconds(), payload)
    msg += struct.pack(CRC_FMT, crc16_buf(msg))
    ser.write(b'\xAC') # SYNC byte
    ser.write(msg)

def serial_obj(port):
    return serial.Serial(port, 230400, parity=serial.PARITY_NONE, timeout=1)

@click.command()
@click.argument('seconds', type=int)
def set_time(seconds):
    # TODO: Send a STATQ with the time
    pass

@click.command()
@click.argument('id', type=int)
def set_id(id):
    send_message(
        ser,
        SERIAL_OPCODE_SETID,
        payload=id
    )
    id_set = await_ack(ser)
    if (id_set != id):
        raise ValueError("Failed to set ID")
    print("ID set to %d" % id_set)

@click.command()
def statq():
    send_message(
        ser,
        SERIAL_OPCODE_STATQ,
    )
    stat_dump = await_ack(ser)
    # TODO: Decode stat_dump
    print(stat_dump)

@click.group(commands=[set_id, statq, set_time])
@click.argument('port')
def controller(port):
    global ser
    ser = serial_obj(port)

if __name__ == '__main__':
    controller()
