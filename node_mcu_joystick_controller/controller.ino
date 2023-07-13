#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int joystickPinX = A0;
int joystickThreshold = 512;
int currentMenuOption = 0;
int maxMenuOptions = 3;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Initialize with the I2C address of your display
  display.clearDisplay();

  pinMode(joystickPinX, INPUT);
  Serial.begin(115200);
}

void loop() {
  int joystickValueX = analogRead(joystickPinX);
  
  if (joystickValueX < (joystickThreshold - 100)) {
    currentMenuOption--;
    if (currentMenuOption < 0) {
      currentMenuOption = maxMenuOptions - 1;
    }
    Serial.print("Switched to Menu Option ");
    Serial.println(currentMenuOption + 1);
    while (analogRead(joystickPinX) < (joystickThreshold - 100)) {
      delay(10);
    }
    delay(200);
  } else if (joystickValueX > (joystickThreshold + 100)) {
    currentMenuOption++;
    if (currentMenuOption >= maxMenuOptions) {
      currentMenuOption = 0;
    }
    Serial.print("Switched to Menu Option ");
    Serial.println(currentMenuOption + 1);
    while (analogRead(joystickPinX) > (joystickThreshold + 100)) {
      delay(10);
    }
    delay(200);
  }
  
  displayMenuOption();
}

void displayMenuOption() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  
  switch (currentMenuOption) {
    case 0:
      display.setCursor(0, 0);
      display.println("Enviar atas");  // Display Menu Option 1
      break;
    case 1:
      display.setCursor(0, 0);
      display.println("Enviar pendencias");  // Display Menu Option 2
      break;
    case 2:
      display.setCursor(0, 0);
      display.println("Enviar email");  // Display Menu Option 3
      break;
    default:
      break;
  }
  
  display.display();
}
