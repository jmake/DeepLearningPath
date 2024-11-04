import serial
import time

# Set up the serial connection to COM3 at 9600 baud rate
arduino_port = "COM3"  # Change if necessary
baud_rate = 9600
timeout = 1  # Timeout in seconds

# Initialize the serial connection
ser = serial.Serial(arduino_port, baud_rate, timeout=timeout)
time.sleep(2)  # Wait for the connection to initialize

print("Connected to Arduino on", arduino_port)

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