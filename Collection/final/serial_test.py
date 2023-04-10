#!/usr/bin/env python3
import serial
import time

if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
    ser.reset_input_buffer()

    ser.reset_output_buffer()
    ser.write(b"Input: a\n")
    time.sleep(4)

    line = ser.readline().decode('utf-8').rstrip()
    print(line)

    ser.reset_output_buffer()
    ser.write(b"Input: f\n")
    time.sleep(4)

    line = ser.readline().decode('utf-8').rstrip()
    print(line)

    ser.reset_output_buffer()
    ser.write(b"Input: b\n")
    time.sleep(4)

    line = ser.readline().decode('utf-8').rstrip()
    print(line)

    ser.reset_output_buffer()
    ser.write(b"Input: c\n")
    time.sleep(4)

    line = ser.readline().decode('utf-8').rstrip()
    print(line)

    ser.reset_output_buffer()
    ser.write(b"Input: e\n")
    time.sleep(4)

    line = ser.readline().decode('utf-8').rstrip()
    print(line)