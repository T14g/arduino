#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

// Pin configuration for LEDs
const int ledPin = 13;
const int ledPin1 = 10;
const int ledPin2 = 11;
const int ledPin3 = 12;
const int potPin = A0; //potentiometer

// Tempo variables
int tempo = 120;
int previousTempo = 0;
unsigned long beatInterval;
unsigned long previousMillis = 0;
bool isLedOn = false;

// Set the LCD address
const int I2C_ADDR = 0x27;  // Replace with your LCD's address

// Set the LCD dimensions (columns x rows)
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

// Create LCD object
hd44780_I2Cexp lcd;

// 4/4 Time Signature
const int beatsPerMeasure = 4;
int beatCounter = 0;

void setup() {
  // Initialize the LCD
  lcd.begin(LCD_COLS, LCD_ROWS);

  // Set LED pins as output
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  // Calculate beat interval based on tempo
  calculateBeatInterval();
}

void loop() {
  // Read the potentiometer value
  int potValue = analogRead(potPin);

  // Map the potentiometer value to tempo range
  tempo = map(potValue, 0, 1023, 40, 240);

  // Calculate the beat interval
   // Only update the LCD if the tempo value has changed
    if (tempo != previousTempo) {
      // Calculate the beat interval
      calculateBeatInterval();

      // Update LCD with tempo information
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Tempo: ");
      lcd.print(tempo);
      lcd.print(" BPM");

      // Update the previous tempo value
      previousTempo = tempo;
    }
  // Check if it's time for a beat
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= beatInterval) {
    // Toggle LED state for the current beat
    if (beatCounter == 1) {
      digitalWrite(ledPin, HIGH);   // First LED on
      digitalWrite(ledPin1, LOW);   // Second LED off
      digitalWrite(ledPin2, LOW);   // Third LED off
      digitalWrite(ledPin3, LOW);   // Fourth LED off
    } else if (beatCounter == 2) {
      digitalWrite(ledPin, LOW);    // First LED off
      digitalWrite(ledPin1, HIGH);  // Second LED on
      digitalWrite(ledPin2, LOW);   // Third LED off
      digitalWrite(ledPin3, LOW);   // Fourth LED off
    } else if (beatCounter == 3) {
      digitalWrite(ledPin, LOW);    // First LED off
      digitalWrite(ledPin1, LOW);   // Second LED off
      digitalWrite(ledPin2, HIGH);  // Third LED on
      digitalWrite(ledPin3, LOW);   // Fourth LED off
    } else if (beatCounter == 4) {
      digitalWrite(ledPin, LOW);    // First LED off
      digitalWrite(ledPin1, LOW);   // Second LED off
      digitalWrite(ledPin2, LOW);   // Third LED off
      digitalWrite(ledPin3, HIGH);  // Fourth LED on
    }

    // Increment beat counter
    beatCounter++;

    // Play the metronome sound
    if (beatCounter == 1 || beatCounter == 3) {
      tone(9, 1000, 100); // Play a click sound for the first and third beat
    } else {
      tone(9, 800, 100); // Play a softer click sound for the second and fourth beat
    }

    // Check if a measure is completed
    if (beatCounter > beatsPerMeasure) {
      beatCounter = 1; // Reset the beat counter for a new measure
    }

    // Store current time
    previousMillis = currentMillis;
  }
}

void calculateBeatInterval() {
  beatInterval = 60000 / tempo; // Convert tempo to beat interval in milliseconds
}
