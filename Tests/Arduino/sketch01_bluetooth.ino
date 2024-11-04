#include <ArduinoBLE.h> // Arduino Nano 33 BLE

class BluetoothDevice 
{
  public:
    BluetoothDevice() {
      // Constructor
    }

    void begin() {
      // Start BLE
      if (!BLE.begin()) {
        Serial.println("Starting BLE failed!");
        while (1);
      }

      // Set up the custom service and characteristic
      BLE.setLocalName("SpicyTechAI");          // Set device name
      BLE.setAdvertisedService(customService);  // Advertise the custom service
      customService.addCharacteristic(customChar);
      BLE.addService(customService);

      // Set initial value for the characteristic
      customChar.writeValue(0);

      // Start advertising
      BLE.advertise();
      Serial.println("Bluetooth device active, waiting for connections...");
    }

    void loop() {
      // Listen for BLE central connections
      BLEDevice central = BLE.central();

      if (central) {
        Serial.print("Connected to central: ");
        Serial.println(central.address());

        // Wait for central to disconnect
        while (central.connected()) {
          // Check if the characteristic has been written
          if (customChar.written()) {
            int value = customChar.value();
            Serial.print("Characteristic value: ");
            Serial.println(value);

            // Update characteristic with new value if needed
            customChar.writeValue(value + 1);
          }
        }

        Serial.print("Disconnected from central: ");
        Serial.println(central.address());
      }
      else 
      {
        //Serial.println("Waiting...");
      }
    }

  private:
    BLEService customService = BLEService("180C");  
    BLEUnsignedCharCharacteristic customChar = BLEUnsignedCharCharacteristic("2A56", BLERead | BLEWrite); 
};


// Create an instance of the BluetoothDevice class
BluetoothDevice bluetooth;

void setup() 
{
  Serial.begin(9600);
  while (!Serial);

  bluetooth.begin();  // Initialize Bluetooth
}

void loop() 
{
  bluetooth.loop();  // Call the loop function of the BluetoothDevice
}
