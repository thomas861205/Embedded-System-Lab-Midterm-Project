from selenium import webdriver
import subprocess
from subprocess import CalledProcessError
# from pyvirtualdisplay import Display

def git_lab_inquiry(user = "105061110"):

    # create a directory for new users
    try:
        cmd = "mkdir ../user/{0}/".format(user)
        response = subprocess.check_output(cmd, shell=True)
    except:
        print "already exist"
    #


    # start parsing
    menu_url = "http://gitlab.larc-nthu.net/users/{0}/projects".format(user)

    driver = webdriver.PhantomJS(executable_path="/usr/bin/phantomjs")
    # driver = webdriver.Chrome("/usr/bin/chromedriver")
    driver.get(menu_url)
    #

    # switch to user's git lab
    user_link_url = "http://gitlab.larc-nthu.net/{0}/".format(user)

    # extract the project name tags
    project_name_tags = driver.find_elements_by_class_name("project-name")

    # store project names
    project_names = [tag.text for tag in project_name_tags]

    # create a list of target urls
    project_links = [user_link_url + name for name in project_names]

    # key: project name, value: file name
    files_in_projects = dict()

    # create a file content all project names
    try:
        directory2 = "../user/{0}/project_list".format(user)
        print directory2
        g = open(directory2, "w")
    except:
        print "Unable to open {0}".format(directory2)
    #

    # iterate the project name and url parallelly

    order_of_line = 0
    for project_name, project_url in zip(project_names, project_links):

        # write the project name in "../user/{user}/project_list"
        # if order_of_line != 0:
        #     g.write("\n")
        g.write("#{0}\n".format(project_name[0:16]))
        order_of_line += 1

        # switch to project url
        driver.get(project_url)

        # extract file name tags
        files_in_project_tags = driver.find_elements_by_class_name(
            "str-truncated")

        # save the file name in dict
        files_in_projects[project_name] = [
            tag.text for tag in files_in_project_tags]

        # finish this project
        print "Done " + project_name


    # save the project name and file name in the same file
    while True:
        try:
            directory = "../user/{0}/result".format(user)
            print directory
            f = open(directory, "w")



        except IOError, err:
            print err
            yes_or_no = raw_input("Try again?[Y\\n]")
            if yes_or_no == "Y" or yes_or_no == "y":
                pass
            elif yes_or_no == "N" or yes_or_no == "n":
                break
            else:
                pass
        else:
            for project_name in files_in_projects.keys():
                f.write("#{0}\n".format(project_name).encode('utf-8'))
                for file_name in files_in_projects[project_name]:
                    line = "-" + "{0}\n".format(file_name[0:16])
                    f.write(line.encode('utf-8'))
                f.write("\n".encode('utf-8'))

            print "Done dumping"
            # format_text(directory)
            # print "Done formating"
            f.close()
            g.close()
            break
    #

    # done using the driver
    driver.close()


def git_lab_inquiry2():
    user = "105061110"
    # user = raw_input("user: ")
    menu_url = "http://gitlab.larc-nthu.net/users/{0}/projects".format(user)

    driver = webdriver.PhantomJS(executable_path="/usr/bin/phantomjs")
    driver.get(menu_url)

    user_link_url = "http://gitlab.larc-nthu.net/{0}/".format(user)
    project_name_tags = driver.find_elements_by_class_name("project-name")
    project_names = [tag.text for tag in project_name_tags]
    project_links = [user_link_url + name for name in project_names]

    files_in_projects = dict()

    for project_name, project_url in zip(project_names, project_links):

        driver.get(project_url)
        files_in_project_tags = driver.find_elements_by_class_name(
            "str-truncated")
        files_in_projects[project_name] = [
            tag.text for tag in files_in_project_tags]
        print "Done " + project_name

    for project_name in files_in_projects.keys():
        directory = "~/ee2405/midterm/user/{0}/{1}".format(user, project_name)
        # directory = "../user/{0}".format(user)
        try:
            project_file = open(directory, "w+")
        except:
            print "cannot open directory '{0}'".format(project_name)            
        else:
            print "Open {0} successfully".format(directory)
            project_file.write("{0}\n".format(project_name).encode('utf-8'))
            project_file.close()

            for file_name in files_in_projects[project_name]:
                try:
                    project_file = open(directory, "w+")
                except:
                    print "cannot open directory '{0}'".format(project_name)
                else:
                    print "Open {0} successfully".format(directory)
                    line = "+" + "{0}\n".format(file_name)
                    project_file.write(line.encode('utf-8'))
                    project_file.close()
                # project_file.write("\n".encode('utf-8'))

    print "Done dumping"

    driver.close()


def debug():
    try:
        user = "105061110"
        # cmd = "ls ~/ee2405/midterm/user"
        cmd = "mkdir ~/ee2405/midterm/user/{0}".format(user)
        response = subprocess.check_output(cmd, shell=True)
    except CalledProcessError, err:
        print "cannot create directory '{0}': File exists".format(user)
    else:
        response = subprocess.check_output("ls {0}".format(cmd), shell=True)
        print response

if __name__ == "__main__":
    # display = Display(visible=0, size=(800, 800))  
    # display.start()
    git_lab_inquiry()
    # git_lab_inquiry2()
    # debug()
