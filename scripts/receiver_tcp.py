import socket

ESP32_IP = "192.168.1.1"  # ESP32 AP IP address
TCP_PORT = 4210 # Port number to connect to the ESP32

def loop():
  while True:
    try:
      with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.settimeout(10)
        print(f"Connecting to {ESP32_IP}:{TCP_PORT}")
        sock.connect((ESP32_IP, TCP_PORT)) # Connect to the ESP32

        while True: # Keeps waiting for new data to arrive
          data = sock.recv(2048)
          if data:
            print("Received:", data.decode())
    except (socket.timeout, ConnectionRefusedError) as error: # Handle timeout or connection refused
      print("No response or connection refused:", error)

if __name__ == "__main__":
  loop()
