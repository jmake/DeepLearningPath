
Clear-Host


function SETUP() 
{ 
    Remove-Item -Recurse -Force -ErrorAction SilentlyContinue -Path build-path
    Remove-Item -Recurse -Force -ErrorAction SilentlyContinue -Path output-dir
    Remove-Item -Recurse -Force -ErrorAction SilentlyContinue -Path tmp 

    Get-ChildItem -Force

    & $ARDUINO_CLE_EXE  config set library.enable_unsafe_install true

    & $ARDUINO_CLE_EXE  lib install --git-url https://github.com/arduino-libraries/ArduinoBLE

    ##& $ARDUINO_CLE_EXE  lib install --git-url https://github.com/arduino-libraries/Arduino_LSM6DS3

    & $ARDUINO_CLE_EXE  lib install --git-url https://github.com/arduino-libraries/Arduino_BMI270_BMM150
} 

function COMPILER() 
{
    & $ARDUINO_CLE_EXE  lib list 

    & $ARDUINO_CLE_EXE compile `
    --clean `
    --fqbn arduino:mbed_nano:nano33ble `
    --build-cache-path ./tmp `
    --output-dir ./output-dir `
    --build-path ./build-path .

    #exit 
}


function INSTALLER() 
{
    if(Test-Path "output-dir") 
    {
        & $ARDUINO_CLE_EXE board list

        & $ARDUINO_CLE_EXE upload `
        --port COM4 `
        --fqbn arduino:mbed_nano:nano33ble `
        --input-dir output-dir
    }

    & $ARDUINO_CLE_EXE board list

}

$ARDUINO_CLE_EXE="F:\z2024_11\Arduino\arduino-cli.exe"
SETUP 
COMPILER
INSTALLER 


## See 'Nano33BLE' in chrome://bluetooth-internals/#devices


## python.exe .\Python\example1a.py


## cd WebApps\App2; 
## python -m http.server