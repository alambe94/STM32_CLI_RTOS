import sys
import serial
import time

serial_port = None
ser_open = False

if __name__ == "__main__":

    print(sys.argv)

    if len(sys.argv) >= 2:

        port = sys.argv[1]
        baud = 38400

        try:
            serial_port = serial.Serial(port, baud, timeout=0.1)
            ser_open = True
        except:
            print("not valid port")
    else:
        print("please enter port")

    if(ser_open):
        print("serial port opened")

        while(True):

            cmd = "run-time-stats\r\n"
            serial_port.write(bytes(cmd, 'utf-8'))
            str_lines = serial_port.readlines()
            print("\033[6;3H")
            for x in range(len(str_lines)):
                print(str_lines[x].decode(), end='')

        serial_port.close()
