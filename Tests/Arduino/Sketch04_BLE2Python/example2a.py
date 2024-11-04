import serial
import struct
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
        # Read raw binary data (24 bytes for 6 floats)
        if ser.in_waiting >= 24:  # Check if enough bytes are available
            raw_data = ser.read(24)  # Read 24 bytes

            # Unpack the binary data into floats
            ax, ay, az, gx, gy, gz = struct.unpack('ffffff', raw_data[:24])  # 'ffffff' means six floats
            print(f"Accelerometer: X = {ax:.3f} g, Y = {ay:.3f} g, Z = {az:.3f} g")
            print(f"Gyroscope: X = {gx:.3f} °/s, Y = {gy:.3f} °/s, Z = {gz:.3f} °/s")


except KeyboardInterrupt:
    print("Closing the connection.")

finally:
    ser.close()  # Ensure the connection is closed