import socket

LOCAL_UDP_IP = "192.168.1.2" # This computers local IP address
SHARED_UDP_PORT = 4210 # Shared port for receiving UDP packets
# Create an UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) 
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

while True:
    try:
        sock.bind((LOCAL_UDP_IP, SHARED_UDP_PORT)) # Bind to the specified IP and port
        print(f"Socket binded to {LOCAL_UDP_IP}:{SHARED_UDP_PORT}")
        break
    except OSError as e: 
        if e.winerror == 10048: # Handles the issue, when the python script is restarted and the port is still in use
            break
        elif e.winerror == 10013: # Handles the issue, when the python script is restarted and the port is still in use
            break
        else:
            raise # Unexpected error

sock.settimeout(5)  # timeout in seconds

def loop():
    while True:
        try:
            data, addr = sock.recvfrom(2048)
            print("Received from:", addr)
            print("Raw data:", data)
        except socket.timeout:
            print("Waiting for data... (no data received in last 5 seconds)")


if __name__ == "__main__":
    loop()
