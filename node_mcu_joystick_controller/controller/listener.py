import serial
import subprocess
import pyautogui
import os
from file_paths import wmplayer, mp3_path

# Define the COM port and baud rate for Arduino communication
COM_PORT = 'COM5'
BAUD_RATE = 115200

# Open the serial connection to Arduino
arduino = serial.Serial(COM_PORT, BAUD_RATE)

# Function to open CMD
def open_cmd():
    subprocess.Popen('start cmd.exe', shell=True)
    print("Opening windows CMD")


def play_mp3_file(file_path):
    print(file_path)
    try:
        subprocess.Popen([wmplayer, file_path])
        print("Starting music")
    except Exception as e:
        print("An error occurred while opening Windows Media Player:")
        print(str(e))

def stop_music():
    try:
        os.system("taskkill /f /im wmplayer.exe")
    except Exception as e:
        print("An error occurred while stopping the music:")
        print(str(e))

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

    import os

def create_folder(path):
    try:
        os.makedirs(path)
        print(f"Folder '{path}' created successfully.")
    except FileExistsError:
        print(f"Folder '{path}' already exists.")
    except OSError as e:
        print(f"Error creating folder '{path}': {e}")

def clone_git_repository(url, destination_path):
    try:
        print("Cloning")
        subprocess.run(["git", "clone", url,  os.path.join(destination_folder, "html-boilerplate")], check=True)
        print(f"Repository cloned successfully to '{destination_path}'.")
    except subprocess.CalledProcessError as e:
        print(f"Error cloning repository: {e}")

# Main program loop
while True:
    # Read a line from Arduino
    line = arduino.readline().decode().strip()
    print(line)
    # Check if a key is pressed
    if line:
        # Check which key is pressed
        if line == 'html-boilerplate':
            documents_folder = os.path.join(os.path.expanduser("~"), "Documents")
            generated_code_folder = os.path.join(documents_folder, "generated-code")
            if not os.path.exists(generated_code_folder):
                create_folder(generated_code_folder)

            git_repository_url = "https://github.com/T14g/html-boilerplate.git"  
            destination_folder = generated_code_folder
            clone_git_repository(git_repository_url, destination_folder)

        elif line == '2':
           # Open Notepad and type "Hello, World!"
            open_notepad_and_type()
        elif line == '3':
            # Do something else if key '3' is pressed
            pass
        elif line == 'backtrack1':
            # Do something else if key '3' is pressed
            print("Play Backtrack")
            mp3_file_path =  mp3_path + r'\1.mp3'
            play_mp3_file(mp3_file_path)
            pass

        elif line == 'backtrack2':
        # Do something else if key '3' is pressed
            print("Play Backtrack")
            mp3_file_path = mp3_path + r'\2.mp3'
            play_mp3_file(mp3_file_path)
            pass

        elif line == 'stopmusic':
            # Do something else if key '3' is pressed
            print("Stop Music")
            stop_music()
            pass

# Close the serial connection
arduino.close()
