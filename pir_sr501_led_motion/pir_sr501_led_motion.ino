# Motion detection using Pir SR501
int pirPin = 2;  // PIR sensor output pin
int ledPin = 3;  // LED pin
int ledState = LOW;  // LED state
int motionState = LOW;  // Motion state
unsigned long previousMillis = 0;  // Time of last motion detection
unsigned long motionInterval = 5000;  // Time interval for LED to stay on (in milliseconds)

void setup() {
  pinMode(pirPin, INPUT);  // PIR sensor pin as input
  pinMode(ledPin, OUTPUT);  // LED pin as output
  Serial.begin(9600);  // Optional: Open serial communication for debugging
}

void loop() {
  motionState = digitalRead(pirPin);  // Read motion sensor state

  if (motionState == HIGH) {  // Motion detected
    digitalWrite(ledPin, HIGH);  // Turn on LED
    previousMillis = millis();  // Reset timer
    Serial.println("Motion detected!");
  }

  if (motionState == LOW && millis() - previousMillis >= motionInterval) {
    digitalWrite(ledPin, LOW);  // Turn off LED after motionInterval
    Serial.println("LED turned off");
  }
}
