# read UPD message from the ESP32

import socket

LOCAL_UDP_IP = "xxx.xxx.x.x" # Check this IP address
SHARED_UDP_PORT = 4210
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # Internet  # UDP
sock.bind((LOCAL_UDP_IP, SHARED_UDP_PORT))
def loop():
    while True:
        data, addr = sock.recvfrom(2048)
        print(data)
if __name__ == "__main__":
    loop()
   