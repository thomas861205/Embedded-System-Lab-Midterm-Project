import serial
import time
from serial.serialutil import SerialException
from XBee_host import getGesture

s = serial.Serial("/dev/ttyUSB0")
# ss = serial.Serial("/dev/ttyACM0")


def sendSingleMessege(messege="3=D"):
    for char in messege:
        print char,
        s.write(char)
        time.sleep(.02)
    print "Done"


def readFileByChar():
    user = "105061110"
    filename = "project_list"
    # file = open('../text/formated_result', 'r')
    # file = open('../text/result', 'r')
    file = open("../user/{0}/{1}".format(user, filename), 'r')
    content = file.read()
    file.close()

    number_of_line = 0

    for char in content:
        if not char:
            break
        if char == '\n':
            number_of_line += 1

        if number_of_line == 16:
            raw_input()
            s.write('\n')
            number_of_line = 0
        else:
            s.write(char)
            time.sleep(.02)


def readFileByLine():
    user = "105061110"
    filename = "project_list"
    file = open('../user/{0}/result'.format(user), 'r')
    # file = open('../text/test', 'r')
    # file = open("../user/{0}/{1}".format(user, filename), 'r')
    content = file.readlines()
    file.close()

    number_of_line = 0
    i = 0

    # print len(content) // 34
    while i <= len(content):
        if i == len(content):
            j = 0
            while j < 16 - number_of_line:
                j += 1
                s.write("\n*")
                time.sleep(.02)
            number_of_line = 16
            scroll = raw_input()
            if scroll == "w":
                i = i - number_of_line - 17 + 1
                number_of_line = 0
                if i == 0:
                    s.write("\n")
            continue

        line = content[i]
        number_of_line += 1
        i += 1

        if number_of_line == 17:
            scroll = raw_input()
            if scroll == "w":
                i -= 17
                number_of_line = 0
                if i == 0:
                    s.write("\n")
            else:
                number_of_line = 1
                s.write("\n")
                time.sleep(.02)
                for char in line[0:-1]:
                    s.write(char)
                    time.sleep(.02)

        else:
            if i != 1:
                s.write("\n")
                time.sleep(.02)
            for char in line[0:-1]:
                s.write(char)
                time.sleep(.02)

def sendOneChar(char = ""):
    s.write(char)
    time.sleep(.02) 

def serialPut(path , user = "105061110"):
    filename = "project_list"

    try:
        file = open(path, 'r')
        # file = open("../user/{0}/{1}".format(user, filename), 'r')
    except:
        print "Unable to open" + path
    else:
        content = file.readlines()
        file.close()

        number_of_line = 0
        i = -1

        while i < len(content) - 1:
            i += 1
            number_of_line += 1

            line = content[i]

            if number_of_line == 17:
                number_of_line = 0

                while True:
                    try:

                        # [gesture, time stamp]
                        scroll = getGesture(False)[0]

                        # scroll = s.readline()
                        # scroll = raw_input()
                    except SerialException, err:
                        print err
                    else:
                        if scroll == "RIGHT":
                            break
                        if scroll == "LEFT":
                            i -= (16 * 2)
                            if i < 0:
                                i = 0
                            if i == 0:
                                # print
                                s.write("\n")
                                time.sleep(.02)
                            break

                i -= 1
            else:
                if i != 0:
                    # print
                    s.write("\n")
                    time.sleep(.02)
                for char in line[0:-1]:
                    # print char,
                    s.write(char)
                    time.sleep(.02)

        print "Done serial put"

        scroll = getGesture(False)[0]
        
        if scroll:
            s.write('`')
            

if __name__ == "__main__":
    serialPut()

    s.close()
