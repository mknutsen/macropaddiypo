from serial import Serial

with Serial(port="/dev/tty.usbmodem2301", baudrate=115200) as ser:
    while True:
        bs = ser.read_until().decode('utf-8')
        print(bs)
        ser.write("hi back!\n".encode('utf-8'))
