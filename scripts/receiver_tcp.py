import socket
import time

ESP32_IP = "192.168.1.1"  # ESP32 AP IP address
TCP_PORT = 4210 # Port number to connect to the ESP32

def loop():
    while True:
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
                sock.settimeout(10)
                print(f"Connecting to {ESP32_IP}:{TCP_PORT}")
                sock.connect((ESP32_IP, TCP_PORT)) # Connect to the ESP32

                # Receive data from the ESP32
                data = sock.recv(2048)
                print("Received:", data.decode())

                sock.close() # Close the socket after receiving data
        except (socket.timeout, ConnectionRefusedError) as e: # Handle timeout or connection refused
            print("No response or connection refused:", e)

        time.sleep(1) # Wait before trying again


if __name__ == "__main__":
    loop()
