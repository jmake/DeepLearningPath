/*

>> .\arduino-cli.exe --help

>> .\arduino-cli.exe sketch new MyFirstSketch

>> ..\arduino-cli.exe core update-index

>> ..\arduino-cli.exe board list
Port Protocol Type              Board Name          FQBN                        Core
COM3 serial   Serial Port (USB) Arduino Nano 33 BLE arduino:mbed_nano:nano33ble arduino:mbed_nano

## Error during build: Platform 'arduino:mbed_nano' not found: platform not installed
>> ../arduino-cli.exe core install arduino:mbed_nano

../arduino-cli.exe compile --clean --fqbn arduino:mbed_nano:nano33ble --build-cache-path ./tmp --output-dir ./output-dir --build-path ./build-path . 

Sketch uses 85328 bytes (8%) of program storage space. Maximum is 983040 bytes.
Global variables use 44560 bytes (16%) of dynamic memory, leaving 217584 bytes for local variables. Maximum is 262144 bytes.

Used platform     Version Path
arduino:mbed_nano 4.1.5   C:\Users\zvl_2\AppData\Local\Arduino15\packages\arduino\hardware\mbed_nano\4.1.5

>> Get-WMIObject Win32_SerialPort | Select-Object DeviceID, Caption, Description
COM3     USB Serial Device (COM3) USB Serial Device


../arduino-cli.exe upload --port COM3 --fqbn arduino:mbed_nano:nano33ble --input-dir output-dir

Device       : nRF52840-QIAA
Version      : Arduino Bootloader (SAM-BA extended) 2.0 [Arduino:IKXYZ]
Address      : 0x0
Pages        : 256
Page Size    : 4096 bytes
Total Size   : 1024KB
Planes       : 1
Lock Regions : 0
Locked       : none
Security     : false
Erase flash

Done in 0.000 seconds
Write 85336 bytes to flash (21 pages)
[==============================] 100% (21/21 pages)
Done in 3.493 seconds
New upload port: COM3 (serial)

SEE : 
https://arduino.github.io/arduino-cli/0.23/getting-started/
*/

// git clone https://github.com/arduino-libraries/Arduino_LSM9DS1
#include "Arduino_LSM9DS1.h"  // Include the IMU library for Nano 33 BLE

// Timing variables
unsigned long previousLedMillis = 0;  // Stores the last time the LED toggled
unsigned long previousPrintMillis = 0;  // Stores the last time IMU data was printed

// Interval constants
const unsigned long ledInterval = 250;  // Interval for LED blinking (milliseconds)
const unsigned long printInterval = 500;  // Interval for printing IMU data (milliseconds)

void setup() {
  Serial.begin(9600);  // Initialize Serial at 9600 baud
  while (!Serial);      // Wait for Serial to connect

  // Initialize the IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  
  Serial.println("IMU initialized successfully.");

  // Initialize the LED pin
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // Get the current time
  unsigned long currentMillis = millis();

  // Blink LED independently of other tasks
  if (currentMillis - previousLedMillis >= ledInterval) {
    previousLedMillis = currentMillis;  // Update the time
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  // Toggle LED state
  }

  // Print IMU data independently of LED blinking
  if (currentMillis - previousPrintMillis >= printInterval) {
    previousPrintMillis = currentMillis;  // Update the time

    float ax, ay, az;  // Accelerometer data
    float gx, gy, gz;  // Gyroscope data

    // Read and print accelerometer data
    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(ax, ay, az);
      Serial.print("Accelerometer: ");
      Serial.print("X = ");
      Serial.print(ax);
      Serial.print(" g, Y = ");
      Serial.print(ay);
      Serial.print(" g, Z = ");
      Serial.print(az);
      Serial.println(" g");
    }

    // Read and print gyroscope data
    if (IMU.gyroscopeAvailable()) {
      IMU.readGyroscope(gx, gy, gz);
      Serial.print("Gyroscope: ");
      Serial.print("X = ");
      Serial.print(gx);
      Serial.print(" °/s, Y = ");
      Serial.print(gy);
      Serial.print(" °/s, Z = ");
      Serial.print(gz);
      Serial.println(" °/s");
    }
  }
}
