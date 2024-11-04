void setup() 
{
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() 
{
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(5000);
}

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
