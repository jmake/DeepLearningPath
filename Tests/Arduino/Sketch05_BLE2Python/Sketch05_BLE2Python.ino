#include <ArduinoBLE.h>

BLEService ledService("180A"); // Custom service UUID
BLEBoolCharacteristic connectChar("2A56", BLERead | BLEWrite); // Characteristic UUID

const int bluePin = 24;   // Blue LED pin
const int greenPin = 23;  // Green LED pin

void setup() {
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  
  digitalWrite(greenPin, HIGH);  // Green LED on at start (advertising)
  digitalWrite(bluePin, LOW);     // Blue LED off

  // Initialize BLE without Serial monitoring
  if (!BLE.begin()) {
    // If BLE initialization fails, the board will remain here.
    while (1);
  }

  BLE.setLocalName("BLE_Sense_Device");
  BLE.setAdvertisedService(ledService);
  ledService.addCharacteristic(connectChar);
  BLE.addService(ledService);

  BLE.advertise();
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    // Connection established: change LED to blue
    digitalWrite(greenPin, LOW);  // Turn off green LED
    digitalWrite(bluePin, HIGH);   // Turn on blue LED

    // Blink the blue LED while connected
    while (central.connected()) {
      digitalWrite(bluePin, HIGH);   // Turn on blue LED
      delay(100);                    // Wait for 100 milliseconds
      digitalWrite(bluePin, LOW);    // Turn off blue LED
      delay(400);                    // Wait for 400 milliseconds
    }

    // Connection lost: change LED back to green
    digitalWrite(bluePin, LOW);   // Turn off blue LED
    digitalWrite(greenPin, HIGH); // Turn on green LED
  } else {
    // When disconnected, use low-power logic for the green LED
    // Flash the green LED at a lower frequency to indicate waiting for connection
    digitalWrite(greenPin, HIGH);  // Turn on green LED
    delay(1000);                   // Wait for 1 second
    digitalWrite(greenPin, LOW);   // Turn off green LED
    delay(1000);                   // Wait for 1 second
  }
}

/*
#include <ArduinoBLE.h>

BLEService ledService("180A"); // Custom service UUID
BLEBoolCharacteristic connectChar("2A56", BLERead | BLEWrite); // Characteristic UUID

const int bluePin = 24;   // Blue LED pin
const int greenPin = 23;  // Green LED pin

void setup() {
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  
  digitalWrite(greenPin, HIGH);  // Green LED on at start (advertising)
  digitalWrite(bluePin, LOW);    // Blue LED off

  // Initialize BLE without Serial monitoring
  if (!BLE.begin()) {
    // If BLE initialization fails, the board will remain here.
    while (1);
  }

  BLE.setLocalName("BLE_Sense_Device");
  BLE.setAdvertisedService(ledService);
  ledService.addCharacteristic(connectChar);
  BLE.addService(ledService);

  BLE.advertise();
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    // Connection established: change LED to blue
    digitalWrite(greenPin, LOW);  // Turn off green LED
    digitalWrite(bluePin, HIGH);  // Turn on blue LED

    // Blink the blue LED while connected
    while (central.connected()) {
      digitalWrite(bluePin, HIGH);   // Turn on blue LED
      delay(100);                    // Wait for 500 milliseconds
      digitalWrite(bluePin, LOW);    // Turn off blue LED
      delay(500);                    // Wait for 500 milliseconds
    }

    // Connection lost: change LED back to green
    digitalWrite(bluePin, LOW);   // Turn off blue LED
    digitalWrite(greenPin, HIGH); // Turn on green LED
  }
}
*/