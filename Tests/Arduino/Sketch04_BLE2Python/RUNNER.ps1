
../arduino-cli.exe compile --clean --fqbn arduino:mbed_nano:nano33ble --build-cache-path ./tmp --output-dir ./output-dir --build-path ./build-path .

../arduino-cli.exe upload --port COM3 --fqbn arduino:mbed_nano:nano33ble --input-dir output-dir

## python.exe .\example1a.py