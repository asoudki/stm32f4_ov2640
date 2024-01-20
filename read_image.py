import serial
from binascii import unhexlify
import random

# change COM port to wherever it's coming from
camera = serial.Serial(port='COM5', baudrate=115200, bytesize=8)

i = 0

# continuously read in images from serial and write them to desktop
while True:
    # current image data is most easily stored as an array
    img = []

    prevByte = b""
    currByte = b""

    # the ending pattern for jpeg images is {\xff, \xd9}, use this to know when to stop reading for current image
    while (prevByte == b"\xff" and currByte == b"\xd9") == False:
        byte = camera.readline().strip()

        prevByte = currByte

        if len(byte) == 1:
            currByte = unhexlify(b'0' + byte)

            # handle start of image pattern by discarding current image and starting a new one
            if (prevByte == b"\xff" and currByte == b"\xd8"):
                img = [b"\xff", b"\xd8"]
            else:
                img.append(currByte)

        elif len(byte) == 2:
            currByte = unhexlify(byte)

            # handle start of image pattern by discarding current image and starting a new one
            if (prevByte == b"\xff" and currByte == b"\xd8"):
                img = [b"\xff", b"\xd8"]
            else:
                img.append(currByte)

        # print the current byte to show progress, comes out REALLY fast though
        print(currByte)
    

    # convert array of bytes into a byte string for writing
    data = b"".join(img)

    # write current image
    with open(f"test_images/recv_image_{i}.jpg", "wb") as f:
        f.write(data)
    i = i+1
 
camera.close()