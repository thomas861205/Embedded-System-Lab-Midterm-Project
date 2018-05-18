import time
import subprocess
from subprocess import CalledProcessError
from serial_put import serialPut


def miniShell(cmd = "", user = "105061110"):
    while True:
        cmd = raw_input(">>> ")

        if cmd == "":
            print "leave shell"
            break

        else:
            try:
                response = subprocess.check_output(cmd, shell=True)
            except CalledProcessError, err:
                print err
            else:
                try:
                    directory = "../user/{0}/shell_response".format(user)
                    # directory = "~/ee2405/midterm/user/105061110/shell_response"
                    f = open(directory, "w")
                except:
                    print "Unable to open " + directory
                else:
                    f.write(response)
                    f.close()

                    serialPut(directory)
                finally:
                    print response

def oneTimeShell(cmd = "", user = "105061110"):
    # cmd = raw_input(">>> ")

    if cmd == "":
        print "leave shell"
        return

    else:
        try:
            response = subprocess.check_output(cmd, shell=True)
        except CalledProcessError, err:
            print err
        else:
            try:
                directory = "../user/{0}/shell_response".format(user)
                # directory = "~/ee2405/midterm/user/105061110/shell_response"
                f = open(directory, "w")
            except:
                print "Unable to open " + directory
            else:
                for line in response.splitlines():
                    cut = line[0:15] + '\n'
                    f.write(cut)

                f.close()

                serialPut(directory)
            finally:
                # print response
                pass

if __name__ == "__main__":
    miniShell()
