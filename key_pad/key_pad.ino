#include <Wire.h>
#include <Keypad.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
// define the symbols on the buttons of the keypad

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

String enteredNumber = "";

const char* exerciseList[] = {
  "Chose an Exercise",
  "Kb Sumo Squats",
  "Kb Squats",
  "Kb Deadlift",
  "Kb Pushup",
  "Pushup",
  "Biceps Kb Hammer",
  "Concentr Curl Kb",
  "Kb Biceps Curls",
  "Kb French Press",
  "Shoulder Press",
  "Kb upright row",
  "Kb Front raise",
  "Blue B lateral",
  "Blk B lateral",
  "Kb ab rota 40s",
  "Band Abs",
  "Band Tri pulld"
};

void setup() {
  Serial.begin(9600); // Initialize the serial port and set the baud rate to 9600
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Chose a Exercise");
}

void loop() {
  // Get the character input
  char keyPressed = myKeypad.getKey();

  // If there is a character input, sent it to the serial port
  if (keyPressed && isdigit(keyPressed)) {
    enteredNumber += keyPressed;
    Serial.println(keyPressed);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(exerciseList[enteredNumber.toInt()]);
    lcd.setCursor(0, 1);
    lcd.print(enteredNumber);
  }else{
    if(keyPressed == 'C') {
      enteredNumber = "";
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Chose a Exercise");
      lcd.setCursor(0, 1);
      lcd.print(enteredNumber);
    }
  }
}

