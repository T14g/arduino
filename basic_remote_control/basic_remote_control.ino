#include <IRremote.h>

int RECV_PIN = 12; // Infrared receiving pin

IRrecv irrecv(RECV_PIN); // Create a class object used to receive class
decode_results results; // Create a decoding results class object

void setup() {
  Serial.begin(9600); // Initialize the serial port and set the baud rate to 9600
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) { // Waiting for decoding
    Serial.println(results.value, HEX); // Print out the decoded results
    irrecv.resume(); // Receive the next value
  }
 delay(100);
}
