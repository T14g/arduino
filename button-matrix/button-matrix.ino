#include <Keypad.h>

const byte numRows = 4; // Number of rows in the keypad
const byte numCols = 4; // Number of columns in the keypad

// Define the key map - this should match the wiring of your button matrix
char keyMap[numRows][numCols] = {
  {'A', '3', '2', '1'},
  {'B', '6', '5', '4'},
  {'C', '9', '8', '7'},
  {'D', '*', '0', '#'}
};

byte rowPins[numRows] = {6, 7, 8, 9}; // Connect these pins to the row pinouts of the button matrix
byte colPins[numCols] = {2, 3, 4, 5}; // Connect these pins to the column pinouts of the button matrix

Keypad keypad = Keypad(makeKeymap(keyMap), rowPins, colPins, numRows, numCols);

void setup() {
  Serial.begin(9600);
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    Serial.println(key);
  }
}
