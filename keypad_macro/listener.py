import serial
import pyautogui
import os

# Define the COM port and baud rate for Arduino communication
COM_PORT = 'COM5'
BAUD_RATE = 9600

# Open the serial connection to Arduino
arduino = serial.Serial(COM_PORT, BAUD_RATE)

def type_from_file(file_name):
    try:
        # Get the path to the current script
        script_dir = os.path.dirname(os.path.abspath(__file__))

        # Construct the file path for the text file in the same folder
        file_path = os.path.join(script_dir, file_name)

        # Open the text file in read mode
        with open(file_path, 'r') as file:
            # Read the content of the file
            content = file.read()

        # Use pyautogui.write() to paste the content
        pyautogui.write(content)

        print("Content from the file has been pasted.")
    except FileNotFoundError:
        print(f"Error: File '{file_name}' not found.")
    except Exception as e:
        print(f"Error: {e}")


# Main program loop
while True:
    # Read a line from Arduino
    line = arduino.readline().decode().strip()
    
    # Check if a key is pressed
    if line:
        if line == 'A0':
            pass
        elif line == 'A1':
          
            pass
        elif line == 'B0':
            file_path = 'tests/basic.txt'
            type_from_file(file_path)   
        
            pass
        elif line == 'C1':
            pass

        pass

# Close the serial connection
arduino.close()
