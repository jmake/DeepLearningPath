import serial
import time

arduino_port = "COM5"  
baud_rate = 9600
timeout = 1  # Timeout in seconds

# Initialize the serial connection
ser = serial.Serial(arduino_port, baud_rate, timeout=timeout)
time.sleep(2)  # Wait for the connection to initialize

print(f"Connected to Arduino on '{arduino_port}'")

print( ser.get_settings() )

try:
    while True:
        if ser.in_waiting > 0:  # Check if there is data to read
            line = ser.readline().decode('utf-8').strip()  # Read and decode the data
            print(line)  # Print the incoming data to the console

except KeyboardInterrupt:
    print("Closing the connection.")

finally:
    ser.close()  # Ensure the connection is closed


r"""
python.exe -m venv Env1a

.\Env1a\Scripts\Activate.ps1

python.exe -m pip install pyserial 


"""