# Receiver script for ESP32 TCP communication.
# This script connects to an ESP32 device over TCP and listens for incoming data.
import socket

ESP32_IP = "192.168.1.1"  # ESP32 AP IP address
TCP_PORT = 4210 # Shared port for TCP communication

def loop():
  while True:
    try:
      with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.settimeout(10) # Set a timeout for the connection
        print(f"Connecting to {ESP32_IP}:{TCP_PORT}")
        sock.connect((ESP32_IP, TCP_PORT)) # Connect to the ESP32

        while True: # Keep waiting for new data to arrive
          data = sock.recv(2048) # Store the received data
          if data:
            print("Received:", data.decode())
    except (socket.timeout, ConnectionRefusedError) as timeoutError: # Handle timeout or connection refused
      print("No response or connection refused:", timeoutError)

if __name__ == "__main__":
  loop()
