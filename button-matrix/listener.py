import serial
import subprocess
import os
from file_paths import wmplayer, mp3_path

# Define the COM port and baud rate for Arduino communication
COM_PORT = 'COM5'
BAUD_RATE = 9600

# Open the serial connection to Arduino
arduino = serial.Serial(COM_PORT, BAUD_RATE)

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

# Main program loop
while True:
    # Read a line from Arduino
    line = arduino.readline().decode().strip()
    print(line)
    # Check if a key is pressed
    if line:
        # Check which key is pressed
        if line == '1':
            print("Play Backtrack")
            mp3_file_path =  mp3_path + r'\1.mp3'
            play_mp3_file(mp3_file_path)

        elif line == '2':
            print("Play Backtrack")
            mp3_file_path = mp3_path + r'\2.mp3'
            play_mp3_file(mp3_file_path)
        elif line == '*':
            stop_music()


   
# Close the serial connection
arduino.close()
