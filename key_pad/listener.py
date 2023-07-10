import serial
import subprocess
import pyautogui

# Define the COM port and baud rate for Arduino communication
COM_PORT = 'COM5'
BAUD_RATE = 9600

# Open the serial connection to Arduino
arduino = serial.Serial(COM_PORT, BAUD_RATE)

# Function to open CMD
def open_cmd():
    subprocess.Popen('start cmd.exe', shell=True)
    print("Opening windows CMD")


# Function to open Notepad and type "Hello, World!"
def open_notepad_and_type():
    print("notepad")
    # Open Notepad
    pyautogui.press('win')
    pyautogui.write('notepad')
    pyautogui.press('enter')

    # Wait for Notepad to open
    pyautogui.sleep(1)

    # Type "Hello, World!"
    pyautogui.write('Hello, World!')

# Main program loop
while True:
    # Read a line from Arduino
    line = arduino.readline().decode().strip()
    
    # Check if a key is pressed
    if line:
        # Check which key is pressed
        if line == '1':
            # Open CMD if key '1' is pressed
            open_cmd()
        elif line == '2':
           # Open Notepad and type "Hello, World!"
            open_notepad_and_type()
        elif line == '3':
            # Do something else if key '3' is pressed
            pass

# Close the serial connection
arduino.close()
