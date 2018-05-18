"""
center_control.py

    the central control program running on PC to handle task from Xbee
"""

from XBee_host import setupXbee, getGesture
from serial_put import serialPut, sendOneChar
from shell_command import oneTimeShell
from test_selenium import git_lab_inquiry
# from getKeycode import *

user = "105061110"


class finite_state_machine:

    def __init__(self):

        self.state = "menu"
        self.curr_project = 0   # for git lab project name
        self.curr_file = 0      # for gtt lab file name
        self.curr_code = 0      # for shell file(code) name

    def transiton(self, gesture):

        if self.state == "menu":
            if gesture == "LEFT":
                self.state = "git lab projects"

                print "Current state: ", self.state

                sendOneChar('\a')

                serialPut('../user/105061110/result', user)

                self.state = "menu"

                print "Current state: ", self.state

                sendOneChar('\f')

            elif gesture == "UP":

                self.state = "crawler"

                print "Current state: ", self.state

                sendOneChar('\a')

                sendOneChar('\b')

                git_lab_inquiry(user)

                sendOneChar('\v')

                self.state = "menu"

                print "Current state: ", self.state

                sendOneChar('\f')

            elif gesture == "RIGHT":

                self.state = "shell"

                print "Current state: ", self.state

                sendOneChar('`')

                gesture = getGesture(False)[0]

                cmd = ""

                if gesture == "UP":

                    cmd = "ls"

                    print "Current command: ", cmd

                elif gesture == "LEFT":
                    # leave shell
                    cmd = ""

                    print "Current command: ", cmd

                elif gesture == "RIGHT":

                    cmd = "git status"

                    print "Current command: ", cmd

                elif gesture == "DOWN":

                    cmd = "pwd"

                    print "Current command: ", cmd

                sendOneChar('\a')

                oneTimeShell(cmd, user)

                self.state = "menu"

                print "Current state: ", self.state

                sendOneChar('\f')

        elif self.state == "shell":

            if gesture == "LEFT":

                self.state = "menu"


def debug():
    print "Press Esc twice to leave..."

    sekretariat = finite_state_machine()
    print "Current state: ", sekretariat.state

    esc_esc = False
    direction = ""

    while True:
        cmd = get()

        if cmd == 27:
            direction = ""
            if esc_esc:
                print "Leaving center control..."
                break
            else:
                esc_esc = True

        elif cmd == 91 and esc_esc:
            arrow = get()
            if arrow == 68:
                direction = "LEFT"
            elif arrow == 65:
                direction = "UP"
            elif arrow == 67:
                direction = "RIGHT"
            elif arrow == 66:
                direction = "DOWN"
            esc_esc = False

        else:
            esc_esc = False
            direction = ""

        if direction != "":
            sekretariat.transiton(direction)
            print "Current state: ", sekretariat.state


def main():
    # setupXbee()

    last_event_time = 0

    print "welcome Master"

    function_panel = finite_state_machine()
    print "Current state: ", function_panel.state

    while True:

        # false : don't print the time stamp
        # cmd = [gesture, time stamp]
        (gesture, time_stamp) = getGesture(False)

        if time_stamp != last_event_time:

            last_event_time = time_stamp

            function_panel.transiton(gesture)


if __name__ == "__main__":
    main()
