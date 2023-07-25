#include <Wire.h>
#include <Keypad.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

// Set the LCD address
const int I2C_ADDR = 0x27;  // Replace with your LCD's address

// Set the LCD dimensions (columns x rows)
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

// Create LCD object
hd44780_I2Cexp lcd;

char keys[4][4] = {
 {'1', '2', '3', 'A'},
 {'4', '5', '6', 'B'},
 {'7', '8', '9', 'C'},
 {'*', '0', '#', 'D'}
};

byte rowPins[4] = {11, 10, 9, 8}; // connect to the row pinouts of the keypad
byte colPins[4] = {7, 6, 5, 4}; // connect to the column pinouts of the keypad

// initialize an instance of class Keypad
Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

String selectedMode = "";

void setup() {
  Serial.begin(9600); // Initialize the serial port and set the baud rate to 9600
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Select a Mode:");
}

void loop() {
  // Get the character input
  char keyPressed = myKeypad.getKey();

  if (keyPressed && !isdigit(keyPressed)) {
    selectedMode = keyPressed;
    switch(keyPressed) {
      case 'A' : 
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("CSS");
        break;
      case 'B' : 
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Tests");
        break;
      case 'C' : 
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Git Messages");
        break;
      case 'D': 
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("React");
        break;
      case '*': 
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Select a Mode");
        selectedMode = "";
        break;
      default:
        break;
    }
  }else if(keyPressed && isdigit(keyPressed) && selectedMode != "") {
    Serial.println(selectedMode + keyPressed);
    lcd.setCursor(0, 1);
    lcd.print(keyPressed);
  }
}

