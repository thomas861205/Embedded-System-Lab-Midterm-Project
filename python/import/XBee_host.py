import serial
import time

serdev = '/dev/ttyUSB0'
s = serial.Serial(serdev, 9600)

def setupXbee():
    """
    XBee setting
    """
    s.write("+++")
    char = s.read(2)
    print("Enter AT mode.")
    print(char)

    s.write("ATMY 0x12\r\n")
    char = s.read(3)
    print("Set MY 0x12.")
    print(char)

    s.write("ATDL 0x10\r\n")
    char = s.read(3)
    print("Set DL 0x10.")
    print(char)

    s.write("ATWR\r\n")
    char = s.read(3)
    print("Write config.")
    print(char)

    s.write("ATMY\r\n")
    char = s.read(3)
    print("MY :")
    print(char)

    s.write("ATDL\r\n")
    char = s.read(3)
    print("DL : ")
    print(char)

    s.write("ATCN\r\n")
    char = s.read(3)
    print("Exit AT mode.")
    print(char)


def getGesture(print_time = True):
    line = s.readline()
    gesture_event = [line[0:-1], time.time()]
    
    if print_time:
        print gesture_event

    return gesture_event

if __name__ == "__main__":
    # setupXbee()

    while True:
        # getGesture(True)
        line = raw_input()
        for char in line:
            s.write(char)
            time.sleep(.02)
    s.close()
