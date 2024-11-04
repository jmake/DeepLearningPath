#include <ArduinoBLE.h>

class BluetoothLED {
public:
    // Constructor
    BluetoothLED(int ledPin) : _ledPin(ledPin) {
        pinMode(_ledPin, OUTPUT);
    }

    // Initialize BLE
    void begin() {
        Serial.begin(9600);
        while (!Serial);

        if (!BLE.begin()) {
            Serial.println("Starting BLE failed!");
            while (1);
        }

        // Start advertising
        BLE.advertise();
        Serial.println("BLE device is ready to connect.");
    }

    // Poll for BLE events
    void poll() {
        BLE.poll();
    }

    // Turn the LED on
    void ledOn() {
        digitalWrite(_ledPin, HIGH);
        Serial.println("LED ON");
    }

    // Turn the LED off
    void ledOff() {
        digitalWrite(_ledPin, LOW);
        Serial.println("LED OFF");
    }

private:
    int _ledPin; // Pin for the LED
};

// Create an instance of the BluetoothLED class
BluetoothLED btLed(LED_BUILTIN);

void setup() 
{
    btLed.begin(); // Initialize BLE
}

void loop() 
{
    btLed.poll(); // Poll for BLE events

    // Example: toggle LED every second
    btLed.ledOn();   // Turn LED ON
    delay(1000);
    btLed.ledOff();  // Turn LED OFF
    delay(1000);
}

