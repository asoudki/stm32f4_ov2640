import serial
from binascii import unhexlify
import random
 
arduino = serial.Serial(port='COM4', baudrate=115200, bytesize=8)

i = 0

while True:
    img = []

    prevByte = b""
    currByte = b""

    while (prevByte == b"\xff" and currByte == b"\xd9") == False:
        byte = arduino.readline().strip()

        prevByte = currByte

        if len(byte) == 1:
            currByte = unhexlify(b'0' + byte)

            if (prevByte == b"\xff" and currByte == b"\xd8"):
                img = [b"\xff", b"\xd8"]
            else:
                img.append(currByte)

        elif len(byte) == 2:
            currByte = unhexlify(byte)

            if (prevByte == b"\xff" and currByte == b"\xd8"):
                img = [b"\xff", b"\xd8"]
            else:
                img.append(currByte)

        print(currByte)
    

    data = b"".join(img)

    print(img)

    with open(f"recv_image_{i}.jpg", "wb") as f:
        f.write(data)
    i = i+1
 
arduino.close()