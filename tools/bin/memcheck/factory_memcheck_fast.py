import os
import sys
import serial
import subprocess

project_path = os.path.dirname(__file__)

if __name__ == "__main__":
    print("demo exe run start")
    burntools = open("burntools.txt").readlines()
    com_num = open("com.txt").readlines()
    com = ' -com:' + com_num[0]
    switchbaud = ' -switchbaud:750000'
    bin = ' -bin:' + os.path.join('fwpkg', 'factory_memcheck_fast.fwpkg')
    bin_select = ' -onlyburn:ssb.bin'
    chip_select = ' -3x'
    args = burntools[0] + com + bin + switchbaud + bin_select + chip_select
    subprocess.run(args)

    com = 'com' + com_num[0]
    baud = 115200
    ser = serial.Serial(com, baud)
    print("\n---------------------------------------------------------------")
    print("                     Start Memory Check                        ")
    print("---------------------------------------------------------------\n")
    while True:
        data = ser.readline()
        print(str(data[:-2], 'utf-8'))
