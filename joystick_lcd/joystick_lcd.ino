#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Joystick pins
const int joystickXPin = A0;
const int joystickYPin = A1;

// Joystick threshold values for directions
const int joystickThreshold = 100;
const int joystickMaxValue = 1023;

// Joystick directions
enum JoystickDirection {
  CENTER,
  UP,
  DOWN,
  LEFT,
  RIGHT
};

// Menu options
const char* menuOptions[] = {
  "3 responsive cols",
  "5 responsive cols",
  "50/50 cols",
  "Reverse cols",
  "Dynamic selects"
};

// Current selected option
int selectedOption = 0;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
}

void loop() {
  // Read joystick values
  int joystickX = analogRead(joystickXPin);
  int joystickY = analogRead(joystickYPin);

  // Determine joystick direction
  JoystickDirection direction = getJoystickDirection(joystickX, joystickY);

  // Update selected option based on joystick direction
  switch (direction) {
    case UP:
      selectedOption--;
      if (selectedOption < 0) {
        selectedOption = sizeof(menuOptions) / sizeof(menuOptions[0]) - 1;
      }
      break;
    case DOWN:
      selectedOption++;
      if (selectedOption >= sizeof(menuOptions) / sizeof(menuOptions[0])) {
        selectedOption = 0;
      }
      break;
    default:
      break;
  }

  // Display menu option on OLED
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println(menuOptions[selectedOption]);
  display.display();
  delay(100);
}

JoystickDirection getJoystickDirection(int x, int y) {
  if (x > joystickThreshold && x < joystickMaxValue - joystickThreshold) {
    if (y > joystickThreshold && y < joystickMaxValue - joystickThreshold) {
      return CENTER;
    } else if (y <= joystickThreshold) {
      return UP;
    } else if (y >= joystickMaxValue - joystickThreshold) {
      return DOWN;
    }
  } else if (x <= joystickThreshold) {
    return LEFT;
  } else if (x >= joystickMaxValue - joystickThreshold) {
    return RIGHT;
  }

  return CENTER; // Default to CENTER if no direction is detected
}
