#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

// Pin configuration for buzzer
const int buzzerPin = 9;

// Pin configuration for LED
const int ledPin = 13;

// Potentiometer pin
const int potPin = A0;

// Tempo variables
int tempo = 120;
unsigned long beatInterval;
unsigned long previousMillis = 0;
bool isBuzzerOn = false;
bool isLedOn = false;

// Set the LCD address
const int I2C_ADDR = 0x27;  // Replace with your LCD's address

// Set the LCD dimensions (columns x rows)
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

// Create LCD object
hd44780_I2Cexp lcd;

void setup() {
  // Initialize the LCD
  lcd.begin(LCD_COLS, LCD_ROWS);
  
  // Set buzzer pin as output
  pinMode(buzzerPin, OUTPUT);
  
  // Set LED pin as output
  pinMode(ledPin, OUTPUT);

  // Calculate beat interval based on tempo
  calculateBeatInterval();
}

void loop() {
  // Read the potentiometer value
  int potValue = analogRead(potPin);
  
  // Map the potentiometer value to tempo range
  tempo = map(potValue, 0, 1023, 40, 240);
  
  // Calculate the beat interval
  calculateBeatInterval();
  
  // Check if it's time for a beat
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= beatInterval) {
    // Toggle buzzer state
    isBuzzerOn = !isBuzzerOn;
    
    // Display tempo on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tempo: ");
    lcd.print(tempo);
    lcd.print(" BPM");
    
    // Activate/deactivate buzzer
    if (isBuzzerOn) {
      tone(buzzerPin, 1000);  // Play a sound on the buzzer
    } else {
      noTone(buzzerPin);  // Stop the sound on the buzzer
    }
    
    // Toggle LED state
    isLedOn = !isLedOn;
    digitalWrite(ledPin, isLedOn ? HIGH : LOW);
    
    // Store current time
    previousMillis = currentMillis;
  }
}

void calculateBeatInterval() {
  beatInterval = 60000 / tempo; // Convert tempo to beat interval in milliseconds
}
