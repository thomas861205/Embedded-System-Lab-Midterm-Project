# 1. K66F

The code that K66F runs connects the board to the smart phone through Bluetooth and handles movement commands to run the boebot.

# 2. K64F

The code that K64F runs contains the IR gesture, XBee, uLCD modules. Using hand gestures as input, the corresponding function is determined and the command will be sent to the PC through XBee, and the result will be display on the uLCD.

# 3. PC

PC runs python codes "main.py", which intakes the command from K64F and executes. It supports the following functions:

- Swipe UP for [git lab crawler](#git lab crawler)

- Swipe LEFT to display the content of your git lab repositories

- Swipe RIGHT to select some basic shell commands

# <a name="git lab crawler"></a> git lab crawler