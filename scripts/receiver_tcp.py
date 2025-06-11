# Code for a PC receiving data from an AP ESP32

import socket
import time

ESP32_IP = "192.168.1.1"  # ESP32 AP IP address
TCP_PORT = 4210

def loop():
    while True:
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock: # Create a TCP socket
                sock.settimeout(10) # Set a timeout for the connection attempt
                print(f"Connecting to {ESP32_IP}:{TCP_PORT}")
                sock.connect((ESP32_IP, TCP_PORT)) # Connect to the ESP32
                
                # Print received data from the ESP32
                data = sock.recv(2048)
                print("Received:", data.decode())

                sock.close() # Close the socket after receiving data
        except (socket.timeout, ConnectionRefusedError) as e: # Handle timeout or connection refused
            print("No response or connection refused:", e)

        time.sleep(1) # Wait before retrying

if __name__ == "__main__":
    loop()
