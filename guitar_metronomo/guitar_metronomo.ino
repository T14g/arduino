#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <SoftwareSerial.h>

SoftwareSerial espSerial(2, 3); // RX, TX pins for ESP01 module

// Replace with your network credentials
const char* ssid = "NETWORK";
const char* password = "NETWORK PASSWORD";

const int START_OPTION = 0;
const int END_OPTION = 2;

// Pin configuration for LEDs
const int ledPin = 13;
const int ledPin1 = 10;
const int ledPin2 = 11;
const int ledPin3 = 12;
const int potPin = A0; //potentiometer

const int prevPin = 7;
const int playPin = 6;
const int nextPin = 5;

int reading1 = 0;
int reading2 = 0;
int reading3 = 0;

int prevState = 0;
int prevPrevState = 0;
int playState = 0;
int prevPlayState = 0;
int nextState = 0;
int prevNextState = 0;

int currentMode = 0;
int switchingMode = 1;

unsigned long debounceDelay = 50;    // Debounce delay in milliseconds

unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
unsigned long lastDebounceTime3 = 0;

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
  Serial.begin(115200); // Serial communication with Arduino IDE
  espSerial.begin(115200); // Serial communication with ESP01 module
  // Initialize the LCD
  lcd.begin(LCD_COLS, LCD_ROWS);

  // Set LED pins as output
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(prevPin, INPUT);
  pinMode(playPin, INPUT);
  pinMode(nextPin, INPUT);

  // Calculate beat interval based on tempo
  calculateBeatInterval();
}

void loop() {
  buttons();
  switch(currentMode) {
    case 0 :
      metronome();
      switchingMode = 0;
      break;
    case 1: 
      scales();
      switchingMode = 0;
      break;
    case 2:
      chords();
      switchingMode = 0;
      break;
    default:
      switchingMode = 0;
      break;
  }
}

String getModeName() {
    switch(currentMode) {
    case 0 :
      return "Metronome";
    case 1: 
      return "Scales";
    case 2:
      return "Chords";
    default:
      return "Mode not selected";
  }
}

void sendRequest () {
    Serial.println("Starting ESP01 module...");

      // Send an HTTP request to the server
      String payloadData = "mode=" + getModeName() + "&tempo=" + tempo;  // Example payload data
      String request = "POST /arduino/teste.php HTTP/1.1\r\n";
      request += "Host: teste.com\r\n";
      request += "Connection: close\r\n";
      request += "User-Agent: Arduino\r\n";
      request += "Accept: */*\r\n";
      request += "Content-Type: application/x-www-form-urlencoded\r\n";
      request += "Content-Length: " + String(payloadData.length()) + "\r\n";
      request += "\r\n";
      request += payloadData;

      sendCommand("AT+CIPSTART=\"TCP\",\"teste.com\",80\r\n", 5000); // Connect to the server
      sendCommand(("AT+CIPSEND=" + String(request.length()) + "\r\n").c_str(), 2000); // Send request length
      sendCommand(request.c_str(), 5000); // Send the HTTP request

      // Wait for the response
      delay(500);
      while (espSerial.available()) {
        String response = espSerial.readStringUntil('\n');
        Serial.println(response);
      }

      sendCommand("AT+CIPCLOSE\r\n", 2000); // Close the TCP connection

      delay(5000); // Wait for 5 seconds before sending the next request

}

void sendCommand(const char* command, int timeout) {
  espSerial.print(command);
  delay(10);

  long int time = millis();
  while ((time + timeout) > millis()) {
    while (espSerial.available()) {
      char c = espSerial.read();
      Serial.write(c);
    }
  }
}

void connectToWiFi() {
  String cmd = "AT+CWJAP=\"";
  cmd += ssid;
  cmd += "\",\"";
  cmd += password;
  cmd += "\"\r\n";
  sendCommand(cmd.c_str(), 10000); // Connect to Wi-Fi network

  sendCommand("AT+CIFSR\r\n", 5000); // Get IP address
}

void buttons() {
    reading1 = digitalRead(prevPin);
    reading2 = digitalRead(playPin);
    reading3 = digitalRead(nextPin);
  
    if(reading1 != prevPrevState) {
      lastDebounceTime1 = millis();   
    }

    if(reading2 != prevPlayState) {
      lastDebounceTime2 = millis();   
    }

    if(reading3 != prevNextState) {
      lastDebounceTime3 = millis();   
    }

    //Prev
    if ((millis() - lastDebounceTime1) > debounceDelay) {
      // If the button state is HIGH and different from the previous state, execute the command
      if (reading1 == HIGH && reading1 != prevState) {
        // Your command here
        Serial.println("Prev pressed!");
          if(currentMode > START_OPTION) {
          currentMode = currentMode - 1;
          switchingMode = 1;
        }
      }

      prevState = reading1;   // Update the button state
    }

    prevPrevState = reading1;

    if ((millis() - lastDebounceTime2) > debounceDelay) {
      // If the button state is HIGH and different from the previous state, execute the command
      if (reading2 == HIGH && reading2 != playState) {
        // Your command here
        Serial.println("Play pressed!");
          // Serial.println("Sending Request");
          sendCommand("AT\r\n", 2000); // Send AT command to ESP01 module
          sendCommand("AT+RST\r\n", 5000); // Reset ESP01 module
          connectToWiFi();
          sendRequest();
      }

      playState = reading2;   // Update the button state
    }

    prevPlayState = reading2;

    if ((millis() - lastDebounceTime3) > debounceDelay) {
      // If the button state is HIGH and different from the previous state, execute the command
      if (reading3 == HIGH && reading3 != nextState) {
        // Your command here
        Serial.println("Next pressed!");
        if(currentMode < END_OPTION) {
          currentMode = currentMode + 1;
          switchingMode = 1;
        }
      }

      nextState = reading3;   // Update the button state
    }

    prevNextState = reading3;
}

void turnLedsOFF() {
    int ledState = digitalRead(ledPin);
    int ledState1 = digitalRead(ledPin1);
    int ledState2 = digitalRead(ledPin2);
    int ledState3 = digitalRead(ledPin3);

    if(ledState == HIGH || ledState1 == HIGH || ledState2 == HIGH || ledState3 == HIGH) {
      digitalWrite(ledPin, LOW);    // First LED off
      digitalWrite(ledPin1, LOW);  // Second LED on
      digitalWrite(ledPin2, LOW);   // Third LED off
      digitalWrite(ledPin3, LOW);   // Fourth LED off
    }
}

void scales() {
    turnLedsOFF();
    int potValue = analogRead(potPin);
    // Map the potentiometer value to tempo range
    tempo = map(potValue, 0, 1023, 1, 240);

    if (tempo != previousTempo) {
      // Update LCD with tempo information
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Scales Mode: ");
      lcd.setCursor(0, 1);
      lcd.print(tempo);
      lcd.print(" Mins");

      // Update the previous tempo value
      previousTempo = tempo;
    }
}

void chords() {
    turnLedsOFF();
    int potValue = analogRead(potPin);
   // Map the potentiometer value to tempo range
    tempo = map(potValue, 0, 1023, 1, 240);

    if (tempo != previousTempo) {
      // Update LCD with tempo information
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Chords Mode: ");
      lcd.setCursor(0, 1);
      lcd.print(tempo);
      lcd.print(" Mins");

      // Update the previous tempo value
      previousTempo = tempo;
    }

}

void metronome() {
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
